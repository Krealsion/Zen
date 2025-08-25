#include "text.h"

#include "game_graphics.h"
#include "texture_manager.h"


namespace Zen {

void BaseText::draw(GameGraphics& game_graphics) {
  CustomLayout::draw(game_graphics);
  if (_wrap) {
    if (get_width() < _max_width) {
      game_graphics.draw_text(_text, _font, _font_size, _font_color, get_size().get_x_int(), get_position());
    } else {
      game_graphics.draw_text(_text, _font, _font_size, _font_color, _max_width, get_position());
    }
  } else {
    game_graphics.draw_text(_text, _font, _font_size, _font_color, 0, get_position());
  }
}

void BaseText::_update_text() {
  if (_font != "" && _font_size > 0) {
    Vector2 text_size = TextureManager::get_text_size(_text, _font, _font_size, _max_width);
    CustomLayout::set_width(SizeTo::STATIC, text_size.get_x() + _padding_value_right + _padding_value_left);
    CustomLayout::set_height(SizeTo::STATIC, text_size.get_y() + _padding_value_top + _padding_value_bottom);
  }
}

Text::Text() : CustomLayout() {
  _create_base_text();
}

Text::Text(Config& config) : CustomLayout() {
  _create_base_text();
  this->set_font(config.font_name);
  this->set_font_size(config.font_size);
  this->set_font_color(config.font_color);
}

void Text::_create_base_text() {
  _base_text = new BaseText();
  set_font("Basic-Regular.ttf");
  set_font_size(16.0f);
  set_font_color(Color(0, 0, 0));
  _base_text->set_position(Zen::PositionTo::CENTER, Zen::PositionTo::CENTER);
  add_child(_base_text);
}

void Text::set_wrap(bool should_wrap) {
  _base_text->_wrap = should_wrap;
  if (!_base_text->_wrap) {
    _max_width = 0;
  }
}

void Text::set_text(const std::string& text) {
  _base_text->_text = text;
  _base_text->_update_text();
}

void Text::set_font(const std::string& font) {
  _base_text->_font = font;
  _base_text->_update_text();
}

void Text::set_font_size(float font_size) {
  _base_text->_font_size = font_size;
  _base_text->_update_text();
}

void Text::set_font_color(Color font_color) {
  _base_text->_font_color = font_color;
}
const std::string& Text::get_text() const {
  return _base_text->_text;
}
const std::string& Text::get_font() const {
  return _base_text->_font;
}
float Text::get_font_size() const {
  return _base_text->_font_size;
}
Color Text::get_font_color() const {
  return _base_text->_font_color;
}
bool Text::get_wrap() const {
  return _base_text->_wrap;
}
} // namespace Zen