#pragma once

namespace Types {
class Shape;
class Triangle;
}

namespace Zen {
class Collision3D {
  bool check_do_shapes_collide(Types::Shape a, Types::Shape b);
  bool check_do_triangles_collide(Types::Triangle a, Types::Triangle b);
};
}