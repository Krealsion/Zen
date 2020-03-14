#pragma once

#include "types/color.h"
#include "types/rectangle.h"

#include <vector>
#include <SDL_rect.h>
#include <SDL_render.h>

namespace Zen {
struct PriorityDrawable;

class GameGraphics {
public:
  GameGraphics();

  void draw(SDL_Renderer* renderer);

  void draw_rectangle(const Rectangle& rectangle, const Color& color, int layer = 1, float sub_layer = 1, bool use_camera = true);
  void fill_rectangle(const Rectangle& rectangle, const Color& color, int layer = 1, float sub_layer = 1, bool use_camera = true);

  void draw_oval(const Rectangle& oval_bounds, const Color& color, int layer = 1, float sub_layer = 1, bool use_camera = true);
  void fill_oval(const Rectangle& oval_bounds, const Color& color, int layer = 1, float sub_layer = 1, bool use_camera = true);

  void set_clear_before_draw(bool should_clear) { _clear_before_draw = should_clear; }
  bool get_clear_before_draw() { return _clear_before_draw; }

private:
  SDL_Rect* to_sdl_rect(Rectangle rectangle, bool use_camera = false);
  static void set_color(SDL_Renderer* renderer, Color color);

  bool _clear_before_draw = true;
  Vector2 _camera;
  std::vector<PriorityDrawable*> _draw_list;
};
}

