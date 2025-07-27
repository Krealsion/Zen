#include "input.h"

#include "timer.h"
#include "window.h"

namespace Zen {

Window* Input::_window = nullptr;
std::unordered_map<SDL_Scancode, InputState> Input::_key_states;
std::vector<KeyCombo> Input::_combo_list;
std::vector<InputState> Input::_combo_states;
std::unordered_multimap<SDL_Scancode, size_t> Input::_combos_by_scancode;
MouseState Input::_mouse_state;

std::string Input::_text_input;
bool Input::_text_input_enabled = false;
std::function<void(std::string)> Input::_update_text_input_callback = nullptr;

std::vector<std::tuple<TriggerType, KeyCombo, std::function<void(int)>, int>> Input::_key_callbacks;
std::vector<std::tuple<TriggerType, MouseButton, std::function<void(int)>, int>> Input::_mouse_callbacks;

std::queue<TriggerEvent> Input::_pending_triggers;

int Input::_default_tap_window_ms = 200;
int Input::_default_double_tap_duration_ms = 300;

bool Input::_quick_tap_prevention_enabled = true;
int Input::_quick_tap_buffer_ms = 100;

std::unordered_map<SDL_Scancode, int> Input::_scancode_to_group;
std::unordered_map<int, SDL_Scancode> Input::_group_to_canonical;
int Input::_next_modifier_group = 0;

std::vector<SDL_Scancode> Input::_pressed_keys;

std::mutex Input::_input_mutex;

Input::Input() {
  _mouse_state.button_states.resize(5);

  SDL_SetGamepadEventsEnabled(true);

  // Set up standard modifier groups
  int shift_group = _next_modifier_group++;
  int ctrl_group = _next_modifier_group++;
  int alt_group = _next_modifier_group++;
  int gui_group = _next_modifier_group++;
  int caps_group = _next_modifier_group++;
  int num_group = _next_modifier_group++;
  int scroll_group = _next_modifier_group++;

  _scancode_to_group[SDL_SCANCODE_LSHIFT] = shift_group;
  _scancode_to_group[SDL_SCANCODE_RSHIFT] = shift_group;
  _group_to_canonical[shift_group] = SDL_SCANCODE_LSHIFT;

  _scancode_to_group[SDL_SCANCODE_LCTRL] = ctrl_group;
  _scancode_to_group[SDL_SCANCODE_RCTRL] = ctrl_group;
  _group_to_canonical[ctrl_group] = SDL_SCANCODE_LCTRL;

  _scancode_to_group[SDL_SCANCODE_LALT] = alt_group;
  _scancode_to_group[SDL_SCANCODE_RALT] = alt_group;
  _group_to_canonical[alt_group] = SDL_SCANCODE_LALT;

  _scancode_to_group[SDL_SCANCODE_LGUI] = gui_group;
  _scancode_to_group[SDL_SCANCODE_RGUI] = gui_group;
  _group_to_canonical[gui_group] = SDL_SCANCODE_LGUI;

  _scancode_to_group[SDL_SCANCODE_CAPSLOCK] = caps_group;
  _group_to_canonical[caps_group] = SDL_SCANCODE_CAPSLOCK;

  _scancode_to_group[SDL_SCANCODE_NUMLOCKCLEAR] = num_group;
  _group_to_canonical[num_group] = SDL_SCANCODE_NUMLOCKCLEAR;

  _scancode_to_group[SDL_SCANCODE_SCROLLLOCK] = scroll_group;
  _group_to_canonical[scroll_group] = SDL_SCANCODE_SCROLLLOCK;
}

Input::~Input() {}

void Input::set_active_window(Window* window) {
  std::lock_guard<std::mutex> lock(_input_mutex);
  _window = window;
}

void Input::update_input() {
  std::lock_guard<std::mutex> lock(_input_mutex);
  SDL_PumpEvents();
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_EVENT_QUIT) {
      exit(0); // TODO: Handle properly
    }
    if (_text_input_enabled) {
      if (event.type == SDL_EVENT_KEY_DOWN) {
        if (event.key.key == SDLK_ESCAPE || event.key.key == SDLK_RETURN) {
          if (_update_text_input_callback) {
            _update_text_input_callback("\0");
          }
          end_text_input();
        }
      } else if (event.type == SDL_EVENT_TEXT_INPUT) {
        _text_input += event.text.text;
        if (_update_text_input_callback) {
          _update_text_input_callback(_text_input);
        }
      }
    } else {
      _handle_event(event);
    }
  }
}

