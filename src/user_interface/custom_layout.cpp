#include "custom_layout.h"

namespace Zen {
void CustomLayout::update() {
  for (auto child : _children) {
    child->update();
  }
}

CustomLayout::~CustomLayout() {
  for (auto child : _children) {
    delete child;
  }
}

void CustomLayout::draw(GameGraphics& game_graphics) {
  if (!_visible) return;

  if (_has_background_color) {
    auto size = get_size();
    auto pos = get_position();
    game_graphics.fill_rectangle(Rectangle(pos, size), _background_color);
  }

  for (auto child : _children) {
    child->draw(game_graphics);
  }
}

void CustomLayout::set_parent(CustomLayout* parent) {
  _parent = parent;
}

void CustomLayout::add_child(CustomLayout* child) {
  add_child(child, -1);
}

void CustomLayout::add_child(CustomLayout* child, int position) {
  if (child == nullptr) {
    throw std::runtime_error("Child is null");
  }
  if (child == this) {
    throw std::runtime_error("Can't add self as child");
  }
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
  child->on_size_changed.connect(this, [this]() { _child_size_changed(); });
  _size_current = false;
}

void CustomLayout::remove_child(CustomLayout* child) {
  auto it = std::find(_children.begin(), _children.end(), child);
  if (it != _children.end()) {
    _children.erase(it);
  }
}

void CustomLayout::set_width(SizeTo size_to, double value) {
  _size_to_width = size_to;
  _size_to_width_value = value;
}

void CustomLayout::set_height(SizeTo size_to, double value) {
  _size_to_height = size_to;
  _size_to_height_value = value;
}

void CustomLayout::set_size(SizeTo size_to_width, SizeTo size_to_height) {
  set_size(size_to_width, 0, size_to_height, 0);
}

void CustomLayout::set_size(SizeTo size_to_width, double width_value, SizeTo size_to_height, double height_value) {
  set_width(size_to_width, width_value);
  set_height(size_to_height, height_value);
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
    case SizeTo::PARENT_STATIC:
      if (_parent != nullptr) {
        _height =_parent->get_size().get_x() - _size_to_width_value;
      } else {
        throw std::runtime_error("Parent is null");
      }
      break;
    case SizeTo::CHILDREN:
    case SizeTo::CHILDREN_PERCENT:
      if (_children.empty()) {
        _width = _padding_value_left + _padding_value_right;
      } else if (_layout == Layout::VERTICAL) {
        double max_width = 0;
        for (auto child : _children) {
          double child_width = child->get_width();
          if (child_width > max_width) {
            max_width = child_width;
          }
        }
        if (_size_to_width == SizeTo::CHILDREN_PERCENT) {
          _width = max_width * _size_to_width_value + _padding_value_left + _padding_value_right;
        } else {
          _width = max_width + _padding_value_left + _padding_value_right;
        }
      } if (_layout == Layout::HORIZONTAL) {
        double children_width = 0;
        for (auto child : _children) {
          double child_width = child->get_width();
          if (children_width != 0) {
            children_width += child_width + _child_spacing;
          } else {
            children_width += child_width;
          }
        }
        if (_size_to_width == SizeTo::CHILDREN_PERCENT) {
          _width = children_width * _size_to_width_value + _padding_value_left + _padding_value_right;
        } else {
          _width = children_width + _padding_value_left + _padding_value_right;
        }
      }
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
    case SizeTo::PARENT_STATIC:
      if (_parent != nullptr) {
        _height =_parent->get_size().get_y() - _size_to_height_value;
      } else {
        throw std::runtime_error("Parent is null");
      }
      break;
    case SizeTo::CHILDREN:
    case SizeTo::CHILDREN_PERCENT:
      if (_children.empty()) {
        _height = _padding_value_top + _padding_value_bottom;
      } else if (_layout == Layout::HORIZONTAL) {
        double max_height = 0;
        for (auto child : _children) {
          double child_height = child->get_height();
          if (child_height > max_height) {
            max_height = child_height;
          }
        }
        if (_size_to_height == SizeTo::CHILDREN_PERCENT) {
          _height = max_height * _size_to_height_value + _padding_value_top + _padding_value_bottom;
        } else {
          _height = max_height + _padding_value_top + _padding_value_bottom;
        }
      } if (_layout == Layout::VERTICAL) {
        double children_height = 0;
        for (auto child : _children) {
          double child_height = child->get_height();
          if (children_height != 0) {
            children_height += child_height + _child_spacing;
          } else {
            children_height += child_height;
          }
        }
        if (_size_to_height == SizeTo::CHILDREN_PERCENT) {
          _height = children_height * _size_to_height_value + _padding_value_top + _padding_value_bottom;
        } else {
          _height = children_height + _padding_value_top + _padding_value_bottom;
        }
      }
      break;
    case SizeTo::STATIC:
      _height = _size_to_height_value;
      break;
  }
  _size_current = true;
  return {_width, _height};
}

