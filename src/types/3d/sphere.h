#pragma once

#include "vector3.h"
#include "triangle.h"

#include <vector>

namespace Zen {
class Sphere {
public:
  Sphere(Vector3 center, double radius = 1);
  ~Sphere() = default;

private:
    void generate_sphere();
    std::vector<Vector3> generate_points(int recursion_level = 3);

    Vector3 center;
    double radius;
    std::vector<Triangle> triangles;
};
}