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
    set_color(renderer, color);
    auto sdlRect = to_sdl_rect(rectangle, use_camera);
    SDL_RenderFillRect(renderer, sdlRect);
    delete (sdlRect);
  }, layer, sub_layer));
}

void GameGraphics::draw_rectangle(const Rectangle& rectangle, const Color& color, int layer, float sub_layer, bool use_camera) {
  _draw_list.emplace_back(new PriorityDrawable([=](SDL_Renderer* renderer) {
    set_color(renderer, color);
    SDL_Rect* sdlRect = to_sdl_rect(rectangle, use_camera);
    SDL_RenderDrawRect(renderer, sdlRect);
    delete (sdlRect);
  }, layer, sub_layer));
}

void GameGraphics::set_color(SDL_Renderer* renderer, Color color) {
  SDL_SetRenderDrawColor(renderer, color.get_red(), color.get_green(), color.get_green(), color.get_alpha());
}

SDL_Rect* GameGraphics::to_sdl_rect(Rectangle rectangle, bool use_camera) {
  auto rect = new SDL_Rect;
  rect->x = rectangle.get_position().get_x_int();
  rect->y = rectangle.get_position().get_y_int();
  rect->h = rectangle.get_size().get_x_int();
  rect->w = rectangle.get_size().get_y_int();
  if (use_camera) {
    rect->x += _camera.get_x_int();
    rect->y += _camera.get_y_int();
  }
  return rect;
}

void GameGraphics::draw_oval(const Rectangle& oval_bounds, const Color& color, int layer, float sub_layer, bool use_camera) {
  _draw_list.emplace_back(new PriorityDrawable([=](SDL_Renderer* renderer) {
    set_color(renderer, color);
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
      SDL_RenderDrawPoint(renderer, round(center.get_x_int() + relativex), round(center.get_y_int() + relativey));
      SDL_RenderDrawPoint(renderer, round(center.get_x_int() + relativex), round(center.get_y_int() - relativey));
      SDL_RenderDrawPoint(renderer, round(center.get_x_int() - relativex), round(center.get_y_int() + relativey));
      SDL_RenderDrawPoint(renderer, round(center.get_x_int() - relativex), round(center.get_y_int() - relativey));
      SDL_RenderDrawPoint(renderer, round(center.get_x_int() + relativexb), round(center.get_y_int() + relativeyb));
      SDL_RenderDrawPoint(renderer, round(center.get_x_int() + relativexb), round(center.get_y_int() - relativeyb));
      SDL_RenderDrawPoint(renderer, round(center.get_x_int() - relativexb), round(center.get_y_int() + relativeyb));
      SDL_RenderDrawPoint(renderer, round(center.get_x_int() - relativexb), round(center.get_y_int() - relativeyb));
    }
  }, layer, sub_layer));
}

void GameGraphics::fill_oval(const Rectangle& oval_bounds, const Color& color, int layer, float sub_layer, bool use_camera) {
  _draw_list.emplace_back(new PriorityDrawable([=](SDL_Renderer* renderer) {
    set_color(renderer, color);
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
      for (int j = (int) round(center.get_y_int() - relativey); j < (int) round(center.get_y_int() + relativey); j++) {
        SDL_RenderDrawPoint(renderer, round(center.get_x_int() + relativex), j);
        SDL_RenderDrawPoint(renderer, round(center.get_x_int() - relativex), j);
      }
      for (int j = (int) round(center.get_y_int() - relativeyb); j < (int) round(center.get_y_int() + relativeyb); j++) {
        SDL_RenderDrawPoint(renderer, round(center.get_x_int() + relativexb), j);
        SDL_RenderDrawPoint(renderer, round(center.get_x_int() - relativexb), j);
      }
    }
  }, layer, sub_layer));
}
}
