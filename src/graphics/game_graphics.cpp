#include "game_graphics.h"

#include "logic/math.h"
#include "logger.h"

#include <SDL3_ttf/SDL_ttf.h>

#include <functional>
#include <cmath>
#include <cstring>

namespace Zen {

GameGraphics::GameGraphics() {
  _draw_list = std::vector<PriorityDrawable>();
}
void GameGraphics::add_drawable(PriorityDrawable&& pd) {
  if (this->_clipping_rectangle) {
    pd.clipping_rect = new Rectangle(*_clipping_rectangle);
  }
  if (this->_offset) {
    pd.offset = *this->_offset;
  }
  // TODO scale
  _draw_list.emplace_back(std::move(pd));
}

void GameGraphics::draw(SDL_Renderer* renderer) {
  std::stable_sort(_draw_list.begin(), _draw_list.end(), [](const PriorityDrawable& a, const PriorityDrawable& b) {
    if (a.layer != b.layer) return a.layer < b.layer;
    return a.sub_layer < b.sub_layer;
  });
  for (const auto& priority_drawable : _draw_list) {
    // TODO truncating clipping to int, maybe rounding instead?
    auto sdl_rect = std::make_unique<SDL_Rect>();
    if (priority_drawable.clipping_rect) {
      sdl_rect->x = (int) priority_drawable.clipping_rect->get_x();
      sdl_rect->y = (int) priority_drawable.clipping_rect->get_y();
      sdl_rect->w = (int) priority_drawable.clipping_rect->get_width();
      sdl_rect->h = (int) priority_drawable.clipping_rect->get_height();
      SDL_SetRenderClipRect(renderer, sdl_rect.get());
    } else {
      SDL_SetRenderClipRect(renderer, nullptr);
    }
    if (priority_drawable.offset.get_x() != 0 || priority_drawable.offset.get_y() != 0) {

    }
    priority_drawable.draw_function(renderer, priority_drawable.offset);
  }
  _draw_list.clear();
}
void GameGraphics::set_clipping(const Rectangle& clip) {
  if (this->_clipping_rectangle) {
    *_clipping_rectangle = clip;
  } else {
    this->_clipping_rectangle = new Rectangle(clip);
  }
}

void GameGraphics::clear_clipping() {
  delete this->_clipping_rectangle;
  this->_clipping_rectangle = nullptr;
}

void GameGraphics::add_offset(const Vector2& offset) {
  if (this->_offset) {
    this->_offset->add(offset);
  } else {
    this->_offset = new Vector2(offset);
  }
}
void GameGraphics::set_offset(const Vector2& offset) {
  if (this->_offset) {
    *this->_offset = offset;
  } else {
    this->_offset = new Vector2(offset);
  }
}
void GameGraphics::clear_offset() {
  delete this->_offset;
  this->_offset = nullptr;
}

void GameGraphics::fill_rectangle(const Rectangle& rectangle, const Color& color, int layer, float sub_layer) {
  add_drawable(std::move(PriorityDrawable([=, this](SDL_Renderer* renderer, Vector2 offset) {
    _set_color(renderer, color);
    auto sdl_rect = _to_sdl_rect(Rectangle(rectangle.get_position().add(offset), rectangle.get_size()));
    SDL_RenderFillRect(renderer, sdl_rect.get());
  }, layer, sub_layer)));
}

void GameGraphics::draw_rectangle(const Rectangle& rectangle, const Color& color, int layer, float sub_layer) {
  add_drawable(std::move(PriorityDrawable([=, this](SDL_Renderer* renderer, Vector2 offset) {
    _set_color(renderer, color);
    auto sdl_rect = _to_sdl_rect(Rectangle(rectangle.get_position().add(offset), rectangle.get_size()));
    SDL_RenderRect(renderer, sdl_rect.get());
  }, layer, sub_layer)));
}

void GameGraphics::draw_oval(const Rectangle& oval_bounds, const Color& color, int layer, float sub_layer) {
  add_drawable(std::move(PriorityDrawable([=, this](SDL_Renderer* renderer, Vector2 offset) {
    _set_color(renderer, color);
    auto center = oval_bounds.get_position().add(offset).add(oval_bounds.get_size().scale(.5));
    auto d = 2.0f * Math::PI * sqrt((pow(oval_bounds.get_width() / 2.0f, 2.0f) + pow(oval_bounds.get_height() / 2.0f, 2.0f)) / 2.0f);
    auto step = Math::PI / 4.0f / d;
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
      SDL_RenderPoint(renderer, center.get_x() + relative_x, center.get_y() + relative_y);
      SDL_RenderPoint(renderer, center.get_x() + relative_x, center.get_y() - relative_y);
      SDL_RenderPoint(renderer, center.get_x() - relative_x, center.get_y() + relative_y);
      SDL_RenderPoint(renderer, center.get_x() - relative_x, center.get_y() - relative_y);
      SDL_RenderPoint(renderer, center.get_x() + relative_xb, center.get_y() + relative_yb);
      SDL_RenderPoint(renderer, center.get_x() + relative_xb, center.get_y() - relative_yb);
      SDL_RenderPoint(renderer, center.get_x() - relative_xb, center.get_y() + relative_yb);
      SDL_RenderPoint(renderer, center.get_x() - relative_xb, center.get_y() - relative_yb);
    }
  }, layer, sub_layer)));
}

