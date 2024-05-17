#include "camera.h"

#include "logic/math.h"

#include <cmath>

namespace Zen {
Camera::Camera() {
  _pos = Vector3(0, 0, 0);
  _pitch = 0;
  _yaw = 0;
  _roll = 0;
  _fov = Math::PI * 2 / 3; // 120 degrees
  _tan_half_fov = std::tan(_fov / 2);
}

Camera::Camera(Vector3 pos, double pitch, double yaw, double roll) {
  _pos = pos;
  _pitch = pitch;
  _yaw = yaw;
  _roll = roll;
  _fov = Math::PI * 2 / 3;
  _tan_half_fov = std::tan(_fov / 2);
}

void Camera::set_position(Vector3 pos) {
  _pos = pos;
}

void Camera::set_pitch(double pitch) {
  _pitch = pitch;
}

void Camera::set_yaw(double yaw) {
  _yaw = yaw;
}

void Camera::set_roll(double roll) {
  _roll = roll;
}

void Camera::set_fov(double fov) {
  _fov = fov;
  _tan_half_fov = std::tan(_fov / 2);
}

Vector3 Camera::get_pos() const{
  return _pos;
}

double Camera::get_pitch() const {
  return _pitch;
}

double Camera::get_yaw() const {
  return _yaw;
}

double Camera::get_roll() const {
  return _roll;
}

double Camera::get_fov() const {
  return _fov;
}

double Camera::get_tan_half_fov() const {
  return _tan_half_fov;
}
}
