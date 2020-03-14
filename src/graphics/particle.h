#pragma once

#include <vector2.h>
#include <color.h>

namespace Zen {
struct ParticleContructor {
  Vector2 position;
  Vector2 movement;
  Color color;
  bool emission;
  float emission_strength;
  Color emission_color;
};

class Particle {
public:


};
}
