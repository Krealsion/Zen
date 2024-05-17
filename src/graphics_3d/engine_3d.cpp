#include "engine_3d.h"

#include "logic/math.h"

namespace Zen {
Vector2 Engine3D::get_screen_coords(const Vector3& point, const Camera& c, double screen_size) {
  Vector3 CamPos = c.get_pos();
  Vector3 OriginP = Vector3(point.get_x() - CamPos.get_x(), point.get_y() - CamPos.get_y(), point.get_z() - CamPos.get_z());
  OriginP = _rotate_all(OriginP, c.get_pitch(), c.get_yaw(), c.get_roll());
  double WidthAtX = std::abs(c.get_tan_half_fov() * OriginP.get_x());
  if (OriginP.get_x() <= 0) {
    WidthAtX = .00001;
  }
  double PercX = .5 + (OriginP.get_y() / WidthAtX);
  double PercY = .5 - (OriginP.get_z() / WidthAtX);
  return {PercX * screen_size, PercY * screen_size};
}

Vector3 Engine3D::_rotate_all(const Vector3& p, double pitch, double yaw, double roll){
  return _rotate_yz(_rotate_xz(_rotate_xy(p, yaw), pitch), roll);
}

Vector3 Engine3D::_rotate_xy(const Vector3& p, double yaw) {
  double si = Math::FastSin(yaw);
  double co = Math::FastCos(yaw);
  return {p.get_x() * co - p.get_y() * si, p.get_y() * co + p.get_x() * si, p.get_z()};
}

Vector3 Engine3D::_rotate_xz(const Vector3& p, double pitch) {
  double si = Math::FastSin(pitch);
  double co = Math::FastCos(pitch);
  return {p.get_x() * co - p.get_z() * si, p.get_y(), p.get_z() * co + p.get_x() * si};
}

Vector3 Engine3D::_rotate_yz(const Vector3& p, double roll) {
  double si = Math::FastSin(roll);
  double co = Math::FastCos(roll);
  return {p.get_x(), p.get_y() * co - p.get_z() * si, p.get_z() * co + p.get_y() * si};
}
}