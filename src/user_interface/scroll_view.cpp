#include "scroll_view.h"

#include "game_graphics.h"
#include "input.h"

#include <SDL3/SDL_render.h>

// TODO HORIZONTAL YOU DUMBASS

namespace Zen {
void ScrollView::add_child(CustomLayout* child) {
  if (_child_container == nullptr) {
    _child_container = new CustomLayout();
    _child_container->set_name("ScrollViewChildContainer");
    _child_container->set_size(SizeTo::PARENT, 0, SizeTo::CHILDREN, 0); // TODO control this based on vertical horizontal
    CustomLayout::add_child(_child_container);
  }
  _child_container->add_child(child);
}

void ScrollView::update() {
  _manage_input();
  if (_child_container) {
    _child_container->set_position_y(PositionTo::RELATIVE, _scroll_offset);
  }
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

    // Handle mouse wheel scrolling
    Vector2 wheel = Input::get_mouse_wheel();
    if (wheel.get_y() != 0) {
      _scroll_offset += wheel.get_y() * 50; // Adjust scroll speed as needed
    }

    // Handle scrolling input
    if (Input::is_key_down(SDL_SCANCODE_UP) || Input::is_key_down(SDL_SCANCODE_W)) {
      _scroll_offset -= 10; // Scroll up
    } else if (Input::is_key_down(SDL_SCANCODE_DOWN) || Input::is_key_down(SDL_SCANCODE_S)) {
      _scroll_offset += 10; // Scroll down
    }

    // Clamp the scroll offset
    if (_scroll_offset > 0) {
      _scroll_offset = 0;
    }
    else if (_scroll_offset < - overflow) {
      _scroll_offset = - overflow;
    }
  }
}

void ScrollView::draw(GameGraphics& game_graphics) {
  game_graphics.set_clipping(get_background_destination());
  CustomLayout::draw(game_graphics);
  game_graphics.clear_clipping();
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
  if (_child_container == nullptr) {
    return total_size; // No children, return zero size
  }
  for (const auto& child : _child_container->get_children()) {
    if (child->is_visible()) {
      Vector2 child_size = child->get_size();
      total_size.add_x(child_size.get_x());
      total_size.add_y(child_size.get_y());
    }
  }
  return total_size;
}
}