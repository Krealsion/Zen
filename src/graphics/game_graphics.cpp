#include "game_graphics.h"

#include <SDL_ttf.h>

#include <functional>
#include <cmath>

namespace Zen {
struct PriorityDrawable {
  PriorityDrawable(std::function<void(SDL_Renderer*)>&& draw_function, int layer, float sub_layer) {
    this->layer = layer;
    this->sub_layer = sub_layer;
    this->draw_function = std::move(draw_function);
  }
  int layer;
  float sub_layer;
  std::function<void(SDL_Renderer*)> draw_function;
};

GameGraphics::GameGraphics() {
  _draw_list = std::vector<PriorityDrawable*>();
}

void GameGraphics::draw(SDL_Renderer* renderer) {
  std::sort(_draw_list.begin(), _draw_list.end(), [](PriorityDrawable* a, PriorityDrawable* b) {
    if (a->layer == b->layer) {
      return a->sub_layer - b->sub_layer;
    }
    return (float) (a->layer - b->layer);
  });
  for (auto priority_drawable : _draw_list) {
    priority_drawable->draw_function(renderer);
    delete (priority_drawable);
  }
  _draw_list.clear();
}

void GameGraphics::fill_rectangle(const Rectangle& rectangle, const Color& color, int layer, float sub_layer, bool use_camera) {
  _draw_list.emplace_back(new PriorityDrawable([=](SDL_Renderer* renderer) {
    _set_color(renderer, color);
    auto sdl_rect = _to_sdl_rect(rectangle, use_camera);
    SDL_RenderFillRect(renderer, sdl_rect.get());
  }, layer, sub_layer));
}

void GameGraphics::draw_rectangle(const Rectangle& rectangle, const Color& color, int layer, float sub_layer, bool use_camera) {
  _draw_list.emplace_back(new PriorityDrawable([=](SDL_Renderer* renderer) {
    _set_color(renderer, color);
    auto sdl_rect = _to_sdl_rect(rectangle, use_camera);
    SDL_RenderDrawRect(renderer, sdl_rect.get());
  }, layer, sub_layer));
}

void GameGraphics::draw_oval(const Rectangle& oval_bounds, const Color& color, int layer, float sub_layer, bool use_camera) {
  _draw_list.emplace_back(new PriorityDrawable([=](SDL_Renderer* renderer) {
    _set_color(renderer, color);
    auto center = oval_bounds.get_position().add(oval_bounds.get_size().scale(.5));
    if (use_camera) {
      center.add(_camera);
    }
    auto d = 2.0f * M_PI * sqrt((pow(oval_bounds.get_width() / 2.0f, 2.0f) + pow(oval_bounds.get_height() / 2.0f, 2.0f)) / 2.0f);
    auto step = M_PI / 4.0f / d;
    for (int i = 0; i < d; i++) {
      auto s = 0.0f;
      auto c = 1.0f;
      if (i != 0) {
        s = sin(i * step);
        c = cos(i * step);
      }
      auto relative_x = c * oval_bounds.get_width() / 2.0f;
      auto relative_y = s * oval_bounds.get_height() / 2.0f;
      auto relative_xb = s * oval_bounds.get_width() / 2.0f;
      auto relative_yb = c * oval_bounds.get_height() / 2.0f;
      SDL_RenderDrawPoint(renderer, int(round(center.get_x() + relative_x)), int(round(center.get_y() + relative_y)));
      SDL_RenderDrawPoint(renderer, int(round(center.get_x() + relative_x)), int(round(center.get_y() - relative_y)));
      SDL_RenderDrawPoint(renderer, int(round(center.get_x() - relative_x)), int(round(center.get_y() + relative_y)));
      SDL_RenderDrawPoint(renderer, int(round(center.get_x() - relative_x)), int(round(center.get_y() - relative_y)));
      SDL_RenderDrawPoint(renderer, int(round(center.get_x() + relative_xb)), int(round(center.get_y() + relative_yb)));
      SDL_RenderDrawPoint(renderer, int(round(center.get_x() + relative_xb)), int(round(center.get_y() - relative_yb)));
      SDL_RenderDrawPoint(renderer, int(round(center.get_x() - relative_xb)), int(round(center.get_y() + relative_yb)));
      SDL_RenderDrawPoint(renderer, int(round(center.get_x() - relative_xb)), int(round(center.get_y() - relative_yb)));
    }
  }, layer, sub_layer));
}

void GameGraphics::fill_oval(const Rectangle& oval_bounds, const Color& color, int layer, float sub_layer, bool use_camera) {
  _draw_list.emplace_back(new PriorityDrawable([=](SDL_Renderer* renderer) {
    _set_color(renderer, color);
    auto center = oval_bounds.get_position().add(oval_bounds.get_size().scale(0.5f));
    if (use_camera) {
      center.add(_camera);
    }
    auto d = 2.0f * M_PI * sqrt((pow(oval_bounds.get_width() / 2.0f, 2.0f) + pow(oval_bounds.get_height() / 2.0f, 2.0f)) / 2.0f);
    auto step = M_PI / 4.0f / d;
    for (int i = 0; i < d; i++) {
      auto s = 0.0f;
      auto c = 1.0f;
      if (i != 0) {
        s = sin(i * step);
        c = cos(i * step);
      }
      auto relative_x = c * oval_bounds.get_width() / 2;
      auto relative_y = s * oval_bounds.get_height() / 2;
      auto relative_xb = s * oval_bounds.get_width() / 2;
      auto relative_yb = c * oval_bounds.get_height() / 2;
      for (int j = (int) round(center.get_y() - relative_y); j < (int) round(center.get_y() + relative_y); j++) {
        SDL_RenderDrawPoint(renderer, int(round(center.get_x() + relative_x)), j);
        SDL_RenderDrawPoint(renderer, int(round(center.get_x() - relative_x)), j);
      }
      for (int j = (int) round(center.get_y() - relative_yb); j < (int) round(center.get_y() + relative_yb); j++) {
        SDL_RenderDrawPoint(renderer, int(round(center.get_x() + relative_xb)), j);
        SDL_RenderDrawPoint(renderer, int(round(center.get_x() - relative_xb)), j);
      }
    }
  }, layer, sub_layer));
}

void GameGraphics::draw_texture(const std::shared_ptr<Texture>& texture, const Rectangle& destination, std::pair<bool, bool> flip, const Vector2& origin, double rotation_angle, const Rectangle& clipping, int layer, float sub_layer, bool use_camera) {
  _draw_list.emplace_back(new PriorityDrawable([=](SDL_Renderer* renderer) {
    auto sdl_texture = texture->_get_sdl_texture();
    auto sdl_dest = _to_sdl_rect(destination, use_camera);
    auto sdl_clipping = _to_sdl_rect(clipping);
    auto sdl_origin = _to_sdl_point(origin);
    auto sdl_render_flip = _to_sdl_render_flip(flip);
    SDL_RenderCopyEx(renderer, sdl_texture, sdl_clipping.get(), sdl_dest.get(), rotation_angle, sdl_origin.get(), sdl_render_flip);
  }, layer, sub_layer));
}

void GameGraphics::_set_color(SDL_Renderer* renderer, Color color) {
  SDL_SetRenderDrawColor(renderer, color.get_red(), color.get_green(), color.get_green(), color.get_alpha());
}

void GameGraphics::draw_texture(const std::shared_ptr<Texture>& texture, const Rectangle& destination, std::pair<bool, bool> flips, const Vector2& origin, double rotation_angle, int layer, float sub_layer, bool use_camera) {
  auto invalid_rectangle = Rectangle();
  invalid_rectangle.invalidate();
  draw_texture(texture, destination, flips, origin, rotation_angle, invalid_rectangle, layer, sub_layer, use_camera);
}

void GameGraphics::draw_texture(const std::shared_ptr<Texture>& texture, const Rectangle& destination, std::pair<bool, bool> flips, int layer, float sub_layer, bool use_camera) {
  draw_texture(texture, destination, flips, Vector2(), 0.0f, layer, sub_layer, use_camera);
}

void GameGraphics::draw_texture(const std::shared_ptr<Texture>& texture, const Rectangle& destination, int layer, float sub_layer, bool use_camera) {
  draw_texture(texture, destination, std::make_pair<bool, bool>(false, false), layer, sub_layer, use_camera);
}

void GameGraphics::draw_line(const Zen::Vector2& start, const Zen::Vector2& end, const Zen::Color& color, int layer, float sub_layer, bool use_camera) {
  _draw_list.emplace_back(new PriorityDrawable([=](SDL_Renderer* renderer) {
    _set_color(renderer, color);
    auto sdl_start = _to_sdl_point(start, use_camera);
    auto sdl_end = _to_sdl_point(end, use_camera);
    SDL_RenderDrawLine(renderer, sdl_start->x, sdl_start->y, sdl_end->x, sdl_end->y);
  }, layer, sub_layer));
}

void GameGraphics::draw_text(const std::string& text, const std::string& font, int font_size, const Color& color, double max_width, std::function<Vector2(Vector2)>&& post_positioning_check, int layer, float sub_layer, bool use_camera) {
  _draw_list.emplace_back(new PriorityDrawable([=](SDL_Renderer* renderer) {
    _set_color(renderer, color);
    auto path = SDL_GetBasePath();
    TTF_Font* sdl_font = TTF_OpenFont(("Resources/TTFs/" + font).c_str(), font_size);
    auto error = SDL_GetError();
    auto sdl_color = _to_sdl_color(color);
    auto text_surface = TTF_RenderUTF8_Blended_Wrapped(sdl_font, text.c_str(), *sdl_color, max_width);
    auto destination_rect = post_positioning_check(Vector2(text_surface->w, text_surface->h));
    auto sdl_dest = _to_sdl_rect(Rectangle(destination_rect, Vector2(text_surface->w, text_surface->h)), use_camera);
    SDL_RenderCopy(renderer, SDL_CreateTextureFromSurface(renderer, text_surface), nullptr, sdl_dest.get());
    SDL_FreeSurface(text_surface);
  }, layer, sub_layer));

}

std::unique_ptr<SDL_Rect> GameGraphics::_to_sdl_rect(const Rectangle& rectangle, bool use_camera) {
  auto sdl_rect = std::make_unique<SDL_Rect>();
  sdl_rect->x = rectangle.get_position().get_x_int();
  sdl_rect->y = rectangle.get_position().get_y_int();
  sdl_rect->h = rectangle.get_size().get_y_int();
  sdl_rect->w = rectangle.get_size().get_x_int();
  if (use_camera) {
    sdl_rect->x += _camera.get_x_int();
    sdl_rect->y += _camera.get_y_int();
  }
  return sdl_rect;
}

std::unique_ptr<SDL_Point> GameGraphics::_to_sdl_point(const Vector2& point, bool use_camera) {
  auto sdl_point = std::make_unique<SDL_Point>();
  sdl_point->x = point.get_x_int();
  sdl_point->y = point.get_y_int();
  if (use_camera) {
    sdl_point->x += _camera.get_x_int();
    sdl_point->y += _camera.get_y_int();
  }
  return sdl_point;
}

std::unique_ptr<SDL_Color> GameGraphics::_to_sdl_color(const Color& color) {
  auto sdl_color = std::make_unique<SDL_Color>();
  sdl_color->r = color.get_red();
  sdl_color->g = color.get_green();
  sdl_color->b = color.get_blue();
  sdl_color->a = color.get_alpha();
  return sdl_color;
}

SDL_RendererFlip GameGraphics::_to_sdl_render_flip(const std::pair<bool, bool>& flips) {
  return SDL_RendererFlip(
          (flips.first) ? SDL_RendererFlip::SDL_FLIP_HORIZONTAL | ((flips.second) ? SDL_RendererFlip::SDL_FLIP_VERTICAL
                                                                                  : SDL_RendererFlip::SDL_FLIP_NONE)
                        : ((flips.second) ? SDL_RendererFlip::SDL_FLIP_VERTICAL
                                          : SDL_RendererFlip::SDL_FLIP_NONE));
}
}
