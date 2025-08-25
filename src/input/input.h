#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>
#include <queue>
#include <mutex>
#include <variant>
#include <unordered_set>
#include <algorithm>

#include <SDL3/SDL.h>

#include "callback.h"
#include "vector2.h"

namespace Zen {

class Window;

enum class TriggerType {
  PRESSED,       // On key press
  RELEASED,      // On key release
  TAPPED,        // On key press and release (fast)
  HELD,          // On key press and hold
  DOUBLE_TAPPED  // On key press and release (fast) twice
};

struct KeyCombo {
  SDL_Scancode key = SDL_SCANCODE_UNKNOWN;
  std::vector<SDL_Scancode> modifiers;
  bool operator==(const KeyCombo& other) const {
    if (key != other.key || modifiers.size() != other.modifiers.size()) return false;
    for (size_t i = 0; i < modifiers.size(); ++i) {
      if (modifiers[i] != other.modifiers[i]) return false;
    }
    return true;
  }
};

struct InputState {
  Uint64 last_press_time = 0;
  Uint64 last_release_time = 0;
  Uint64 last_tap_start_time = 0;  // For double tap detection
  bool was_pressed_since_last_check = false;  // For quick tap prevention
};

struct TriggerEvent {
  TriggerType type;
  std::variant<KeyCombo, int> source; // KeyCombo for keyboard, int for mouse button
  int duration_ms;
};

struct MouseState {
  std::vector<InputState> button_states;
  float x = 0.0f, y = 0.0f;
  float dx = 0.0f, dy = 0.0f;
  float wheel_x = 0.0f, wheel_y = 0.0f;
};

enum MouseButton {
  LEFT = SDL_BUTTON_LEFT,
  MIDDLE = SDL_BUTTON_MIDDLE,
  RIGHT = SDL_BUTTON_RIGHT,
  X1 = SDL_BUTTON_X1,
  X2 = SDL_BUTTON_X2
};

class Input {
public:
  static void init();

  static void set_active_window(Window* window);
  static void update_input();
  static void clean_input();
  static void process_input_callbacks();  // Call this in the user's update loop to trigger queued callbacks

  // Callback Registration
  static void register_on_input_callback(TriggerType type, const KeyCombo& combo, std::function<void(int)> callback, int duration_ms = 0);
  static void register_on_mouse_callback(TriggerType type, MouseButton button, std::function<void(int)> callback, int duration_ms = 0);
  static void set_default_input_durations(int tap_window_ms, int double_tap_duration_ms);

  // Text Input
  static void start_text_input();
  static void start_text_input(Action<const std::string&> callback);
  static std::string get_text_input();
  static void end_text_input();

  // Keyboard Functions
  static bool is_key_down(SDL_Scancode key);
  static bool is_key_down(const KeyCombo& combo);
  static int get_key_pressed_duration(SDL_Scancode key);
  static int get_key_pressed_duration(const KeyCombo& combo);
  static int get_key_released_duration(SDL_Scancode key);
  static int get_key_released_duration(const KeyCombo& combo);
  static void reset_keyboard();

  // Quick Tap Prevention Configuration
  static void set_quick_tap_prevention(bool enabled, int buffer_duration_ms = 100);

  // Custom Modifiers
  static void add_custom_modifier(SDL_Scancode key);
  static void remove_custom_modifier(SDL_Scancode key);

  // Key Combo Listening
  static KeyCombo listen_for_key_combo();

  // Mouse Functions
  static bool is_mouse_button_down(MouseButton button);
  static Vector2 get_mouse_position();
  static Vector2 get_mouse_delta();
  static Vector2 get_mouse_wheel();
  static void warp_mouse(float x, float y);
  static bool warp_mouse_absolute(float x, float y);
  static void warp_mouse_to_window(Window* window, float x, float y);

  // Mouse Management
  static bool show_cursor();
  static bool hide_cursor();
  static bool is_cursor_visible();
  static bool set_relative_mouse_mode(bool enabled);
  static bool get_relative_mouse_mode();

  // Keyboard Management
  static SDL_KeyboardID* get_keyboards(int* count);
  static const char* get_keyboard_name(SDL_KeyboardID instance_id);
  static SDL_Window* get_keyboard_focus();

private:
  static Window* _window;
  static std::unordered_map<SDL_Scancode, InputState> _key_states;
  static std::vector<KeyCombo> _combo_list;
  static std::vector<InputState> _combo_states;
  static std::unordered_multimap<SDL_Scancode, size_t> _combos_by_scancode;
  static MouseState _mouse_state;

  static std::string _text_input;
  static bool _text_input_enabled;
  static Action<const std::string&> _update_text_input_callback;

  static std::vector<std::tuple<TriggerType, KeyCombo, std::function<void(int)>, int>> _key_callbacks;
  static std::vector<std::tuple<TriggerType, MouseButton, std::function<void(int)>, int>> _mouse_callbacks;

  static std::queue<TriggerEvent> _pending_triggers;

  static int _default_tap_window_ms;
  static int _default_double_tap_duration_ms;

  static bool _quick_tap_prevention_enabled;
  static int _quick_tap_buffer_ms;

  static std::unordered_map<SDL_Scancode, int> _scancode_to_group;
  static std::unordered_map<int, SDL_Scancode> _group_to_canonical;
  static int _next_modifier_group;

  static std::vector<SDL_Scancode> _pressed_keys;

  static std::mutex _input_mutex;

  static void _handle_event(const SDL_Event& event);
  static void _handle_key_down(const SDL_KeyboardEvent& event);
  static void _handle_key_up(const SDL_KeyboardEvent& event);
  static void _handle_mouse_button_down(const SDL_MouseButtonEvent& event);
  static void _handle_mouse_button_up(const SDL_MouseButtonEvent& event);
  static void _handle_mouse_motion(const SDL_MouseMotionEvent& event);
  static void _handle_mouse_wheel(const SDL_MouseWheelEvent& event);

  static void _queue_trigger(const KeyCombo& combo, TriggerType type, int duration_ms = 0);
  static void _queue_trigger(MouseButton button, TriggerType type, int duration_ms = 0);

  static InputState& _get_or_create_combo_state(const KeyCombo& combo);

  static bool _combo_matches(const KeyCombo& combo, const std::vector<SDL_Scancode>& pressed);
};
}  // namespace Zen
