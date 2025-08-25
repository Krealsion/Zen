#include "button.h"

#include "input.h"
#include "text.h"

namespace Zen {

Button::Button() {
  // TODO default border color config
  _border_color = Color(0, 0, 0); // Default border color
  _default_background_color = Color(220, 220, 220);

  // TODO: Make this configurable
  _text = new Text();
  _text->set_name("Button Text");
  _text->set_size(SizeTo::PARENT, SizeTo::CHILDREN);
  _text->center();
  _text->set_font("Basic-Regular.ttf");
  _text->set_font_size(16.0f);
  _text->set_font_color(Color(0, 0, 0));

  _inner = new CustomLayout();
  _inner->set_name("Inner Button area");
  _inner->set_size(SizeTo::PARENT, SizeTo::CHILDREN);
  _inner->set_background_color(_default_background_color);
  _inner->set_margin(2, 2, 2, 2);
  _inner->add_child(_text);

  this->CustomLayout::set_background_color(_border_color);
  this->set_padding(2, 2, 2, 2);
  this->add_child(_inner);
}

void Button::set_text(const std::string& text) {
  // Get Text size, compare it to set size (if any default to child) enable multi-line if needed default centered
  _text->set_text(text);
  if (get_name().empty()) {
    _text->set_name("button_text:" + text);
  }
}
const std::string& Button::get_text() const {
  return _text->get_text();
}

void Button::update() {
  if (get_background_destination().contains(Input::get_mouse_position())) {
    _mouse_hovered = true;
  } else {
    _mouse_hovered = false;
  }
  CustomLayout::update();
}
void Button::set_background_color(Color color) {
  _default_background_color = color;
  if (_inner) {
    _inner->set_background_color(_default_background_color);
  }
}

void Button::set_hovered_bg_color(const Color& color) {
  _hovered_bg_color = color;
  _auto_hover_color = false;
}

void Button::set_auto_hover_color(bool auto_hover_color) {
  _auto_hover_color = auto_hover_color;
}

void Button::draw(GameGraphics& game_graphics) {
  if (_mouse_hovered) {
    auto color = _default_background_color;
    if (_auto_hover_color) {
      Color auto_color = Color(color.get_red() + (255 - color.get_red()) / 2,
        color.get_green() + (255 - color.get_green()) / 2,
        color.get_blue() + (255 - color.get_blue()) / 2);
      set_background_color(auto_color);
    } else {
      set_background_color(_hovered_bg_color);
    }
    CustomLayout::draw(game_graphics);
    set_background_color(color);
  } else {
    set_background_color(_default_background_color);
    CustomLayout::draw(game_graphics);
  }
}
}