double CustomLayout::get_width() {
  if (_size_current) {
    return _width;
  } else {
    return get_size().get_x();
  }
}
double CustomLayout::get_height() {
  if (_size_current) {
    return _height;
  } else {
    return get_size().get_y();
  }
}

void CustomLayout::set_padding(double padding_top, double padding_bottom, double padding_left, double padding_right) {
  set_padding(PaddingTo::STATIC, padding_top, PaddingTo::STATIC, padding_bottom, PaddingTo::STATIC, padding_left, PaddingTo::STATIC, padding_right);
}

void CustomLayout::set_padding(PaddingTo padding_to_top, double padding_top,
                               PaddingTo padding_to_bottom, double padding_bottom,
                               PaddingTo padding_to_left, double padding_left,
                               PaddingTo padding_to_right, double padding_right) {
  set_padding_top(padding_top, padding_to_top);
  set_padding_bottom(padding_bottom, padding_to_bottom);
  set_padding_left(padding_left, padding_to_left);
  set_padding_right(padding_right, padding_to_right);
}
void CustomLayout::set_padding_sides(double padding) {
  set_padding_sides(padding, padding);
}
void CustomLayout::set_padding_sides(double padding_left,
                                     double padding_right) {
  set_padding_sides(padding_left, PaddingTo::STATIC, padding_right, PaddingTo::STATIC);
}
void CustomLayout::set_padding_sides(double padding_left, PaddingTo padding_to_left,
                       double padding_right, PaddingTo padding_to_right) {
  set_padding_left(padding_left, padding_to_left);
  set_padding_right(padding_right, padding_to_right);
}
void CustomLayout::set_padding_vertical(double padding) {
  set_padding_vertical(padding, padding);
}
void CustomLayout::set_padding_vertical(double padding_top,
                                        double padding_bottom) {
  set_padding_vertical(padding_top, PaddingTo::STATIC, padding_bottom, PaddingTo::STATIC);
}
void CustomLayout::set_padding_vertical(double padding_top, PaddingTo padding_to_top,
                       double padding_bottom, PaddingTo padding_to_bottom) {
  set_padding_top(padding_top, padding_to_top);
  set_padding_bottom(padding_bottom, padding_to_bottom);
}

void CustomLayout::set_padding_top(double padding_top, PaddingTo padding_to_top) {
  _padding_value_top = padding_top;
  _padding_to_top = padding_to_top;
}