void Input::clean_input() {
  _mouse_state.dx = 0.0f;
  _mouse_state.dy = 0.0f;
  _mouse_state.wheel_x = 0.0f;
  _mouse_state.wheel_y = 0.0f;
}

void Input::process_input_callbacks() {
  std::lock_guard<std::mutex> lock(_input_mutex);
  while (!_pending_triggers.empty()) {
    TriggerEvent te = _pending_triggers.front();
    _pending_triggers.pop();
    if (std::holds_alternative<KeyCombo>(te.source)) {
      const KeyCombo& combo = std::get<KeyCombo>(te.source);
      for (const auto& cb : _key_callbacks) {
        auto [cb_type, cb_combo, func, cb_duration] = cb;
        if (cb_type == te.type && cb_combo == combo) {
          if (cb_duration <= 0 || te.duration_ms >= cb_duration) {
            func(te.duration_ms);
          }
        }
      }
    } else if (std::holds_alternative<int>(te.source)) {
      int button = std::get<int>(te.source);
      for (const auto& cb : _mouse_callbacks) {
        auto [cb_type, cb_button, func, cb_duration] = cb;
        if (cb_type == te.type && cb_button == button) {
          if (cb_duration <= 0 || te.duration_ms >= cb_duration) {
            func(te.duration_ms);
          }
        }
      }
    }
  }
}

void Input::register_on_input_callback(TriggerType type, const KeyCombo& combo, std::function<void(int)> callback, int duration_ms) {
  std::lock_guard<std::mutex> lock(_input_mutex);
  _key_callbacks.emplace_back(type, combo, std::move(callback), duration_ms);
  _get_or_create_combo_state(combo);
  size_t index = _combo_states.size() - 1;
  _combos_by_scancode.emplace(combo.key, index);
  for (auto mod : combo.modifiers) {
    _combos_by_scancode.emplace(mod, index);
  }
}

void Input::register_on_mouse_callback(TriggerType type, MouseButton button, std::function<void(int)> callback, int duration_ms) {
  std::lock_guard<std::mutex> lock(_input_mutex);
  _mouse_callbacks.emplace_back(type, button, std::move(callback), duration_ms);
}

void Input::set_default_input_durations(int tap_window_ms, int double_tap_duration_ms) {
  std::lock_guard<std::mutex> lock(_input_mutex);
  _default_tap_window_ms = tap_window_ms;
  _default_double_tap_duration_ms = double_tap_duration_ms;
}

void Input::start_text_input() {
  SDL_StartTextInput(_window->get_window());
  _text_input_enabled = true;
  _text_input.clear();
}

template <typename Function>
void Input::start_text_input(Function callback) {
  _update_text_input_callback = callback;
  start_text_input();
}

std::string Input::get_text_input() {
  std::string result = _text_input;
  _text_input.clear();
  return result;
}

void Input::end_text_input() {
  SDL_StopTextInput(_window->get_window());
  _text_input_enabled = false;
  _update_text_input_callback = nullptr;
}

// Keyboard Functions
bool Input::is_key_down(SDL_Scancode key) {
  KeyCombo combo{key, {}};
  return is_key_down(combo);
}

bool Input::is_key_down(const KeyCombo& combo) {
  std::lock_guard<std::mutex> lock(_input_mutex);
  InputState& state = _get_or_create_combo_state(combo);
  Uint64 current_time = Timer::get_milliseconds_since_started();
  bool is_down = state.last_press_time > state.last_release_time;
  if (!is_down && _quick_tap_prevention_enabled && state.was_pressed_since_last_check) {
    if ((current_time - state.last_release_time) <= _quick_tap_buffer_ms) {
      is_down = true;
    }
  }
  state.was_pressed_since_last_check = false;
  return is_down;
}

int Input::get_key_pressed_duration(SDL_Scancode key) {
  KeyCombo combo{key, {}};
  return get_key_pressed_duration(combo);
}

