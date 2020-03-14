#pragma once

#include "vector2.h"

namespace Zen {

class Rectangle {
public:
  Rectangle() = default;
  Rectangle(double x, double y, double width, double height);
  Rectangle(Vector2 position, Vector2 size);

  Rectangle& set_position(Vector2 position);
  Rectangle& set_size(Vector2 size);
  void set_x(double x);
  void set_y(double y);
  void set_width(double width);
  void set_height(double height);

  Vector2 get_position() const;
  Vector2 get_size() const;
  double get_x() const;
  double get_y() const;
  double get_width() const;
  double get_height() const;

  Rectangle copy() const;
  Rectangle deep_copy() const;

protected:
  Vector2 _position;
  Vector2 _size;
};
}