void CustomLayout::set_padding_bottom(double padding_bottom, PaddingTo padding_to_bottom) {
  _padding_value_bottom = padding_bottom;
  _padding_to_bottom = padding_to_bottom;
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

double CustomLayout::get_padding_bottom() {
  if (_padding_to_bottom == PaddingTo::PERCENT) {
    return _height * _padding_value_bottom;
  } else if (_padding_to_bottom == PaddingTo::STATIC) {
    return _padding_value_bottom;
  } else {
    throw(std::runtime_error("Padding bottom is not set to a valid value"));
    return 0;
  }
}

void CustomLayout::set_margin(double margin_top, double margin_bottom, double margin_left, double margin_right) {
  set_margin(PaddingTo::STATIC, margin_top, PaddingTo::STATIC, margin_bottom, PaddingTo::STATIC, margin_left, PaddingTo::STATIC, margin_right);
}

void CustomLayout::set_margin(PaddingTo margin_to_top, double margin_top,
                               PaddingTo margin_to_bottom, double margin_bottom,
                               PaddingTo margin_to_left, double margin_left,
                               PaddingTo margin_to_right, double margin_right) {
  set_margin_top(margin_top, margin_to_top);
  set_margin_bottom(margin_bottom, margin_to_bottom);
  set_margin_left(margin_left, margin_to_left);
  set_margin_right(margin_right, margin_to_right);
}
void CustomLayout::set_margin_sides(double margin) {
  set_margin_sides(margin, margin);
}
void CustomLayout::set_margin_sides(double margin_left,
                                     double margin_right) {
  set_margin_sides(margin_left, PaddingTo::STATIC, margin_right, PaddingTo::STATIC);
}
void CustomLayout::set_margin_sides(double margin_left, PaddingTo margin_to_left,
                       double margin_right, PaddingTo margin_to_right) {
  set_margin_left(margin_left, margin_to_left);
  set_margin_right(margin_right, margin_to_right);
}
void CustomLayout::set_margin_vertical(double margin) {
  set_margin_vertical(margin, margin);
}
void CustomLayout::set_margin_vertical(double margin_top,
                                        double margin_bottom) {
  set_margin_vertical(margin_top, PaddingTo::STATIC, margin_bottom, PaddingTo::STATIC);
}
void CustomLayout::set_margin_vertical(double margin_top, PaddingTo margin_to_top,
                       double margin_bottom, PaddingTo margin_to_bottom) {
  set_margin_top(margin_top, margin_to_top);
  set_margin_bottom(margin_bottom, margin_to_bottom);
}

void CustomLayout::set_margin_top(double margin_top, PaddingTo margin_to_top) {
  _margin_value_top = margin_top;
  _margin_to_top = margin_to_top;
}

void CustomLayout::set_margin_bottom(double margin_bottom, PaddingTo margin_to_bottom) {
  _margin_value_bottom = margin_bottom;
  _margin_to_bottom = margin_to_bottom;
}

void CustomLayout::set_margin_left(double margin_left, PaddingTo margin_to_left) {
  _margin_value_left = margin_left;
  _margin_to_left = margin_to_left;
}

void CustomLayout::set_margin_right(double margin_right, PaddingTo margin_to_right) {
  _margin_value_right = margin_right;
  _margin_to_right = margin_to_right;
}

double CustomLayout::get_margin_left() {
  if (_margin_to_left == PaddingTo::PERCENT) {
    return _width * _margin_value_left;
  } else if (_margin_to_left == PaddingTo::STATIC) {
    return _margin_value_left;
  } else {
    throw(std::runtime_error("Padding left is not set to a valid value"));
    return 0;
  }
}

double CustomLayout::get_margin_right() {
  if (_margin_to_right == PaddingTo::PERCENT) {
    return _width * _margin_value_right;
  } else if (_margin_to_right == PaddingTo::STATIC) {
    return _margin_value_right;
  } else {
    throw(std::runtime_error("Padding right is not set to a valid value"));
    return 0;
  }
}

double CustomLayout::get_margin_top() {
  if (_margin_to_top == PaddingTo::PERCENT) {
    return _height * _margin_value_top;
  } else if (_margin_to_top == PaddingTo::STATIC) {
    return _margin_value_top;
  } else {
    throw(std::runtime_error("Padding top is not set to a valid value"));
    return 0;
  }
}

double CustomLayout::get_margin_bottom() {
  if (_margin_to_bottom == PaddingTo::PERCENT) {
    return _height * _margin_value_bottom;
  } else if (_margin_to_bottom == PaddingTo::STATIC) {
    return _margin_value_bottom;
  } else {
    throw(std::runtime_error("Padding bottom is not set to a valid value"));
    return 0;
  }
}

void CustomLayout::set_position(PositionTo position_to_x, PositionTo position_to_y) {
  set_position_x(position_to_x, 0);
  set_position_y(position_to_y, 0);
}

void CustomLayout::set_position(PositionTo position_to_x, int position_to_x_value, PositionTo position_to_y, int position_to_y_value) {
  set_position_x(position_to_x, position_to_x_value);
  set_position_y(position_to_y, position_to_y_value);
}

void CustomLayout::set_position_x(PositionTo position_to_x, int value_x) {
  _position_to_x = position_to_x;
  _position_to_x_value = value_x;
}

void CustomLayout::set_position_y(PositionTo position_to_y, int value_y) {
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
      _pos_y = _parent->get_position().get_y() + _parent->get_size().get_y() - _height - _parent->get_padding_bottom();
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
    } else if (_layout == Layout::HORIZONTAL) {
      child->_pos_x = next_position;
      next_position += child->get_width() + _child_spacing;
    } else if (_layout == Layout::NONE) {

    }
  }
}
} // namespace Zen