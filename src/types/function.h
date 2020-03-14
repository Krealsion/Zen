#pragma once

#include <string>
#include <timer.h>
#include <anything_storage.h>
#include <vector>
#include "vector2.h"
#include <random>

namespace Zen {
class Noise {
public:
  AnythingStorage<double> scale;
  AnythingStorage<double> strength;

  std::vector<Vector2> points;

  AnythingStorage<double> last_x;


  Noise() {
    last_x.set_c(0);
  }

  operator double() {
    return get(last_x);
  }

  void gen_to(double x) {
    if (points.empty()) {
      points.emplace_back(0, ((double) rand() / (RAND_MAX) - .5) * strength * 2);
    }
    double last = points[points.size() - 1].get_x();
    while (last < x) {
      double x_increase = ((double) rand() / (RAND_MAX)) * scale;
      double y = ((double) rand() / (RAND_MAX) - .5) * strength * 2;
      Vector2 p(last + x_increase, y);
      points.push_back(p);
      last = p.get_x();
    }
  }

  double get(double x) {
    gen_to(x);
    for (long i = points.size() - 1; i > 0; i--) {
      if (points[i].get_x() < x && points[i + 1].get_x() > x) {
        Vector2 p1 = points[i];
        Vector2 p2 = points[i + 1];
        double slope = (p1.get_y() - p2.get_y()) / (p1.get_x() - p2.get_x());
        double intercept = p1.get_y() - slope * p1.get_x();
        return slope * x + intercept;
      }
    }
    return 0.0;
  }
};
}
