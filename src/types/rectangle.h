#pragma once

#include "vector2.h"

namespace Zen {

class Rectangle {
public:
  Rectangle() = default;
  Rectangle(double x, double y, double width, double height);
  Rectangle(Vector2 position, Vector2 size);
  Rectangle(const Rectangle& other) = default;

  // TODO const& pass
  Rectangle& set_position(Vector2 position);
  Rectangle& set_size(Vector2 size);
  void set_x(double x);
  void set_y(double y);
  void set_width(double width);
  void set_height(double height);

  Vector2 get_position() const;
  Vector2 get_size() const;
  Vector2& get_position_mutable();
  Vector2& get_size_mutable();
  double get_x() const;
  double get_y() const;
  double get_width() const;
  double get_height() const;

  Rectangle& add(const Rectangle& other);

  bool contains(Vector2 position) const;

  Rectangle copy() const;
  Rectangle deep_copy() const;

  Rectangle& operator =(const Rectangle& other) = default;

protected:
  Vector2 _position;
  Vector2 _size;
};
}
