#pragma once

#include <iostream>

namespace Zen {

class Vector3 {
public:
  Vector3();
  Vector3(double x, double y, double z);

  //Returns a reference to this object, not a copy
  Vector3& set_x(double x);
  Vector3& set_y(double y);
  Vector3& set_z(double z);
  Vector3& add_x(double x);
  Vector3& add_y(double y);
  Vector3& add_z(double z);
  Vector3& add(const Vector3& o);
  Vector3& multiply(const Vector3& o);
  Vector3& scale(double scalar);
  Vector3& normalize();
  Vector3& abs();
  Vector3& negate();
  Vector3& invert();
  bool operator==(const Vector3& vector3) const;

  static Vector3 scale(const Vector3& v, const double& s);
  static Vector3 cross_product(const Vector3& a, const Vector3& b);
  static double dot_product(const Vector3& a, const Vector3& b);

  double get_x() const;
  double get_y() const;
  double get_z() const;
  int get_x_int() const;
  int get_y_int() const;
  int get_z_int() const;
  double get_magnitude() const;

  Vector3 copy() const;

  friend std::ostream& operator<<(std::ostream& os, const Vector3& v3);

  Vector3 operator +(const Vector3& o) const {
    return {o.get_x() + get_x(), o.get_y() + get_y(), o.get_z() + get_z()};
  }
  Vector3 operator -(const Vector3& o) const {
    return {o.get_x() - get_x(), o.get_y() - get_y(), o.get_z() - get_z()};
  }
  Vector3 operator *(const double& d) const {
    return {get_x() * d, get_y() * d, get_z() * d};
  }
  Vector3 operator *(const Vector3& o) const {
    return {o.get_x() * get_x(), o.get_y() * get_y(), o.get_z() * get_z()};
  }

protected:
  double _x, _y, _z;
};
}
