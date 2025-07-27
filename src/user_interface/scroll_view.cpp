#include "scroll_view.h"

#include "game_graphics.h"
#include "input.h"

#include <SDL3/SDL_render.h>

// TODO HORIZONTAL YOU DUMBASS

namespace Zen {
void ScrollView::update() {
  _manage_input();
  CustomLayout::update();
}
void ScrollView::_manage_input() {
  if (!get_background_destination().contains(Input::get_mouse_position())) {
    return;
  }
  if (_layout == Layout::VERTICAL) {
    auto total_size = _get_children_total_size();
    double scroll_height = get_height();
    if (scroll_height > total_size.get_y()) {
      _scroll_offset = 0.0; // No need to scroll if the scroll view is larger than the content
      return;
    }
    double overflow = total_size.get_y() - scroll_height;
    if (overflow <= 0) {
      _scroll_offset = 0.0; // No overflow, no scrolling needed
      return;
    }
    // TODO add support for mouse wheel scrolling

    // Handle scrolling input
    if (Input::is_key_down(SDL_SCANCODE_UP) || Input::is_key_down(SDL_SCANCODE_W)) {
      _scroll_offset -= 10; // Scroll up
    } else if (Input::is_key_down(SDL_SCANCODE_DOWN) || Input::is_key_down(SDL_SCANCODE_S)) {
      _scroll_offset += 10; // Scroll down
    }

    // Clamp the scroll offset
    if (_scroll_offset < 0) {
      _scroll_offset = 0;
    } else if (_scroll_offset > overflow) {
      _scroll_offset = overflow;
    }
  }
}

void ScrollView::draw(GameGraphics& game_graphics) {
  game_graphics.set_clipping(get_background_destination());
  game_graphics.add_offset(Vector2(0, -_scroll_offset));
  CustomLayout::draw(game_graphics);
  game_graphics.clear_clipping();
  game_graphics.clear_offset();
}

void ScrollView::scroll_to_percent(double percent) {
  if (percent < 0.0) {
    percent = 0.0;
  } else if (percent > 1.0) {
    percent = 1.0;
  }
  auto total_size = _get_children_total_size();
  if (_layout == Layout::VERTICAL) {
    double scroll_height = get_height();
    if (scroll_height > total_size.get_y()) {
      return; // No need to scroll if the scroll view is larger than the content
    }
    double overflow = total_size.get_y() - scroll_height;
    _scroll_offset = (overflow * percent);
  }
  // TODO horizontal
}

Vector2 ScrollView::_get_children_total_size() {
  Vector2 total_size(0, 0);
  for (const auto& child : _children) {
    if (child->is_visible()) {
      Vector2 child_size = child->get_size();
      total_size.add_x(child_size.get_x());
      total_size.add_y(child_size.get_y());
    }
  }
  return total_size;
}
}