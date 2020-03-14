#pragma once

#include <iostream>

namespace Zen {

class Vector2 {
public:
  Vector2();
  Vector2(double x, double y);

  //Returns a reference to this object, not a copy
  Vector2& set_x(double x);
  Vector2& set_y(double y);
  Vector2& add_x(double x);
  Vector2& add_y(double y);
  Vector2& add(Vector2 o);
  Vector2& multiply(Vector2 o);
  Vector2& scale(double scalar);
  Vector2& normalize();
  Vector2& abs();
  Vector2& negate();
  Vector2& invert();

  double get_x() const;
  double get_y() const;
  int get_x_int() const;
  int get_y_int() const;
  double get_magnitude() const;

  Vector2 copy() const;

  friend std::ostream& operator<<(std::ostream& os, const Vector2& v2);

protected:
  double _x, _y;
};
}
