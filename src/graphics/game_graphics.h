#pragma once

#include "types/color.h"
#include "types/rectangle.h"
#include "texture.h"

#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

#include <vector>
#include <memory>
#include <functional>

namespace Zen {
struct PriorityDrawable {
  PriorityDrawable(std::function<void(SDL_Renderer*, Vector2 offset)>&& draw_function, int layer, float sub_layer) {
    this->layer = layer;
    this->sub_layer = sub_layer;
    this->draw_function = std::move(draw_function);
  }
  int layer;
  float sub_layer;
  std::function<void(SDL_Renderer*, Vector2)> draw_function;
  Rectangle* clipping_rect = nullptr;
  Vector2 offset = Vector2(0, 0);
};

class GameGraphics {
public:
  GameGraphics();

  void add_drawable(PriorityDrawable&& pd);
  void draw(SDL_Renderer* renderer);
  void set_clipping(const Rectangle& clip);
  void clear_clipping();
  void add_offset(const Vector2& offset);
  void set_offset(const Vector2& offset);
  void clear_offset();

  void draw_rectangle(const Rectangle& rectangle, const Color& color, int layer = 1, float sub_layer = 1);
  void fill_rectangle(const Rectangle& rectangle, const Color& color, int layer = 1, float sub_layer = 1);

  void draw_oval(const Rectangle& oval_bounds, const Color& color, int layer = 1, float sub_layer = 1);
  void fill_oval(const Rectangle& oval_bounds, const Color& color, int layer = 1, float sub_layer = 1);

  void draw_texture(const std::shared_ptr<Texture>& texture, const Rectangle& destination, std::pair<bool, bool> flips, const Vector2& origin, double rotation_angle, const Rectangle& clipping, int layer = 1, float sub_layer = 1);
  void draw_texture(const std::shared_ptr<Texture>& texture, const Rectangle& destination, std::pair<bool, bool> flips, const Vector2& origin, double rotation_angle, int layer = 1, float sub_layer = 1);
  void draw_texture(const std::shared_ptr<Texture>& texture, const Rectangle& destination, std::pair<bool, bool> flips, int layer = 1, float sub_layer = 1);
  void draw_texture(const std::shared_ptr<Texture>& texture, const Rectangle& destination, int layer = 1, float sub_layer = 1);

  void draw_line(const Vector2& start, const Vector2& end, const Color& color, int layer = 1, float sub_layer = 1);

  void draw_text(const std::string& text, const std::string& font, float font_size, const Color& color, int max_width, Vector2 position, int layer = 1, float sub_layer = 1);

  void set_clear_before_draw(bool should_clear) { _clear_before_draw = should_clear; }
  [[nodiscard]] bool get_clear_before_draw() const { return _clear_before_draw; }

private:
  static std::unique_ptr<SDL_FRect> _to_sdl_rect(const Rectangle& rectangle);
  static std::unique_ptr<SDL_FPoint> _to_sdl_point(const Vector2& point);
  static std::unique_ptr<SDL_Color> _to_sdl_color(const Color& color);
  static SDL_FlipMode _to_sdl_render_flip(const std::pair<bool, bool>& flips);
  static void _set_color(SDL_Renderer* renderer, Color color);

  bool _clear_before_draw = true;
  std::vector<PriorityDrawable> _draw_list;
  Rectangle* _clipping_rectangle = nullptr;
  Vector2* _offset = nullptr;
};
}

