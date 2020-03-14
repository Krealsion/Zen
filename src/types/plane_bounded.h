#pragma once

#include <vector>

#include "vector3.h"

namespace Zen {

class PlaneBounded {
public:
  explicit PlaneBounded(const std::vector<Vector3>& planar_points);
  Vector3 planar_normal(const Vector3& a, const Vector3& b, const Vector3& c);
  //TODO Implement checking a point inside the Bounded Plane

private:
  std::vector<Vector3> _bounded_points;
  Vector3 _normal;
};
}

