#pragma once

#include "types/color.h"
#include "types/rectangle.h"
#include "texture.h"

#include <SDL_rect.h>
#include <SDL_render.h>

#include <vector>
#include <memory>
#include <functional>

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

  void draw_texture(const std::shared_ptr<Texture>& texture, const Rectangle& destination, std::pair<bool, bool> flips, const Vector2& origin, double rotation_angle, const Rectangle& clipping, int layer = 1, float sub_layer = 1, bool use_camera = true);
  void draw_texture(const std::shared_ptr<Texture>& texture, const Rectangle& destination, std::pair<bool, bool> flips, const Vector2& origin, double rotation_angle, int layer = 1, float sub_layer = 1, bool use_camera = true);
  void draw_texture(const std::shared_ptr<Texture>& texture, const Rectangle& destination, std::pair<bool, bool> flips, int layer = 1, float sub_layer = 1, bool use_camera = true);
  void draw_texture(const std::shared_ptr<Texture>& texture, const Rectangle& destination, int layer = 1, float sub_layer = 1, bool use_camera = true);

  void draw_line(const Vector2& start, const Vector2& end, const Color& color, int layer = 1, float sub_layer = 1, bool use_camera = true);

  void draw_text(const std::string& text, const std::string& font, int font_size, const Color& color, double max_width, std::function<Vector2(Vector2)>&& post_positioning_check, int layer = 1, float sub_layer = 1, bool use_camera = true);

  void set_clear_before_draw(bool should_clear) { _clear_before_draw = should_clear; }
  [[nodiscard]] bool get_clear_before_draw() const { return _clear_before_draw; }

private:
  std::unique_ptr<SDL_Rect> _to_sdl_rect(const Rectangle& rectangle, bool use_camera = false);
  std::unique_ptr<SDL_Point> _to_sdl_point(const Vector2& point, bool use_camera = false);
  std::unique_ptr<SDL_Color> _to_sdl_color(const Color& color);
  static SDL_RendererFlip _to_sdl_render_flip(const std::pair<bool, bool>& flips);
  static void _set_color(SDL_Renderer* renderer, Color color);

  bool _clear_before_draw = true;
  Vector2 _camera;
  std::vector<PriorityDrawable*> _draw_list;
};
}

