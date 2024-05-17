#pragma once

#include <string>
#include <timer.h>
#include "src/message_bus/var_storage.h"
#include <vector>
#include "vector2.h"
#include <random>

namespace Zen {

class Noise {
public:
  AnythingStorage<double> scale;
  AnythingStorage<double> strength;
  AnythingStorage<double> offset;
  AnythingStorage<double> spread_base; // value [0, 1] minimum value % of scale
  AnythingStorage<double> variance_base; // value [0, 1] minimum value % of strength

  std::vector<Vector2> points;
  AnythingStorage<double> last_x;

  operator double() {
    return get(last_x);
  }

  double spread(double num, double min_percent = 0) {
    double spread = num * (1 - min_percent);
    if (spread < 0) {
      spread -= min_percent;
    } else {
      spread += min_percent;
    }
    return spread;
  }

  double rand(double min_percent = 0) {
    double zero_to_one = (double) std::rand() / RAND_MAX;
    return spread(zero_to_one, min_percent);
  }

  double rand_neg_to_pos_one(double min_percent = 0) {
    double neg_one_to_one = (((double) std::rand() / RAND_MAX) - .5) * 2;
    double spread_from_zero = neg_one_to_one * (1 - min_percent);
    if (spread_from_zero < 0) {
      spread_from_zero -= min_percent;
    } else {
      spread_from_zero += min_percent;
    }
    return spread_from_zero;
  }


  void gen_to(double x) {
    if (points.empty()) {
      points.emplace_back(0, rand_neg_to_pos_one(variance_base) * strength);
    }
    double last = points[points.size() - 1].get_x();
    while (last < x) {
      double x_increase = rand(spread_base) * scale;
      double y = rand_neg_to_pos_one(variance_base) * strength;
      Vector2 p(last + x_increase, y);
      points.push_back(p);
      last = p.get_x();
    }
  }

  double get(double x) {
    x += offset;
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
