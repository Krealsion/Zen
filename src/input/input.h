#pragma once

#include <functional>
#include <SDL3/SDL.h>

#include <string>
#include <unordered_map>
#include <vector>

#include "vector2.h"

namespace Zen {

class Window;

struct KeyCombo {
  SDL_Scancode key;
  std::vector<SDL_Scancode> modifiers;
};

struct InputState {
  KeyCombo key;
  Uint64 last_press_time = 0;
  Uint64 last_release_time = 0;
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
  Input();
  ~Input();

  static void set_active_window(Window* window);
  static void update_input();
  static void clean();

  static void start_text_input();
  template <typename Function>
  static void start_text_input(Function callback);
  static std::string get_text_input();
  static void end_text_input();

  // Keyboard Functions
  static bool is_key_down(SDL_Scancode key);
  static bool is_key_down(const KeyCombo& combo);
  static int get_key_pressed_duration(SDL_Scancode key);
  static int get_key_released_duration(SDL_Scancode key);
  static void reset_keyboard();

  // Mouse Functions
  static bool is_mouse_button_down(int button);
  static Vector2 get_mouse_position();
  static Vector2 get_mouse_delta();
  static void get_mouse_wheel(float& wheel_x, float& wheel_y);
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
  static std::vector<InputState> _key_states;
  static MouseState _mouse_state;

  static std::string _text_input;
  static bool _text_input_enabled;
  static std::function<void(std::string)> _update_text_input_callback;

  static void _handle_event(const SDL_Event& event);
  static void _handle_key_down(const SDL_KeyboardEvent& event);
  static void _handle_key_up(const SDL_KeyboardEvent& event);
  static void _handle_mouse_button_down(const SDL_MouseButtonEvent& event);
  static void _handle_mouse_button_up(const SDL_MouseButtonEvent& event);
  static void _handle_mouse_motion(const SDL_MouseMotionEvent& event);
  static void _handle_mouse_wheel(const SDL_MouseWheelEvent& event);
};

} // namespace Zen
