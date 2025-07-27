#include "rectangle.h"

namespace Zen {

Rectangle::Rectangle(double x, double y, double width, double height) {
  _position = Vector2(x, y);
  _size = Vector2(width, height);
}

Rectangle::Rectangle(Vector2 position, Vector2 size) {
  this->_position = position;
  this->_size = size;
}

Rectangle& Rectangle::set_position(Vector2 position) {
  this->_position = position;
  return *this;
}

Rectangle& Rectangle::set_size(Vector2 size) {
  this->_size = size;
  return *this;
}

void Rectangle::set_x(double x) {
  _position.set_x(x);
}

void Rectangle::set_y(double y) {
  _position.set_y(y);
}

void Rectangle::set_width(double width) {
  _size.set_x(width);
}

void Rectangle::set_height(double height) {
  _size.set_y(height);
}

Vector2 Rectangle::get_position() const {
  return _position;
}

Vector2 Rectangle::get_size() const {
  return _size;
}

double Rectangle::get_x() const {
  return _position.get_x();
}

double Rectangle::get_y() const {
  return _position.get_y();
}

double Rectangle::get_width() const {
  return _size.get_x();
}

double Rectangle::get_height() const {
  return _size.get_y();
}

bool Rectangle::contains(Vector2 position) const {
  return position.get_x() >= _position.get_x() &&
    position.get_y() >= _position.get_y() &&
      position.get_x() <= _position.get_x() + _size.get_x() &&
        position.get_y() <= _position.get_y() + _size.get_y();

}

Rectangle Rectangle::copy() const {
  return {_position, _size};
}

Rectangle Rectangle::deep_copy() const {
  return {_position.copy(), _size.copy()};
}
Vector2& Rectangle::get_position_mutable() {
  return _position;
}
Vector2& Rectangle::get_size_mutable() {
  return _size;
}
}
