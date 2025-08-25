#pragma once

#include "vector3.h"

namespace Zen {

class Triangle {
public:
  Triangle(Vector3 p1, Vector3 p2, Vector3 p3) : p1(p1), p2(p2), p3(p3) {}

  Vector3 p1, p2, p3;
};

class TriangleRef {
public:
    TriangleRef(Vector3& p1, Vector3& p2, Vector3& p3) : p1(&p1), p2(&p2), p3(&p3) {}

    Vector3 *p1, *p2, *p3;
};
}
