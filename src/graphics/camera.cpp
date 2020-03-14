#include "camera.h"

#include <cmath>

namespace Zen {

Camera::Camera() {
  _position = Vector3(0, 0, 0);
  _pitch = 0;
  _yaw = 0;
  _roll = 0;
  _fov = M_PI * 2 / 3;
  tan_half_fov = tan(_fov / 2);
}

Camera::Camera(Vector3 position, double pitch, double yaw, double roll) {
  _position = position;
  _pitch = pitch;
  _yaw = yaw;
  _roll = roll;
  _fov = M_PI * 2 / 3;
  tan_half_fov = tan(_fov / 2);
}

void Camera::set_position(Vector3 position) {
  _position = position;
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
  fov = fov;
  tan_half_fov = tan(fov / 2);
}

Vector3 Camera::get_position() {
    return _position;
}

double Camera::get_pitch() {
    return _pitch;
}

double Camera::get_yaw() {
    return _yaw;
}

double Camera::get_roll() {
    return _roll;
}

double Camera::get_fov() {
    return _fov;
}

double Camera::get_tan_half_fov() {
    return tan_half_fov;
}
}
