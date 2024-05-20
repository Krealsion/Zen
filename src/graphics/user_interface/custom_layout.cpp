#include "custom_layout.h"

namespace Zen {

CustomLayout::~CustomLayout() {
  for (auto child : _children) {
    delete child;
  }
}

void CustomLayout::draw(Zen::GameGraphics& game_graphics) {
  if (!_visible) return;

  if (_has_background_color) {
    auto size = get_size();
    auto pos = get_position();
    game_graphics.fill_rectangle(Rectangle(pos, size), _background_color);
  }

  for (auto child : _children) {
    child->_draw_visible(game_graphics);
  }
}

void CustomLayout::set_parent(CustomLayout* parent) {
  _parent = parent;
}

void CustomLayout::add_child(CustomLayout* child, int position) {
  if (child->_parent != nullptr) {
    throw std::runtime_error("Child already has a parent");
  }
  if (position == -1) {
    _children.push_back(child);
  } else {
    _children.insert(_children.begin() + position, child);
  }
  if (_layout == Layout::VERTICAL) {
    child->set_position_y(PositionTo::PARENT_CONTROLLED, 0);
  } else if (_layout == Layout::HORIZONTAL) {
    child->set_position_x(PositionTo::PARENT_CONTROLLED, 0);
  }
  child->set_parent(this);
  child->on_size_changed.connect(this, [this]() { child_size_changed(); });
  _size_current = false;
}

void CustomLayout::remove_child(CustomLayout* child) {
  auto it = std::find(_children.begin(), _children.end(), child);
  if (it != _children.end()) {
    _children.erase(it);
  }
}

void CustomLayout::set_width(SizeTo size_to, int value) {
  _size_to_width = size_to;
  _size_to_width_value = value;
}

void CustomLayout::set_height(SizeTo size_to, int value) {
  _size_to_height = size_to;
  _size_to_height_value = value;
}

void CustomLayout::set_size(SizeTo size_to_width, SizeTo size_to_height, double width_value, double height_value) {
  _size_to_width = size_to_width;
  _size_to_height = size_to_height;
  _size_to_width_value = width_value;
  _size_to_height_value = height_value;
}

void CustomLayout::center() {
  if (_position_to_x == PositionTo::PARENT_CONTROLLED) {
    set_position_y(PositionTo::CENTER, 0);
  } else if (_position_to_y == PositionTo::PARENT_CONTROLLED) {
    set_position_x(PositionTo::CENTER, 0);
  } else {
    set_position(PositionTo::CENTER, 0, PositionTo::CENTER, 0);
  }
}

void CustomLayout::set_vertical() {
  _layout = Layout::VERTICAL;
  for (auto child : _children) {
    child->set_position_y(PositionTo::PARENT_CONTROLLED, 0);
  }
}

void CustomLayout::set_horizontal() {
  _layout = Layout::HORIZONTAL;
  for (auto child : _children) {
    child->set_position_x(PositionTo::PARENT_CONTROLLED, 0);
  }
}

void CustomLayout::click() {
  if (_enabled && _on_click_callback) {
    _on_click_callback();
  }
}

void CustomLayout::_draw_visible(GameGraphics& game_graphics) {
  if (_visible){
    draw(game_graphics);
  }
}

Vector2 CustomLayout::get_size() {
  if (_size_current) {
    return {_width, _height};
  }
  switch (_size_to_width) {
    case SizeTo::NONE:
      break;
    case SizeTo::PARENT:
      if (_parent != nullptr) {
        _width =_parent->get_size().get_x();
      } else {
        throw std::runtime_error("Parent is null");
      }
      break;
    case SizeTo::PARENT_PERCENT:
      if (_parent != nullptr) {
        _width =_parent->get_size().get_x() * _size_to_width_value;
      } else {
        throw std::runtime_error("Parent is null");
      }
      break;
    case SizeTo::CHILDREN:
      // TODO: Implement
      // This will be based on the layout type
      break;
    case SizeTo::CHILDREN_PERCENT:
      break;
    case SizeTo::STATIC:
      _width =_size_to_width_value;
      break;
  }
  switch (_size_to_height) {
    case SizeTo::NONE:
      break;
    case SizeTo::PARENT:
      if (_parent != nullptr) {
        _height =_parent->get_size().get_y();
      } else {
        throw std::runtime_error("Parent is null");
      }
      break;
    case SizeTo::PARENT_PERCENT:
      if (_parent != nullptr) {
        _height =_parent->get_size().get_y() * _size_to_height_value;
      } else {
        throw std::runtime_error("Parent is null");
      }
      break;
    case SizeTo::CHILDREN:
      break;
    case SizeTo::CHILDREN_PERCENT:
      break;
    case SizeTo::STATIC:
      _height = _size_to_height_value;
      break;
  }
  _size_current = true;
  return {_width, _height};
}

void CustomLayout::set_padding(PaddingTo padding_to_top, double padding_top,
                               PaddingTo padding_to_down, double padding_down,
                               PaddingTo padding_to_left, double padding_left,
                               PaddingTo padding_to_right, double padding_right) {
  set_padding_top(padding_top, padding_to_top);
  set_padding_down(padding_down, padding_to_down);
  set_padding_left(padding_left, padding_to_left);
  set_padding_right(padding_right, padding_to_right);
}

void CustomLayout::set_padding_top(double padding_top, PaddingTo padding_to_top) {
  _padding_value_top = padding_top;
  _padding_to_top = padding_to_top;
}

void CustomLayout::set_padding_down(double padding_down, PaddingTo padding_to_down) {
  _padding_value_down = padding_down;
  _padding_to_down = padding_to_down;
}

void CustomLayout::set_padding_left(double padding_left, PaddingTo padding_to_left) {
  _padding_value_left = padding_left;
  _padding_to_left = padding_to_left;
}

void CustomLayout::set_padding_right(double padding_right, PaddingTo padding_to_right) {
  _padding_value_right = padding_right;
  _padding_to_right = padding_to_right;
}

double CustomLayout::get_padding_left() {
  if (_padding_to_left == PaddingTo::PERCENT) {
    return _width * _padding_value_left;
  } else if (_padding_to_left == PaddingTo::STATIC) {
    return _padding_value_left;
  } else {
    throw(std::runtime_error("Padding left is not set to a valid value"));
    return 0;
  }
}

double CustomLayout::get_padding_right() {
  if (_padding_to_right == PaddingTo::PERCENT) {
    return _width * _padding_value_right;
  } else if (_padding_to_right == PaddingTo::STATIC) {
    return _padding_value_right;
  } else {
    throw(std::runtime_error("Padding right is not set to a valid value"));
    return 0;
  }
}

double CustomLayout::get_padding_top() {
  if (_padding_to_top == PaddingTo::PERCENT) {
    return _height * _padding_value_top;
  } else if (_padding_to_top == PaddingTo::STATIC) {
    return _padding_value_top;
  } else {
    throw(std::runtime_error("Padding top is not set to a valid value"));
    return 0;
  }
}

double CustomLayout::get_padding_down() {
  if (_padding_to_down == PaddingTo::PERCENT) {
    return _height * _padding_value_down;
  } else if (_padding_to_down == PaddingTo::STATIC) {
    return _padding_value_down;
  } else {
    throw(std::runtime_error("Padding down is not set to a valid value"));
    return 0;
  }
}

void CustomLayout::set_position(PositionTo position_to_x, int position_to_x_value, PositionTo position_to_y, int position_to_y_value) {
  set_position_x(position_to_x, position_to_x_value);
  set_position_y(position_to_y, position_to_y_value);
}

void CustomLayout::set_position_x(Zen::PositionTo position_to_x, int value_x) {
  _position_to_x = position_to_x;
  _position_to_x_value = value_x;
}

void CustomLayout::set_position_y(Zen::PositionTo position_to_y, int value_y) {
  _position_to_y = position_to_y;
  _position_to_y_value = value_y;
}

Vector2 CustomLayout::get_position() {
  if (_position_current) {
    return {_pos_x, _pos_y};
  }
  switch (_position_to_x) {
    case PositionTo::LEFT:
      _pos_x = _parent->get_position().get_x() + _parent->get_padding_left();
      break;
    case PositionTo::RIGHT:
      _pos_x = _parent->get_position().get_x() + _parent->get_size().get_x() - _width - _parent->get_padding_right();
      break;
    case PositionTo::CENTER:
      _pos_x = _parent->get_position().get_x() + (_parent->get_size().get_x() - _width) / 2;
      break;
    case PositionTo::RELATIVE:
      _pos_x = _position_to_x_value + _parent->get_position().get_x();
      break;
    case PositionTo::PARENT_CONTROLLED:
      _parent->_request_child_position_update();
      break;
    case PositionTo::TOP:
    case PositionTo::BOTTOM:
    case PositionTo::NONE:
      break;
  }
  switch (_position_to_y) {
    case PositionTo::TOP:
      _pos_y = _parent->get_position().get_y() + _parent->get_padding_top();
      break;
    case PositionTo::BOTTOM:
      _pos_y = _parent->get_position().get_y() + _parent->get_size().get_y() - _height - _parent->get_padding_down();
      break;
    case PositionTo::CENTER:
      _pos_y = _parent->get_position().get_y() + (_parent->get_size().get_y() - _height) / 2;
      break;
    case PositionTo::RELATIVE:
      _pos_y = _position_to_y_value + _parent->get_position().get_y();
      break;
    case PositionTo::PARENT_CONTROLLED:
      _parent->_request_child_position_update();
      break;
    case PositionTo::LEFT:
    case PositionTo::RIGHT:
    case PositionTo::NONE:
      break;
  }

  return {_pos_x, _pos_y};
}

void CustomLayout::_request_child_position_update() {
  if (_layout == Layout::NONE) return;
  double next_position = _child_spacing;
  for (auto child : _children) {
    if (_layout == Layout::VERTICAL) {
      child->_pos_y = next_position;
      next_position += child->get_height() + _child_spacing;
    } else {
      child->_pos_x = next_position;
      next_position += child->get_width() + _child_spacing;
    }
  }
}
} // namespace Zen