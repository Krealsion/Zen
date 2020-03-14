#pragma once

#include "vector2.h"
#include "rectangle.h"

namespace Zen {

class Line2D {
public:
  Line2D(Vector2 p1, Vector2 p2);
  bool is_undefined() const;
  double get_slope() const;
  double get_intercept() const;

  bool check_value_in_domain(double x) const;
  double evaluate(double x) const;
  bool bounding_collision_check(Line2D o) const;

  static bool shares_domain_and_range(const Line2D& l1, const Line2D& l2);
  static bool check_lines_parallel(const Line2D& l1, const Line2D& l2);
  static Rectangle* get_shared_bounding_box(Line2D l1, Line2D l2);

private:
  double _domain_start, _domain_end;
  double _range_start, range_end;
  double _slope, _intercept;
};
}
