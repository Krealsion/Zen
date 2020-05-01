#include "line_2d.h"

namespace Zen {

Line2D::Line2D(Vector2 p1, Vector2 p2) {
  if (p1.get_x() != p2.get_x()) {
    _slope = (p2.get_y() - p1.get_y()) / (p2.get_x() - p1.get_x());
    _intercept = p1.get_y() - p1.get_x() * _slope;
  }
  _range_start = std::min(p1.get_y(), p2.get_y());
  _range_end = std::max(p1.get_y(), p2.get_y());
  _domain_start = std::min(p1.get_x(), p2.get_x());
  _domain_end = std::max(p1.get_x(), p2.get_x());
}
bool Line2D::is_undefined() const {
  return _domain_start == _domain_end;
}
double Line2D::evaluate(double x) const {
  if (is_undefined()) {
    return _range_end;
  }
  return x * _slope + _intercept;
}
bool Line2D::shares_domain_and_range(const Line2D& l1, const Line2D& l2) {
  if (l1._domain_start > l2._domain_end) {
    return false;
  }
  if (l1._domain_end < l2._domain_start) {
    return false;
  }
  if (l1._range_start > l2._range_end) {
    return false;
  }
  if (l1._range_end < l2._range_start) {
    return false;
  }
  return true;
}
bool Line2D::check_value_in_domain(double x) const {
  if (x < _domain_start) {
    return false;
  }
  if (x > _domain_end) {
    return false;
  }
  return true;
}
bool Line2D::check_lines_parallel(const Line2D& l1, const Line2D& l2) {
  return l1._slope == l2._slope;
}
Rectangle* Line2D::get_shared_bounding_box(Line2D l1, Line2D l2) {
  double min_x = std::max(l1._domain_start, l2._domain_start);
  double max_x = std::min(l1._domain_end, l2._domain_end);
  double min_y = std::max(l1._range_start, l2._range_start);
  double ma_x_y = std::min(l1._range_end, l2._range_end);
  if (min_x > max_x) {
    return nullptr;
  }
  if (min_y > ma_x_y) {
    return nullptr;
  }
  return new Rectangle(min_x, max_x, min_y, ma_x_y);
}

}
