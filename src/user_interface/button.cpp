#include "button.h"

#include "input.h"
#include "text.h"

namespace Zen {

Button::Button() {
  _text = new Text();
  _text->center();
}

void Button::set_text(const std::string& text) {
  // Get Text size, compare it to set size (if any default to child) enable multi-line if needed default centered
  _text->set_text(text);
  if (get_name().empty()) {
    _text->set_name(text);
  }
}

void Button::update() {
  auto mouse_pos = Input::get_mouse_position();
  Rectangle rec = Rectangle(get_position(), get_size());
  if (rec.contains(mouse_pos)) {
    _mouse_hovered = true;
  } else {
    _mouse_hovered = false;
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
    auto color = get_background_color();
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
    CustomLayout::draw(game_graphics);
  }
}
}