int Input::get_key_pressed_duration(const KeyCombo& combo) {
  std::lock_guard<std::mutex> lock(_input_mutex);
  InputState& state = _get_or_create_combo_state(combo);
  if (is_key_down(combo)) {
    return static_cast<int>(Timer::get_milliseconds_since_started() - state.last_press_time);
  }
  return -1;
}

int Input::get_key_released_duration(SDL_Scancode key) {
  KeyCombo combo{key, {}};
  return get_key_released_duration(combo);
}

int Input::get_key_released_duration(const KeyCombo& combo) {
  std::lock_guard<std::mutex> lock(_input_mutex);
  InputState& state = _get_or_create_combo_state(combo);
  if (!is_key_down(combo)) {
    return static_cast<int>(Timer::get_milliseconds_since_started() - state.last_release_time);
  }
  return -1;
}

void Input::reset_keyboard() {
  std::lock_guard<std::mutex> lock(_input_mutex);
  SDL_ResetKeyboard();
  _key_states.clear();
  _combo_list.clear();
  _combo_states.clear();
  _combos_by_scancode.clear();
  _pressed_keys.clear();
}

void Input::set_quick_tap_prevention(bool enabled, int buffer_duration_ms) {
  std::lock_guard<std::mutex> lock(_input_mutex);
  _quick_tap_prevention_enabled = enabled;
  _quick_tap_buffer_ms = buffer_duration_ms;
}

void Input::add_custom_modifier(SDL_Scancode key) {
  std::lock_guard<std::mutex> lock(_input_mutex);
  if (_scancode_to_group.count(key) == 0) {
    int group = _next_modifier_group++;
    _scancode_to_group[key] = group;
    _group_to_canonical[group] = key;
  }
}

void Input::remove_custom_modifier(SDL_Scancode key) {
  std::lock_guard<std::mutex> lock(_input_mutex);
  auto it = _scancode_to_group.find(key);
  if (it != _scancode_to_group.end()) {
    int group = it->second;
    _scancode_to_group.erase(it);
    _group_to_canonical.erase(group);
  }
}

KeyCombo Input::listen_for_key_combo() {
  std::vector<SDL_Scancode> temp_pressed;
  SDL_Event event;
  while (true) {
    SDL_WaitEvent(&event);
    if (event.type == SDL_EVENT_KEY_DOWN) {
      SDL_Scancode sc = event.key.scancode;
      if (std::find(temp_pressed.begin(), temp_pressed.end(), sc) == temp_pressed.end()) {
        temp_pressed.push_back(sc);
      }
    } else if (event.type == SDL_EVENT_KEY_UP) {
      SDL_Scancode sc = event.key.scancode;
      auto it = std::find(temp_pressed.begin(), temp_pressed.end(), sc);
      if (it != temp_pressed.end()) {
        temp_pressed.erase(it);
      }
      if (_scancode_to_group.count(sc) == 0) { // not modifier
        std::unordered_set<int> pressed_groups;
        for (auto p : temp_pressed) {
          auto git = _scancode_to_group.find(p);
          if (git != _scancode_to_group.end()) {
            pressed_groups.insert(git->second);
          }
        }
        std::vector<SDL_Scancode> mods;
        for (auto g : pressed_groups) {
          mods.push_back(_group_to_canonical[g]);
        }
        std::sort(mods.begin(), mods.end());
        return {sc, mods};
      }
    }
  }
}

bool Input::is_mouse_button_down(MouseButton button) {
  std::lock_guard<std::mutex> lock(_input_mutex);
  int index = button - 1;
  if (index < 0 || index >= _mouse_state.button_states.size()) return false;
  InputState& state = _mouse_state.button_states[index];
  Uint64 current_time = Timer::get_milliseconds_since_started();
  bool is_down = state.last_press_time > state.last_release_time;
  if (!is_down && _quick_tap_prevention_enabled && state.was_pressed_since_last_check) {
    if ((current_time - state.last_release_time) <= _quick_tap_buffer_ms) {
      is_down = true;
    }
  }
  state.was_pressed_since_last_check = false;
  return is_down;
}

Vector2 Input::get_mouse_position() {
  std::lock_guard<std::mutex> lock(_input_mutex);
  return {_mouse_state.x, _mouse_state.y};
}

Vector2 Input::get_mouse_delta() {
  std::lock_guard<std::mutex> lock(_input_mutex);
  return {_mouse_state.dx, _mouse_state.dy};
}

