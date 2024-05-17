#pragma once

#include "camera.h"
#include "vector2.h"
#include "vector3.h"

namespace Zen {

class Engine3D {
public:
  Engine3D() = default;
  ~Engine3D() = default;

  static Vector2 get_screen_coords(const Vector3& point, const Camera& c, double screen_size);

private:

  static Vector3 _rotate_all(const Vector3& p, double pitch, double yaw, double roll);
  static Vector3 _rotate_xy(const Vector3& p, double yaw);
  static Vector3 _rotate_xz(const Vector3& p, double pitch);
  static Vector3 _rotate_yz(const Vector3& p, double roll);
};
}