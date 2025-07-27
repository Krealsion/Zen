#include "text.h"

#include "game_graphics.h"
#include "texture_manager.h"

namespace Zen {

Text::Text() : CustomLayout() {}

Text::Text(Config& config) : CustomLayout() {
  this->set_font(config.font_name);
  this->set_font_size(config.font_size);
  this->set_font_color(config.font_color);
}

void Text::set_height(SizeTo size_to, double value) {
  throw std::runtime_error("Can't set height on Text component");
}

void Text::set_width(SizeTo size_to, double value) {
  throw std::runtime_error("Can't set width on Text component, please use set_text_wrap_width to set maximum text width");
}

void Text::set_text_wrap_width(int value) {
  _max_width = value;
}

void Text::set_wrap(bool should_wrap) {
  _wrap = should_wrap;
  if (!_wrap) {
    _max_width = 0;
  }
}

void Text::set_text(const std::string& text) {
  _text = text;
  _update_text();
}

void Text::set_font(const std::string& font) {
  _font = font;
  _update_text();
}

void Text::set_font_size(float font_size) {
  _font_size = font_size;
  _update_text();
}

void Text::set_font_color(Color font_color) {
  _font_color = font_color;
  _update_text();
}

void Text::draw(GameGraphics& game_graphics) {
  CustomLayout::draw(game_graphics);
  game_graphics.draw_text(_text, _font, _font_size, _font_color, get_width(), get_position());
}

void Text::_update_text() {
  Vector2 text_size = TextureManager::get_text_size(_text, _font, _font_size, _max_width);
  CustomLayout::set_width(SizeTo::STATIC, text_size.get_x() + _padding_value_right + _padding_value_left);
  CustomLayout::set_height(SizeTo::STATIC, text_size.get_y() + _padding_value_top + _padding_value_bottom);
}
} // namespace Zen