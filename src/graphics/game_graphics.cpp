#include "game_graphics.h"

#include <functional>
#include <algorithm>
#include <cmath>

namespace Zen {
struct PriorityDrawable {
  PriorityDrawable(std::function<void(SDL_Renderer*)> draw_function, int layer, float sub_layer) {
    this->layer = layer;
    this->sub_layer = sub_layer;
    draw_function = std::move(draw_function);
  }

  int& get_layer() { return layer; }
  float& get_sublayer() { return sub_layer; }
  std::function<void(SDL_Renderer*)>& get_draw_function() { return draw_function; }

private:
  int layer;
  float sub_layer;
  std::function<void(SDL_Renderer*)> draw_function;
};

GameGraphics::GameGraphics() {
  _draw_list = std::vector<PriorityDrawable*>();
}

void GameGraphics::draw(SDL_Renderer* renderer) {
  std::sort(_draw_list.begin(), _draw_list.end(), [&renderer](PriorityDrawable* a, PriorityDrawable* b) {
    if (a->get_layer() == b->get_layer()) {
      return a->get_sublayer() - b->get_sublayer();
    }
    return (float) (a->get_layer() - b->get_layer());
  });
  for (auto priority_drawable : _draw_list) {
    priority_drawable->get_draw_function()(renderer);
    delete (priority_drawable);
  }
  _draw_list.clear();
}

void GameGraphics::fill_rectangle(const Rectangle& rectangle, const Color& color, int layer, float sub_layer, bool use_camera) {
  _draw_list.emplace_back(new PriorityDrawable([=](SDL_Renderer* renderer) {
    _set_color(renderer, color);
    auto sdlRect = _to_sdl_rect(rectangle, use_camera);
    SDL_RenderFillRect(renderer, sdlRect);
    delete (sdlRect);
  }, layer, sub_layer));
}

void GameGraphics::draw_rectangle(const Rectangle& rectangle, const Color& color, int layer, float sub_layer, bool use_camera) {
  _draw_list.emplace_back(new PriorityDrawable([=](SDL_Renderer* renderer) {
    _set_color(renderer, color);
    SDL_Rect* sdlRect = _to_sdl_rect(rectangle, use_camera);
    SDL_RenderDrawRect(renderer, sdlRect);
    delete (sdlRect);
  }, layer, sub_layer));
}

void GameGraphics::draw_oval(const Rectangle& oval_bounds, const Color& color, int layer, float sub_layer, bool use_camera) {
  _draw_list.emplace_back(new PriorityDrawable([=](SDL_Renderer* renderer) {
    _set_color(renderer, color);
    auto center = oval_bounds.get_position().add(oval_bounds.get_size().scale(.5));
    if (use_camera) {
      center.add(_camera);
    }
    auto d = 2 * M_PI * sqrt((pow(oval_bounds.get_width() / 2, 2) + pow(oval_bounds.get_height() / 2, 2)) / 2);
    auto step = M_PI / 4 / d;
    for (int i = 0; i < d; i++) {
      auto s = 0;
      auto c = 1;
      if (i != 0) {
        s = sin(i * step);
        c = cos(i * step);
      }
      auto relativex = c * oval_bounds.get_width() / 2;
      auto relativey = s * oval_bounds.get_height() / 2;
      auto relativexb = s * oval_bounds.get_width() / 2;
      auto relativeyb = c * oval_bounds.get_height() / 2;
      SDL_RenderDrawPoint(renderer, int(round(center.get_x() + relativex)), int(round(center.get_y() + relativey)));
      SDL_RenderDrawPoint(renderer, int(round(center.get_x() + relativex)), int(round(center.get_y() - relativey)));
      SDL_RenderDrawPoint(renderer, int(round(center.get_x() - relativex)), int(round(center.get_y() + relativey)));
      SDL_RenderDrawPoint(renderer, int(round(center.get_x() - relativex)), int(round(center.get_y() - relativey)));
      SDL_RenderDrawPoint(renderer, int(round(center.get_x() + relativexb)), int(round(center.get_y() + relativeyb)));
      SDL_RenderDrawPoint(renderer, int(round(center.get_x() + relativexb)), int(round(center.get_y() - relativeyb)));
      SDL_RenderDrawPoint(renderer, int(round(center.get_x() - relativexb)), int(round(center.get_y() + relativeyb)));
      SDL_RenderDrawPoint(renderer, int(round(center.get_x() - relativexb)), int(round(center.get_y() - relativeyb)));
    }
  }, layer, sub_layer));
}

void GameGraphics::fill_oval(const Rectangle& oval_bounds, const Color& color, int layer, float sub_layer, bool use_camera) {
  _draw_list.emplace_back(new PriorityDrawable([=](SDL_Renderer* renderer) {
    _set_color(renderer, color);
    auto center = oval_bounds.get_position().add(oval_bounds.get_size().scale(.5));
    if (use_camera) {
      center.add(_camera);
    }
    auto d = 2 * M_PI * sqrt((pow(oval_bounds.get_width() / 2, 2) + pow(oval_bounds.get_height() / 2, 2)) / 2);
    auto step = M_PI / 4 / d;
    for (int i = 0; i < d; i++) {
      auto s = 0;
      auto c = 1;
      if (i != 0) {
        s = sin(i * step);
        c = cos(i * step);
      }
      auto relativex = c * oval_bounds.get_width() / 2;
      auto relativey = s * oval_bounds.get_height() / 2;
      auto relativexb = s * oval_bounds.get_width() / 2;
      auto relativeyb = c * oval_bounds.get_height() / 2;
      for (int j = (int) round(center.get_y() - relativey); j < (int) round(center.get_y() + relativey); j++) {
        SDL_RenderDrawPoint(renderer, int(round(center.get_x() + relativex)), j);
        SDL_RenderDrawPoint(renderer, int(round(center.get_x() - relativex)), j);
      }
      for (int j = (int) round(center.get_y() - relativeyb); j < (int) round(center.get_y() + relativeyb); j++) {
        SDL_RenderDrawPoint(renderer, int(round(center.get_x() + relativexb)), j);
        SDL_RenderDrawPoint(renderer, int(round(center.get_x() - relativexb)), j);
      }
    }
  }, layer, sub_layer));
}
void
GameGraphics::draw_texture(const std::shared_ptr<Texture>& texture, const Rectangle& destination, std::pair<bool, bool> flip, const Vector2& origin, double rotation_angle, const Rectangle& clipping,
                           int layer, float sub_layer, bool use_camera) {
  _draw_list.emplace_back(new PriorityDrawable([=](SDL_Renderer* renderer) {
    auto sdl_texture = texture->_get_sdl_texture();
    auto sdl_dest = _to_sdl_rect(destination, use_camera);
    auto sdl_clipping = _to_sdl_rect(clipping);
    auto sdl_origin = _to_sdl_point(origin);
    auto sdl_render_flip = _to_sdl_render_flip(flip);
    SDL_RenderCopyEx(renderer, sdl_texture, sdl_clipping, sdl_dest, rotation_angle, sdl_origin, sdl_render_flip);
  }, layer, sub_layer));
}

void GameGraphics::_set_color(SDL_Renderer* renderer, Color color) {
  SDL_SetRenderDrawColor(renderer, color.get_red(), color.get_green(), color.get_green(), color.get_alpha());
}

void
GameGraphics::draw_texture(const std::shared_ptr<Texture>& texture, const Rectangle& destination, std::pair<bool, bool> flips, const Vector2& origin, double rotation_angle, int layer, float sub_layer,
                           bool use_camera) {
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

SDL_Rect* GameGraphics::_to_sdl_rect(const Rectangle& rectangle, bool use_camera) {
  auto sdl_rect = new SDL_Rect;
  sdl_rect->x = rectangle.get_position().get_x_int();
  sdl_rect->y = rectangle.get_position().get_y_int();
  sdl_rect->h = rectangle.get_size().get_x_int();
  sdl_rect->w = rectangle.get_size().get_y_int();
  if (use_camera) {
    sdl_rect->x += _camera.get_x_int();
    sdl_rect->y += _camera.get_y_int();
  }
  return sdl_rect;
}

SDL_Point* GameGraphics::_to_sdl_point(const Vector2& point, bool use_camera) {
  auto sdl_point = new SDL_Point();
  sdl_point->x = point.get_x_int();
  sdl_point->y = point.get_y_int();
  if (use_camera) {
    sdl_point->x += _camera.get_x_int();
    sdl_point->y += _camera.get_y_int();
  }
  return sdl_point;
}
SDL_RendererFlip GameGraphics::_to_sdl_render_flip(const std::pair<bool, bool>& flips) {
  return SDL_RendererFlip(
          (flips.first) ? SDL_RendererFlip::SDL_FLIP_HORIZONTAL | ((flips.second) ? SDL_RendererFlip::SDL_FLIP_VERTICAL
                                                                                  : SDL_RendererFlip::SDL_FLIP_NONE)
                        : ((flips.second) ? SDL_RendererFlip::SDL_FLIP_VERTICAL
                                          : SDL_RendererFlip::SDL_FLIP_NONE));
}
}
