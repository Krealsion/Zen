#include "custom_layout.h"
#include "game_graphics.h"
#include "logic/utils.h"
#include "logger.h"

namespace Zen {

int CustomLayout::_next_id = 0;

CustomLayout::CustomLayout() {
  set_name(Utility::demangle(typeid(*this).name()) + std::to_string(_next_id++));
  _pos_x = 0;
  _pos_y = 0;
  _width_current = false;
  _height_current = false;
  _x_current = false;
  _y_current = false;
}

void CustomLayout::update() {
  for (auto child : _children) {
    child->update();
  }
}

CustomLayout::~CustomLayout() {
  for (auto child : _children) {
    delete child;
  }
  _children.clear();
}

void CustomLayout::draw(GameGraphics& game_graphics) {
  if (!_visible) return;

  if (_has_background_color) {
    game_graphics.fill_rectangle(get_background_destination(), _background_color);
  }

  for (auto child : _children) {
    child->draw(game_graphics);
  }
}

void CustomLayout::set_parent(CustomLayout* parent) {
  _parent = parent;
  _width_current = false;
  _height_current = false;
  _x_current = false;
  _y_current = false;
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
  child->on_size_changed.connect(this, Action<>([this]() { _child_size_changed(); }));
  _width_current = false;
  _height_current = false;
  // TODO set child position to be not current only if a child has a fill size
}

void CustomLayout::remove_child(CustomLayout* child) {
  auto it = std::find(_children.begin(), _children.end(), child);
  if (it != _children.end()) {
    // TODO Engine option for removing to delete the child?
    _children.erase(it);
    _width_current = false;
    _height_current = false;
    _x_current = false;
    _y_current = false;
  }
}

void CustomLayout::reset() {
  _pos_x = 0;
  _pos_y = 0;
  _width = 0;
  _height = 0;
  _x_current = false;
  _y_current = false;
  _width_current = false;
  _height_current = false;
  for (auto child : _children) {
    child->reset();
  }
}

void CustomLayout::set_width(SizeTo size_to, double value) {
  _size_to_width = size_to;
  _size_to_width_value = value;
  _width_current = false;
  if (size_to == SizeTo::STATIC) {
    _width = value;
    _width_current = true;
  }
}

void CustomLayout::set_height(SizeTo size_to, double value) {
  _size_to_height = size_to;
  _size_to_height_value = value;
  _height_current = false;
  if (size_to == SizeTo::STATIC) {
    _height = value;
    _height_current = true;
  }
}

void CustomLayout::set_size(SizeTo size_to_width, SizeTo size_to_height) {
  set_size(size_to_width, 0, size_to_height, 0);
}

void CustomLayout::set_size(SizeTo size_to_width, double width_value, SizeTo size_to_height, double height_value) {
  set_width(size_to_width, width_value);
  set_height(size_to_height, height_value);
}

void CustomLayout::set_size(SizeTo size_to_width, SizeTo size_to_height, double height_value) {
  set_size(size_to_width, 0, size_to_height, height_value);
}

void CustomLayout::set_size(SizeTo size_to_width, double width_value, SizeTo size_to_height) {
  set_size(size_to_width, width_value, size_to_height, 0);
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
  _width_current = false;
  _height_current = false;
  _x_current = false;
  _y_current = false;
}

void CustomLayout::set_horizontal() {
  _layout = Layout::HORIZONTAL;
  for (auto child : _children) {
    child->set_position_x(PositionTo::PARENT_CONTROLLED, 0);
  }
  _width_current = false;
  _height_current = false;
  _x_current = false;
  _y_current = false;
}

void CustomLayout::click() {
  if (_enabled && _on_click_callback) {
    _on_click_callback();
  }
}

Vector2 CustomLayout::get_size() {
  return {get_width(), get_height()};
}

double CustomLayout::get_width() {
  if (_size_to_width == SizeTo::STATIC) {
    _width = _size_to_width_value;
    _width_current = true;
    return _width;
  }
  if (_width_current) {
    return _width;
  }
  switch (_size_to_width) {
    case SizeTo::PARENT:{
      _width = _parent->get_inside_width() - _size_to_width_value;
      break;
    }
    case SizeTo::PARENT_PERCENT: {
      // TODO error handling with parent percent going over 1.0
      _width = _parent->get_inside_width() * _size_to_width_value;
      break;
    }
    case SizeTo::CHILDREN:
    case SizeTo::CHILDREN_PERCENT:
      if (_children.empty()) {
        _width = _padding_value_left + _padding_value_right;
      } else if (_layout == Layout::VERTICAL || _layout == Layout::CHILD_CONTROLLED) {
        double max_width = 0;
        for (auto child : _children) {
          if (child->_size_to_width == SizeTo::PARENT_PERCENT || child->_size_to_width == SizeTo::PARENT) {
            // If a child is set to PARENT or PARENT_PERCENT, it will not contribute to the max width
            continue;
          }
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
      } else if (_layout == Layout::HORIZONTAL) {
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
    case SizeTo::FILL: {
      int fill_value = 0;
      int non_fill_children_width = 0;
      for (auto* child: _parent->_children) {
        if (child->_size_to_width == SizeTo::FILL) {
          fill_value += _size_to_width_value;
        } else {
          non_fill_children_width += child->get_width();
        }
      }
      _width = (_parent->get_inside_width() - non_fill_children_width) * (_size_to_width_value / fill_value);
      break;
    }
    case SizeTo::STATIC:
      _width = _size_to_width_value;
      break;
    default:
      Logger::log(LogLevel::ERROR, "Invalid SizeTo for width");
      _width = 0;
      break;
  }
  if (_width < 0) {
    Logger::log(LogLevel::ERROR, "CustomLayout: Width is negative, setting to 0");
    _width = 0;
  }
//  if (_min_width < ) // TODO min width to margin + padding
  if (_width < _min_width) {
    _width = _min_width;
  }
  if (_max_width > 0 && _width > _max_width) {
    _width = _max_width;
  }
  _width_current = true;
  return _width;
}

double CustomLayout::get_height() {
  if (_size_to_height == SizeTo::STATIC) {
    _height = _size_to_height_value;
    _height_current = true;
  }
  if (_height_current) {
    return _height;
  }
  switch (_size_to_height) {
    case SizeTo::PARENT: {
      _height = _parent->get_inside_height() - _size_to_height_value;
      break;
    }
    case SizeTo::PARENT_PERCENT: {
      _height = _parent->get_inside_height() * _size_to_height_value;
      break;
    }
    case SizeTo::CHILDREN:
    case SizeTo::CHILDREN_PERCENT:
      if (_children.empty()) {
        _height = _padding_value_top + _padding_value_bottom;
      } else if (_layout == Layout::HORIZONTAL || _layout == Layout::CHILD_CONTROLLED) {
        double max_height = 0;
        for (auto child : _children) {
          if (child->_size_to_height == SizeTo::PARENT_PERCENT || child->_size_to_height == SizeTo::PARENT) {
            // TODO multiply width by parent percent, and set the children's height here?
            continue;
          }
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
      } else if (_layout == Layout::VERTICAL) {
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
    case SizeTo::FILL: {
      int fill_value = 0;
      int non_fill_children_height = 0;
      for (auto* child: _parent->_children) {
        if (child->_size_to_height == SizeTo::FILL) {
          fill_value += _size_to_height_value;
        } else {
          non_fill_children_height += child->get_height();
        }
      }
      _height = (_parent->get_inside_height() - non_fill_children_height) * (_size_to_height_value / fill_value);
      break;
    }
    case SizeTo::STATIC:
      _height = _size_to_height_value;
      break;
    default:
      Logger::log(LogLevel::ERROR, "Invalid SizeTo for height");
      _height = 0;
      break;
  }
  if (_height < _min_height) {
    _height = _min_height;
  }
  if (_max_height > 0 && _height > _max_height) {
    _height = _max_height;
  }
  _height_current = true;
  return _height;
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
  _height_current = false;
  _width_current = false;
}

void CustomLayout::set_padding_sides(double padding) {
  set_padding_sides(padding, padding);
}

void CustomLayout::set_padding_sides(double padding_left, double padding_right) {
  set_padding_sides(padding_left, PaddingTo::STATIC, padding_right, PaddingTo::STATIC);
}

void CustomLayout::set_padding_sides(double padding_left, PaddingTo padding_to_left,
                                     double padding_right, PaddingTo padding_to_right) {
  set_padding_left(padding_left, padding_to_left);
  set_padding_right(padding_right, padding_to_right);
  _height_current = false;
  _width_current = false;
}

void CustomLayout::set_padding_vertical(double padding) {
  set_padding_vertical(padding, padding);
}

void CustomLayout::set_padding_vertical(double padding_top, double padding_bottom) {
  set_padding_vertical(padding_top, PaddingTo::STATIC, padding_bottom, PaddingTo::STATIC);
}

void CustomLayout::set_padding_vertical(double padding_top, PaddingTo padding_to_top,
                                        double padding_bottom, PaddingTo padding_to_bottom) {
  set_padding_top(padding_top, padding_to_top);
  set_padding_bottom(padding_bottom, padding_to_bottom);
  _height_current = false;
  _width_current = false;
}

void CustomLayout::set_padding_top(double padding_top, PaddingTo padding_to_top) {
  _padding_value_top = padding_top;
  _padding_to_top = padding_to_top;
  _height_current = false;
  _width_current = false;
}

void CustomLayout::set_padding_bottom(double padding_bottom, PaddingTo padding_to_bottom) {
  _padding_value_bottom = padding_bottom;
  _padding_to_bottom = padding_to_bottom;
  _height_current = false;
  _width_current = false;
}

void CustomLayout::set_padding_left(double padding_left, PaddingTo padding_to_left) {
  _padding_value_left = padding_left;
  _padding_to_left = padding_to_left;
  _height_current = false;
  _width_current = false;
}

void CustomLayout::set_padding_right(double padding_right, PaddingTo padding_to_right) {
  _padding_value_right = padding_right;
  _padding_to_right = padding_to_right;
  _height_current = false;
  _width_current = false;
}

double CustomLayout::get_padding_left() {
  try {
    if (_padding_to_left == PaddingTo::PERCENT) {
      return _parent->get_width() * _padding_value_left;
    } else if (_padding_to_left == PaddingTo::STATIC) {
      return _padding_value_left;
    }
    throw std::runtime_error("Invalid PaddingTo for left");
  } catch (const std::exception& e) {
    Logger::log(LogLevel::ERROR, e.what());
    return 0;
  }
}

double CustomLayout::get_padding_right() {
  try {
    if (_padding_to_right == PaddingTo::PERCENT) {
      return _parent->get_width() * _padding_value_right;
    } else if (_padding_to_right == PaddingTo::STATIC) {
      return _padding_value_right;
    }
    throw std::runtime_error("Invalid PaddingTo for right");
  } catch (const std::exception& e) {
    Logger::log(LogLevel::ERROR, e.what());
    return 0;
  }
}

double CustomLayout::get_padding_top() {
  try {
    if (is_root()) {
      return _padding_value_top;
    }
    if (_padding_to_top == PaddingTo::PERCENT) {
      return _parent->get_height() * _padding_value_top;
    } else if (_padding_to_top == PaddingTo::STATIC) {
      return _padding_value_top;
    }
    throw std::runtime_error("Invalid PaddingTo for top");
  } catch (const std::exception& e) {
    Logger::log(LogLevel::ERROR, e.what());
    return 0;
  }
}

double CustomLayout::get_padding_bottom() {
  try {
    if (is_root()) {
      return _padding_value_bottom;
    }
    if (_padding_to_bottom == PaddingTo::PERCENT) {
      return _parent->get_height() * _padding_value_bottom;
    } else if (_padding_to_bottom == PaddingTo::STATIC) {
      return _padding_value_bottom;
    }
    throw std::runtime_error("Invalid PaddingTo for bottom");
  } catch (const std::exception& e) {
    Logger::log(LogLevel::ERROR, e.what());
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
  _height_current = false;
  _width_current = false;
}

void CustomLayout::set_margin_sides(double margin) {
  set_margin_sides(margin, margin);
}

void CustomLayout::set_margin_sides(double margin_left, double margin_right) {
  set_margin_sides(margin_left, PaddingTo::STATIC, margin_right, PaddingTo::STATIC);
}

void CustomLayout::set_margin_sides(double margin_left, PaddingTo margin_to_left,
                                    double margin_right, PaddingTo margin_to_right) {
  set_margin_left(margin_left, margin_to_left);
  set_margin_right(margin_right, margin_to_right);
  _height_current = false;
  _width_current = false;
}

void CustomLayout::set_margin_vertical(double margin) {
  set_margin_vertical(margin, margin);
}

void CustomLayout::set_margin_vertical(double margin_top, double margin_bottom) {
  set_margin_vertical(margin_top, PaddingTo::STATIC, margin_bottom, PaddingTo::STATIC);
}

void CustomLayout::set_margin_vertical(double margin_top, PaddingTo margin_to_top,
                                       double margin_bottom, PaddingTo margin_to_bottom) {
  set_margin_top(margin_top, margin_to_top);
  set_margin_bottom(margin_bottom, margin_to_bottom);
  _height_current = false;
  _width_current = false;
}

void CustomLayout::set_margin_top(double margin_top, PaddingTo margin_to_top) {
  _margin_value_top = margin_top;
  _margin_to_top = margin_to_top;
  _height_current = false;
  _width_current = false;
}

void CustomLayout::set_margin_bottom(double margin_bottom, PaddingTo margin_to_bottom) {
  _margin_value_bottom = margin_bottom;
  _margin_to_bottom = margin_to_bottom;
  _height_current = false;
  _width_current = false;
}

void CustomLayout::set_margin_left(double margin_left, PaddingTo margin_to_left) {
  _margin_value_left = margin_left;
  _margin_to_left = margin_to_left;
  _height_current = false;
  _width_current = false;
}

void CustomLayout::set_margin_right(double margin_right, PaddingTo margin_to_right) {
  _margin_value_right = margin_right;
  _margin_to_right = margin_to_right;
  _height_current = false;
  _width_current = false;
}

double CustomLayout::get_margin_left() {
  try {
    if (is_root()) {
      return _margin_value_left;
    }
    if (_margin_to_left == PaddingTo::PERCENT) {
      return _parent->get_width() * _margin_value_left;
    } else if (_margin_to_left == PaddingTo::STATIC) {
      return _margin_value_left;
    }
    throw std::runtime_error("Invalid PaddingTo for margin left");
  } catch (const std::exception& e) {
    Logger::log(LogLevel::ERROR, e.what());
    return 0;
  }
}

double CustomLayout::get_margin_right() {
  try {
    if (is_root()) {
      return _margin_value_right;
    }
    if (_margin_to_right == PaddingTo::PERCENT) {
      return _parent->get_width() * _margin_value_right;
    } else if (_margin_to_right == PaddingTo::STATIC) {
      return _margin_value_right;
    }
    throw std::runtime_error("Invalid PaddingTo for margin right");
  } catch (const std::exception& e) {
    Logger::log(LogLevel::ERROR, e.what());
    return 0;
  }
}

double CustomLayout::get_margin_top() {
  try {
    if (is_root()) {
      return _margin_value_top;
    }
    if (_margin_to_top == PaddingTo::PERCENT) {
      return _parent->get_height() * _margin_value_top;
    } else if (_margin_to_top == PaddingTo::STATIC) {
      return _margin_value_top;
    }
    throw std::runtime_error("Invalid PaddingTo for margin top");
  } catch (const std::exception& e) {
    Logger::log(LogLevel::ERROR, e.what());
    return 0;
  }
}

double CustomLayout::get_margin_bottom() {
  try {
    if (is_root()) {
      return _margin_value_bottom;
    }
    if (_margin_to_bottom == PaddingTo::PERCENT) {
      return _parent->get_height() * _margin_value_bottom;
    } else if (_margin_to_bottom == PaddingTo::STATIC) {
      return _margin_value_bottom;
    }
    throw std::runtime_error("Invalid PaddingTo for margin bottom");
  } catch (const std::exception& e) {
    Logger::log(LogLevel::ERROR, e.what());
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
  if (position_to_x == _position_to_x && value_x == _position_to_x_value) {
    return;
  }
  _position_to_x = position_to_x;
  _position_to_x_value = value_x;
  _x_current = false;
  _on_position_changed();
}

void CustomLayout::set_position_y(PositionTo position_to_y, int value_y) {
  if (position_to_y == _position_to_y && value_y == _position_to_y_value) {
    return;
  }
  _position_to_y = position_to_y;
  _position_to_y_value = value_y;
  _y_current = false;
  for (auto child : _children) {
    child->_y_current = false; // Reset child y position
  }
  _on_position_changed();
}

double CustomLayout::get_y() {
  if (_y_current) {
    return _pos_y;
  }
  Rectangle parent_inside;
  switch (_position_to_y) {
    case PositionTo::RELATIVE:
      parent_inside = _parent->get_inside_destination();
      _pos_y = parent_inside.get_y() + _position_to_y_value;
      break;
    case PositionTo::PARENT_CONTROLLED:
      _parent->_request_child_position_update();
      break;
    case PositionTo::TOP:
      parent_inside = _parent->get_inside_destination();
      _pos_y = parent_inside.get_y();
      break;
    case PositionTo::BOTTOM:
      parent_inside = _parent->get_inside_destination();
      _pos_y = parent_inside.get_y() + parent_inside.get_height() - get_height();
      break;
    case PositionTo::CENTER:
      parent_inside = _parent->get_inside_destination();
      _pos_y = parent_inside.get_y() + (parent_inside.get_height() - get_height()) / 2;
      break;
    case PositionTo::LEFT:
    case PositionTo::RIGHT:
      Logger::log(LogLevel::ERROR, "Invalid PositionTo for y: " + std::to_string(static_cast<int>(_position_to_y)));

      _pos_y = 0;
      break;
  }
  _y_current = true;
  return _pos_y;
}

double CustomLayout::get_x() {
  if (_x_current) {
    return _pos_x;
  }
  Rectangle parent_inside;
  switch (_position_to_x) {
    case PositionTo::RELATIVE:
      parent_inside = _parent->get_inside_destination();
      _pos_x = parent_inside.get_x() + _position_to_x_value;
      break;
    case PositionTo::PARENT_CONTROLLED:
      _parent->_request_child_position_update();
      break;
    case PositionTo::LEFT:
      parent_inside = _parent->get_inside_destination();
      _pos_x = parent_inside.get_x();
      break;
    case PositionTo::RIGHT:
      parent_inside = _parent->get_inside_destination();
      _pos_x = parent_inside.get_x() + parent_inside.get_width() - get_width();
      break;
    case PositionTo::CENTER:
      parent_inside = _parent->get_inside_destination();
      _pos_x = parent_inside.get_x() + (parent_inside.get_width() - get_width()) / 2;
      break;
    case PositionTo::TOP:
    case PositionTo::BOTTOM:
      Logger::log(LogLevel::ERROR, "Invalid PositionTo for x: " + std::to_string(static_cast<int>(_position_to_x)));
      throw std::runtime_error("Invalid PositionTo for x");
      _pos_x = 0;
      break;
  }
  _x_current = true;
  return _pos_x;
}

Vector2 CustomLayout::get_position() {
  if (is_root()) {
    _pos_x = _position_to_x_value;
    _pos_y = _position_to_y_value;
    _x_current = true;
    _y_current = true;
    return {_pos_x, _pos_y};
  }

  return {get_x(), get_y()};
}

Rectangle CustomLayout::get_owned_destination() {
  auto owned = Rectangle(get_position().get_x(), get_position().get_y(), get_width(), get_height());
  return owned;
}

Rectangle CustomLayout::get_background_destination() {
  auto owned = get_owned_destination();
  owned.set_x(owned.get_x() + get_margin_left());
  owned.set_y(owned.get_y() + get_margin_top());
  owned.set_width(owned.get_width() - get_margin_right() - get_margin_left());
  owned.set_height(owned.get_height() - get_margin_top() - get_margin_bottom());
  return owned;
}

Rectangle CustomLayout::get_inside_destination() {
  auto bg_dest = get_background_destination();
  bg_dest.set_x(bg_dest.get_x() + get_padding_left());
  bg_dest.set_y(bg_dest.get_y() + get_padding_top());
  bg_dest.set_width(bg_dest.get_width() - get_padding_right() - get_padding_left());
  bg_dest.set_height(bg_dest.get_height() - get_padding_top() - get_padding_bottom());
  return bg_dest;
}

void CustomLayout::_request_child_position_update() {
  // Should not be called when a child layout is controlled by the parent size
  if (_layout == Layout::CHILD_CONTROLLED) return;
  double next_position = (_layout == Layout::VERTICAL) ? get_position().get_y() : get_position().get_x();
  for (auto child : _children) {
    if (!child->is_visible()) {
      continue;
    }
    if (_layout == Layout::VERTICAL) {
      child->_pos_y = next_position;
      child->_y_current = true;
      next_position += child->get_height() + _child_spacing;
    } else if (_layout == Layout::HORIZONTAL) {
      child->_pos_x = next_position;
      child->_x_current = true;
      next_position += child->get_width() + _child_spacing;
    }
  }
}

void CustomLayout::_on_size_changed() {
  _width_current = false;
  _height_current = false;
  for (auto child : _children) {
  }
  if (_parent) {
    _parent->_child_size_changed(); // Bubble up
  }
}

void CustomLayout::_on_position_changed() {
  _x_current = false;
  _y_current = false;
  for (auto child : _children) {
    child->_on_position_changed();
  }
}

void CustomLayout::_child_size_changed() {
  _height_current = false;
  _width_current = false;
  _x_current = false;
  _y_current = false;
  if (_parent) {
    _parent->_child_size_changed(); // Bubble up
  }
  for (auto child : _children) {
    if (child->_size_to_width == SizeTo::PARENT_PERCENT || child->_size_to_width == SizeTo::PARENT || child->_size_to_width == SizeTo::FILL) {
      child->_width_current = false; // Reset size current for children with PARENT related values
      child->_x_current = false; // Reset position current for children
    }
    if (child->_size_to_height == SizeTo::PARENT_PERCENT || child->_size_to_height == SizeTo::PARENT || child->_size_to_height == SizeTo::FILL) {
      child->_height_current = false; // Reset size current for children with PARENT related values
      child->_y_current = false; // Reset position current for children
    }
  }
}

} // namespace Zen
