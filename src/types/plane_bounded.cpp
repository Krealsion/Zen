#include "plane_bounded.h"

namespace Zen {

PlaneBounded::PlaneBounded(const std::vector<Vector3>& planar_points) {
  if (planar_points.size() < 3)
    return;
  Vector3 normal = planar_normal(planar_points[0], planar_points[1], planar_points[2]);
  for (int i = 3; i < planar_points.size(); i++) {
    if (Vector3::dot_product(planar_points[i], normal) != 0)
      return;
  }
  _bounded_points = planar_points;
  _normal = normal;
}

Vector3 PlaneBounded::planar_normal(const Vector3& a, const Vector3& b, const Vector3& c) {
  return Vector3::cross_product(Vector3::add(b, Vector3::scale(a, -1)), Vector3::add(c, Vector3::scale(a, -1)));
}
}
