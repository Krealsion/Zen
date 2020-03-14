#pragma once

#include "vector2.h"
#include "rectangle.h"

namespace Zen {

class Line2D {
public:
  Line2D(Vector2 p1, Vector2 p2);

  double get_domain_start() const { return _domain_start; }
  double get_domain_end() const { return _domain_end; }
  double get_range_start() const { return _range_start; }
  double get_range_end() const { return _range_end; }
  double get_slope() const { return _slope; }
  double get_intercept() const { return _intercept; }

  bool is_undefined() const;
  bool check_value_in_domain(double x) const;
  double evaluate(double x) const;

  static bool shares_domain_and_range(const Line2D& l1, const Line2D& l2);
  static bool check_lines_parallel(const Line2D& l1, const Line2D& l2);
  static Rectangle* get_shared_bounding_box(Line2D l1, Line2D l2);

private:
  double _domain_start, _domain_end;
  double _range_start, _range_end;
  double _slope, _intercept;
};
}