void Input::get_mouse_wheel(float& wheel_x, float& wheel_y) {
  std::lock_guard<std::mutex> lock(_input_mutex);
  wheel_x = _mouse_state.wheel_x;
  wheel_y = _mouse_state.wheel_y;
}

void Input::warp_mouse(float x, float y) {
  SDL_WarpMouseInWindow(_window->get_window(), x, y);
}

bool Input::warp_mouse_absolute(float x, float y) {
  return SDL_WarpMouseGlobal(x, y);
}

void Input::warp_mouse_to_window(Window* window, float x, float y) {
  SDL_WarpMouseInWindow(window->get_window(), x, y);
}

bool Input::show_cursor() {
  return SDL_ShowCursor();
}

bool Input::hide_cursor() {
  return SDL_HideCursor();
}

bool Input::is_cursor_visible() {
  return SDL_CursorVisible();
}

bool Input::set_relative_mouse_mode(bool enabled) {
  return SDL_SetWindowRelativeMouseMode(_window->get_window(), enabled);
}

bool Input::get_relative_mouse_mode() {
  return SDL_GetWindowRelativeMouseMode(_window->get_window());
}

SDL_KeyboardID* Input::get_keyboards(int* count) {
  return SDL_GetKeyboards(count);
}

const char* Input::get_keyboard_name(SDL_KeyboardID instance_id) {
  return SDL_GetKeyboardNameForID(instance_id);
}

SDL_Window* Input::get_keyboard_focus() {
  return SDL_GetKeyboardFocus();
}

void Input::_handle_event(const SDL_Event& event) {
  switch (event.type) {
    case SDL_EVENT_KEY_DOWN:
      _handle_key_down(event.key);
      break;
    case SDL_EVENT_KEY_UP:
      _handle_key_up(event.key);
      break;
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
      _handle_mouse_button_down(event.button);
      break;
    case SDL_EVENT_MOUSE_BUTTON_UP:
      _handle_mouse_button_up(event.button);
      break;
    case SDL_EVENT_MOUSE_MOTION:
      _handle_mouse_motion(event.motion);
      break;
    case SDL_EVENT_MOUSE_WHEEL:
      _handle_mouse_wheel(event.wheel);
      break;
    default:
      break;
  }
}

void Input::_handle_key_down(const SDL_KeyboardEvent& event) {
  SDL_Scancode scancode = event.scancode;
  if (std::find(_pressed_keys.begin(), _pressed_keys.end(), scancode) == _pressed_keys.end()) {
    _pressed_keys.push_back(scancode);
  }

  auto& key_state = _key_states[scancode];
  key_state.last_press_time = event.timestamp;
  key_state.was_pressed_since_last_check = true;

  auto range = _combos_by_scancode.equal_range(scancode);
  for (auto it = range.first; it != range.second; ++it) {
    size_t index = it->second;
    auto& state = _combo_states[index];
    const KeyCombo& combo = _combo_list[index];
    if (_combo_matches(combo, _pressed_keys)) {
      state.last_press_time = event.timestamp;
      state.was_pressed_since_last_check = true;
      _queue_trigger(combo, TriggerType::PRESSED, 0);
      if (state.last_tap_start_time > 0 && (event.timestamp - state.last_tap_start_time < _default_double_tap_duration_ms)) {
        _queue_trigger(combo, TriggerType::DOUBLE_TAPPED, 0);
        state.last_tap_start_time = 0;
      } else {
        state.last_tap_start_time = event.timestamp;
      }
    }
  }
}

void Input::_handle_key_up(const SDL_KeyboardEvent& event) {
  SDL_Scancode scancode = event.scancode;
  auto pressed_it = std::find(_pressed_keys.begin(), _pressed_keys.end(), scancode);
  if (pressed_it != _pressed_keys.end()) {
    _pressed_keys.erase(pressed_it);
  }

  auto& key_state = _key_states[scancode];
  key_state.last_release_time = event.timestamp;

  auto range = _combos_by_scancode.equal_range(scancode);
  for (auto it = range.first; it != range.second; ++it) {
    size_t index = it->second;
    auto& state = _combo_states[index];
    const KeyCombo& combo = _combo_list[index];
    int duration = static_cast<int>(event.timestamp - state.last_press_time);
    state.last_release_time = event.timestamp;
    _queue_trigger(combo, TriggerType::RELEASED, duration);
    _queue_trigger(combo, TriggerType::HELD, duration);
    if (duration < _default_tap_window_ms) {
      _queue_trigger(combo, TriggerType::TAPPED, duration);
    }
  }
}

