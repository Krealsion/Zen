#include "vector3.h"

#include <cmath>
#include <iostream>

namespace Zen {

Vector3::Vector3() {
  _x = 0;
  _y = 0;
  _z = 0;
}

Vector3::Vector3(double x, double y, double z) {
  _x = x;
  _y = y;
  _z = z;
}

Vector3& Vector3::set_x(double x) {
  _x = x;
  return *this;
}

Vector3& Vector3::set_y(double y) {
  _y = y;
  return *this;
}

Vector3& Vector3::set_z(double z) {
  _z = z;
  return *this;
}

Vector3& Vector3::add_x(double x) {
  _x += x;
  return *this;
}

Vector3& Vector3::add_y(double y) {
  _y += y;
  return *this;
}

Vector3& Vector3::add_z(double z) {
  _z += z;
  return *this;
}

double Vector3::get_x() const {
  return _x;
}

double Vector3::get_y() const {
  return _y;
}

double Vector3::get_z() const {
  return _z;
}

int Vector3::get_x_int() const {
  return (int) round(_x);
}

int Vector3::get_y_int() const {
  return (int) round(_y);
}

int Vector3::get_z_int() const {
  return (int) round(_z);
}

Vector3& Vector3::add(const Vector3& o) {
  _x += o.get_x();
  _y += o.get_y();
  _y += o.get_z();
  return *this;
}

Vector3& Vector3::multiply(const Vector3& o) {
  _x *= o.get_x();
  _y *= o.get_y();
  _z *= o.get_z();
  return *this;
}

Vector3& Vector3::scale(double scalar) {
  _x *= scalar;
  _y *= scalar;
  _z *= scalar;
  return *this;
}

double Vector3::get_magnitude() const {
  return sqrt(_x * _x + _y * _y + _z * _z);
}

Vector3& Vector3::normalize() {
  return scale(1 / get_magnitude());
}

Vector3& Vector3::abs() {
  _x = fabs(_x);
  _y = fabs(_y);
  _z = fabs(_z);
  return *this;
}

Vector3& Vector3::negate() {
  _x *= -1;
  _y *= -1;
  _z *= -1;
  return *this;
}

Vector3& Vector3::invert() {
  _x = 1 / _x;
  _y = 1 / _y;
  _z = 1 / _z;
  return *this;
}

Vector3 Vector3::add(const Vector3& a, const Vector3& b) {
  return {a.get_x() + b.get_x(), a.get_y() + b.get_y(), a.get_z() + b.get_z()};
}

Vector3 Vector3::multiply(const Vector3& a, const Vector3& b) {
  return {a.get_x() * b.get_x(), a.get_y() * b.get_y(), a.get_z() * b.get_z()};
}

Vector3 Vector3::scale(const Vector3& v, const double& s) {

  return {v.get_x() * s, v.get_y() * s, v.get_z() * s};
}

Vector3 Vector3::cross_product(const Vector3& a, const Vector3& b) {
  return {a._y * b._z - a._z * b._y, a._z * b._x - a._x * b._z, a._x * b._y - a._y * b._x};
}

double Vector3::dot_product(const Vector3& a, const Vector3& b) {
  return a._x * b._x + a._y * b._y + a._z * b._z;
}

Vector3 Vector3::copy() const {
  return {_x, _y, _z};
}

std::ostream& operator<<(std::ostream& os, const Vector3& v3) {
  os << "(" << v3._x << ", " << v3._y << ", " << v3.get_z() << ")";
  return os;
}
}
