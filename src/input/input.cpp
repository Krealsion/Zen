#include "input.h"

#include "window.h"

namespace Zen {

std::vector<InputState> Input::_key_states = std::vector<InputState>(SDL_SCANCODE_COUNT);
MouseState Input::_mouse_state = MouseState();
Window* Input::_window = nullptr;


std::string Input::_text_input = "";
bool Input::_text_input_enabled = false;
std::function<void(std::string)> Input::_update_text_input_callback = nullptr;

Input::Input() {
  _key_states.resize(SDL_SCANCODE_COUNT);
  _mouse_state.button_states.resize(32); // For buttons 1-5 (left, middle, right, x1, x2)
  SDL_SetGamepadEventsEnabled(true); // Enable gamepad events by default
}

void Input::set_active_window(Window* window) {
  _window = window;
}

Input::~Input() {
}
void Input::clean() {
  _mouse_state.dx = 0.0f;
  _mouse_state.dy = 0.0f;
  _mouse_state.wheel_x = 0.0f;
  _mouse_state.wheel_y = 0.0f;
}

void Input::update_input() {
  SDL_PumpEvents();
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_EVENT_QUIT) {
      exit(0); // TODO clean exit
    }
    if (_text_input_enabled) {
      if (event.type == SDL_EVENT_KEY_DOWN) {
        /* the pressed key was Escape or Return? */
        if (event.key.key == SDLK_ESCAPE || event.key.key == SDLK_RETURN) {
          if (_update_text_input_callback) {
            _update_text_input_callback("\0");
          }
          end_text_input();
        }
        /* Handle arrow keys, etc. */
      } else if (event.type == SDL_EVENT_TEXT_INPUT) {
        char input[1024] = {0};
        SDL_strlcat(input, event.text.text, sizeof(input));
        if (_update_text_input_callback) {
          _update_text_input_callback(_text_input);
        } else {
          _text_input += std::string(input);
        }
      }
    } else {
      _handle_event(event);
    }
  }
}

void Input::start_text_input() {
  SDL_StartTextInput(_window->get_window());
  _text_input_enabled = true;
}

template <typename Function>
void Input::start_text_input(Function callback) {
  _update_text_input_callback = callback;
  start_text_input();
}

std::string Input::get_text_input() {
  std::string return_text = _text_input;
  _text_input = "";
  return return_text;
}

void Input::end_text_input() {
  SDL_StopTextInput(_window->get_window());
  _text_input_enabled = false;
  _update_text_input_callback = nullptr;
}


// Keyboard Functions
bool Input::is_key_down(SDL_Scancode key) {
  // TODO add some leeway in case button was tapped for a very short duration, so key presses aren't missed
  return (key != SDL_SCANCODE_UNKNOWN) && _key_states[key].last_press_time > _key_states[key].last_release_time;
}

bool Input::is_key_down(const KeyCombo& combo) {
  if (is_key_down(combo.key)) {
    Uint64 primary_key_time = _key_states[combo.key].last_press_time;
    for (auto& key : combo.modifiers) {
      if (!is_key_down(key) || _key_states[key].last_press_time > primary_key_time) {
        return false;
      }
    }
    return true;
  }
  return false;
}

int Input::get_key_pressed_duration(SDL_Scancode key) {
  Uint64 current_time = SDL_GetTicks();
  Uint64 duration_ms = current_time - _key_states[key].last_press_time;
  return static_cast<int>(duration_ms);
}

int Input::get_key_released_duration(SDL_Scancode key) {
  Uint64 current_time = SDL_GetTicks();
  Uint64 duration_ms = current_time - _key_states[key].last_release_time;
  return static_cast<int>(duration_ms);
}

void Input::reset_keyboard() {
  SDL_ResetKeyboard();
}

// Mouse Functions
bool Input::is_mouse_button_down(int button) {
  return (button >= 0 && button < _mouse_state.button_states.size()) &&
         _mouse_state.button_states[button].last_press_time > _mouse_state.button_states[button].last_release_time;
}

Vector2 Input::get_mouse_position() {
  return {_mouse_state.x, _mouse_state.y};
}

Vector2 Input::get_mouse_delta() {
  return {_mouse_state.dx, _mouse_state.dy};
}

void Input::get_mouse_wheel(float& wheel_x, float& wheel_y) {
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

// Enhanced Features

// Mouse Management
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

// Keyboard Management
SDL_KeyboardID* Input::get_keyboards(int* count) {
  return SDL_GetKeyboards(count);
}

const char* Input::get_keyboard_name(SDL_KeyboardID instance_id) {
  return SDL_GetKeyboardNameForID(instance_id);
}

SDL_Window* Input::get_keyboard_focus() {
  return SDL_GetKeyboardFocus();
}

// Private Event Handlers
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
  if (scancode < _key_states.size()) {
    _key_states[scancode].last_press_time = event.timestamp;
  }
}

void Input::_handle_key_up(const SDL_KeyboardEvent& event) {
  SDL_Scancode scancode = event.scancode;
  if (scancode < _key_states.size()) {
    _key_states[scancode].last_release_time = event.timestamp;
  }
}

void Input::_handle_mouse_button_down(const SDL_MouseButtonEvent& event) {
  int index = event.button - 1;
  if (index >= 0) {
    _mouse_state.button_states[index].last_press_time = event.timestamp;
  }
}

void Input::_handle_mouse_button_up(const SDL_MouseButtonEvent& event) {
  int index = event.button - 1;
  if (index >= 0 && index < _mouse_state.button_states.size()) {
    _mouse_state.button_states[index].last_release_time = event.timestamp;
  }
}

void Input::_handle_mouse_motion(const SDL_MouseMotionEvent& event) {
  _mouse_state.x = event.x;
  _mouse_state.y = event.y;
  _mouse_state.dx += event.xrel;
  _mouse_state.dy += event.yrel;
}

void Input::_handle_mouse_wheel(const SDL_MouseWheelEvent& event) {
  _mouse_state.wheel_x += event.x;
  _mouse_state.wheel_y += event.y;
}
} // namespace Zen