void Input::_handle_mouse_button_down(const SDL_MouseButtonEvent& event) {
  int index = event.button - 1;
  if (index >= 0 && index < _mouse_state.button_states.size()) {
    auto& state = _mouse_state.button_states[index];
    state.last_press_time = event.timestamp;
    state.was_pressed_since_last_check = true;
    _queue_trigger(static_cast<MouseButton>(event.button), TriggerType::PRESSED, 0);
    if (state.last_tap_start_time > 0 && (event.timestamp - state.last_tap_start_time < _default_double_tap_duration_ms)) {
      _queue_trigger(static_cast<MouseButton>(event.button), TriggerType::DOUBLE_TAPPED, 0);
      state.last_tap_start_time = 0;
    } else {
      state.last_tap_start_time = event.timestamp;
    }
  }
}

void Input::_handle_mouse_button_up(const SDL_MouseButtonEvent& event) {
  int index = event.button - 1;
  if (index >= 0 && index < _mouse_state.button_states.size()) {
    auto& state = _mouse_state.button_states[index];
    int duration = static_cast<int>(event.timestamp - state.last_press_time);
    state.last_release_time = event.timestamp;
    _queue_trigger(static_cast<MouseButton>(event.button), TriggerType::RELEASED, duration);
    _queue_trigger(static_cast<MouseButton>(event.button), TriggerType::HELD, duration);
    if (duration < _default_tap_window_ms) {
      _queue_trigger(static_cast<MouseButton>(event.button), TriggerType::TAPPED, duration);
    }
  }
}

void Input::_handle_mouse_motion(const SDL_MouseMotionEvent& event) {
  _mouse_state.x = event.x;
  _mouse_state.y = event.y;
  _mouse_state.dx += event.xrel;
  _mouse_state.dy += event.yrel;
}

void Input::_handle_mouse_wheel(const SDL_MouseWheelEvent& event) {
  if (event.direction == SDL_MOUSEWHEEL_FLIPPED) {
    _mouse_state.wheel_x -= event.x;
    _mouse_state.wheel_y -= event.y;
  } else {
    _mouse_state.wheel_x += event.x;
    _mouse_state.wheel_y += event.y;
  }
}

void Input::_queue_trigger(const KeyCombo& combo, TriggerType type, int duration_ms) {
  _pending_triggers.push({type, combo, duration_ms});
}

void Input::_queue_trigger(MouseButton button, TriggerType type, int duration_ms) {
  _pending_triggers.push({type, static_cast<int>(button), duration_ms});
}

InputState& Input::_get_or_create_combo_state(const KeyCombo& combo) {
  for (size_t i = 0; i < _combo_list.size(); ++i) {
    if (_combo_list[i] == combo) {
      return _combo_states[i];
    }
  }
  _combo_list.push_back(combo);
  _combo_states.push_back({});
  return _combo_states.back();
}

bool Input::_combo_matches(const KeyCombo& combo, const std::vector<SDL_Scancode>& pressed) {
  std::unordered_set<int> pressed_groups;
  std::unordered_set<SDL_Scancode> pressed_set(pressed.begin(), pressed.end());
  for (auto p : pressed) {
    auto it = _scancode_to_group.find(p);
    if (it != _scancode_to_group.end()) {
      pressed_groups.insert(it->second);
    }
  }

  // Check key
  auto key_it = _scancode_to_group.find(combo.key);
  if (key_it != _scancode_to_group.end()) {
    if (pressed_groups.count(key_it->second) == 0) return false;
  } else {
    if (pressed_set.count(combo.key) == 0) return false;
  }

  // Check modifiers
  for (auto mod : combo.modifiers) {
    auto mod_it = _scancode_to_group.find(mod);
    if (mod_it != _scancode_to_group.end()) {
      if (pressed_groups.count(mod_it->second) == 0) return false;
    } else {
      if (pressed_set.count(mod) == 0) return false;
    }
  }

  return true;
}

} // namespace Zen
