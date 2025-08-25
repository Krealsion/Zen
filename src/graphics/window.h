#pragma once

#include "types/rectangle.h"

#include <string>

#include <SDL3/SDL_video.h>

namespace Zen {
class Window {
public:
  Window(const std::string& name, Rectangle window_rectangle);
  ~Window();

  SDL_Window* get_window();

  void set_x(int new_x);
  void set_y(int new_y);
  int get_x();
  int get_y();

  void set_width(int new_width);
  void set_height(int new_height);
  int get_width();
  int get_height();

private:
  SDL_Window* sdl_window;
};
}
