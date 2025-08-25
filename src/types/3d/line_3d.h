#pragma once

#include "vector3.h"

namespace Zen {

class Line3D {
public:
  Line3D(Zen::Vector3 start, Zen::Vector3 end) : l0(start), ld(end - start) {}

//  double evaluate_xy(double x, double y);
//  double evaluate_xz(double x, double z);
//  double evaluate_yz(double y, double z);

private:
  Vector3 l0, ld;
};
}