void GameGraphics::fill_oval(const Rectangle& oval_bounds, const Color& color, int layer, float sub_layer) {
  add_drawable(std::move(PriorityDrawable([=, this](SDL_Renderer* renderer, Vector2 offset) {
    _set_color(renderer, color);
    auto center = oval_bounds.get_position().add(offset).add(oval_bounds.get_size().scale(0.5f));
    auto d = 2.0f * Math::PI * sqrt((pow(oval_bounds.get_width() / 2.0f, 2.0f) + pow(oval_bounds.get_height() / 2.0f, 2.0f)) / 2.0f);
    auto step = Math::PI / 4.0f / d;
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
        SDL_RenderPoint(renderer, center.get_x() + relative_x, j);
        SDL_RenderPoint(renderer, center.get_x() - relative_x, j);
      }
      for (int j = (int) round(center.get_y() - relative_yb); j < (int) round(center.get_y() + relative_yb); j++) {
        SDL_RenderPoint(renderer, center.get_x() + relative_xb, j);
        SDL_RenderPoint(renderer, center.get_x() - relative_xb, j);
      }
    }
  }, layer, sub_layer)));
}

void GameGraphics::draw_texture(const std::shared_ptr<Texture>& texture, const Rectangle& destination, std::pair<bool, bool> flip, const Vector2& origin, double rotation_angle, const Rectangle& clipping, int layer, float sub_layer) {
  add_drawable(std::move(PriorityDrawable([=, this](SDL_Renderer* renderer, Vector2 offset) {
    auto sdl_texture = texture->_get_sdl_texture();
    auto sdl_dest = _to_sdl_rect(Rectangle(destination.get_position().add(offset), destination.get_size()));
    auto sdl_clipping = _to_sdl_rect(clipping);
    auto sdl_origin = _to_sdl_point(origin);
    auto sdl_render_flip = _to_sdl_render_flip(flip);
    SDL_RenderTextureRotated(renderer, sdl_texture, sdl_clipping.get(), sdl_dest.get(), rotation_angle, sdl_origin.get(), sdl_render_flip);
  }, layer, sub_layer)));
}

void GameGraphics::_set_color(SDL_Renderer* renderer, Color color) {
  SDL_SetRenderDrawColor(renderer, color.get_red(), color.get_green(), color.get_blue(), color.get_alpha());
}

void GameGraphics::draw_texture(const std::shared_ptr<Texture>& texture, const Rectangle& destination, std::pair<bool, bool> flips, const Vector2& origin, double rotation_angle, int layer, float sub_layer) {
  draw_texture(texture, destination, flips, origin, rotation_angle, Rectangle(), layer, sub_layer);
}

void GameGraphics::draw_texture(const std::shared_ptr<Texture>& texture, const Rectangle& destination, std::pair<bool, bool> flips, int layer, float sub_layer) {
  draw_texture(texture, destination, flips, Vector2(), 0.0f, layer, sub_layer);
}

void GameGraphics::draw_texture(const std::shared_ptr<Texture>& texture, const Rectangle& destination, int layer, float sub_layer) {
  draw_texture(texture, destination, std::make_pair<bool, bool>(false, false), layer, sub_layer);
}

void GameGraphics::draw_line(const Zen::Vector2& start, const Zen::Vector2& end, const Zen::Color& color, int layer, float sub_layer) {
  add_drawable(std::move(PriorityDrawable([=, this](SDL_Renderer* renderer, Vector2 offset) {
    _set_color(renderer, color);
    auto sdl_start = _to_sdl_point(offset.add(start));
    auto sdl_end = _to_sdl_point(offset.add(end));
    SDL_RenderLine(renderer, sdl_start->x, sdl_start->y, sdl_end->x, sdl_end->y);
  }, layer, sub_layer)));
}

void GameGraphics::draw_text(const std::string& text, const std::string& font, float font_size, const Color& color, int max_width, Vector2 position, int layer, float sub_layer) {
  if (text.empty()) {
    Logger::log(LogLevel::WARNING, "Attempted to draw empty text. Skipping.");
    return;
  }
  add_drawable(std::move(PriorityDrawable([=](SDL_Renderer* renderer, Vector2 offset) {
    _set_color(renderer, color);
    auto path = SDL_GetBasePath();
    TTF_Font* sdl_font = TTF_OpenFont(("../Resources/TTFs/" + font).c_str(), font_size); // TODO: Better path resolving
    auto sdl_color = _to_sdl_color(color);
    auto text_surface = TTF_RenderText_Blended_Wrapped(sdl_font, text.c_str(), std::strlen(text.c_str()), *sdl_color, max_width);
    if (text_surface == nullptr) {
      std::string error_message = "Failed to render text: " + std::string(SDL_GetError());
      Logger::log(LogLevel::ERROR, error_message);
      throw std::runtime_error(error_message);
    }
    auto sdl_dest = _to_sdl_rect(Rectangle(offset.add(position), Vector2(text_surface->w, text_surface->h)));
    SDL_RenderTexture(renderer, SDL_CreateTextureFromSurface(renderer, text_surface), nullptr, sdl_dest.get());
    SDL_DestroySurface(text_surface);
  }, layer, sub_layer)));

}

std::unique_ptr<SDL_FRect> GameGraphics::_to_sdl_rect(const Rectangle& rectangle) {
  auto sdl_rect = std::make_unique<SDL_FRect>();
  sdl_rect->x = rectangle.get_position().get_x();
  sdl_rect->y = rectangle.get_position().get_y();
  sdl_rect->h = rectangle.get_size().get_y();
  sdl_rect->w = rectangle.get_size().get_x();
  return sdl_rect;
}

std::unique_ptr<SDL_FPoint> GameGraphics::_to_sdl_point(const Vector2& point) {
  auto sdl_point = std::make_unique<SDL_FPoint>();
  sdl_point->x = point.get_x();
  sdl_point->y = point.get_y();
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

SDL_FlipMode GameGraphics::_to_sdl_render_flip(const std::pair<bool, bool>& flips) {
  return SDL_FlipMode(
    flips.first ? SDL_FLIP_HORIZONTAL | (
      flips.second ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE) : (
        flips.second ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE));
}
}
