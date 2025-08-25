#pragma once

#include "vector3.h"

#include <vector>

namespace Zen {
class Shape {
public:
  void connect(Vector3 a, Vector3 b);

private:
  std::vector<Vector3> _points;
  std::vector<std::tuple<int, int>> _edges;
};
} // Zen