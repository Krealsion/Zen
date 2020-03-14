
#include "collision2d.h"

namespace Zen {

std::vector<Line2D> Collision2D::get_lines_from_points(const std::vector<Vector2>& points) {
  std::vector<Line2D> lines;
  lines.reserve(points.size());
  for (int i = 0; i < points.size(); i++) {
    lines.emplace_back(points[i], points[(i + 1) % points.size()]);
  }
  return lines;
}

std::vector<Vector2> Collision2D::get_line_collision(const Line2D& l1, const Line2D& l2) {
  std::vector<Vector2> points;
  if (!Line2D::shares_domain_and_range(l1, l2)) {
    return points;
  }
  if (Line2D::check_lines_parallel(l1, l2)) {
    return points;
  }
  if (l1.is_undefined()) {
    if (l2.is_undefined()) {
      points.emplace_back(l1.get_domain_start(), std::max(l1.get_range_start(), l2.get_range_start()));
      points.emplace_back(l1.get_domain_start(), std::min(l1.get_range_end(), l2.get_range_end()));
      return points;
    }
  }

  double x = (l2.get_intercept() - l1.get_intercept()) / (l1.get_slope() - l2.get_slope());
  if (l1.check_value_in_domain(x) && l2.check_value_in_domain(x)) {
    points.emplace_back(x, l1.evaluate(x));
  }
  return points;
}

Rectangle Collision2D::get_bounding_box(const std::vector<Vector2>& points) {  // Creates a rectangle that encompasses all given points
  Rectangle rect; // Rectangle that holds new Bounding Box
  if (points.empty()) {
    return rect;
  }
  auto min_x = points[0].get_x(); // Sets the initial values to the first point
  auto max_x = points[0].get_x();
  auto min_y = points[0].get_y();
  auto max_y = points[0].get_y();
  for (int i = 1; i < points.size(); i++) { // Goes through each point except the one all values are based on
    min_x = std::min(min_x, points[i].get_x()); // ensure correct min value
    max_x = std::max(max_x, points[i].get_x()); // ensure correct max value
    min_y = std::min(min_y, points[i].get_y());
    max_y = std::max(max_y, points[i].get_y());
  }
  rect.set_x(min_x); // Set the X position to the minimum X value found
  rect.set_y(min_y); // Set the Y position to the minimum Y value found
  rect.set_width(max_x - min_x);  // Set the width of the rectangle to the max X value found minus the min X value found
  rect.set_height(max_y - min_y); // Set the height of the rectangle to the max Y value found minus the min Y value found
  return rect; // Return the bounding Rectangle
}

bool Collision2D::bounding_box_collision_check(const Rectangle& rect1, const Rectangle& rect2) {
  if (rect1.get_x() + rect1.get_width() < rect2.get_x()) {
    return false;
  }
  if (rect2.get_x() + rect2.get_width() < rect1.get_x()) {
    return false;
  }
  if (rect1.get_y() + rect1.get_height() < rect2.get_y()) {
    return false;
  }
  if (rect2.get_y() + rect2.get_height() < rect1.get_y()) {
    return false;
  }
  return true;
}

bool Collision2D::check_advanced_collision(const std::vector<Vector2>& set_1, const std::vector<Vector2>& set_2) {
  return !get_points_of_collision(set_1, set_2).empty();
}

std::vector<Vector2> Collision2D::get_points_of_collision(const std::vector<Vector2>& set_1, const std::vector<Vector2>& set_2) {
  std::vector<Vector2> collision_points;
  std::vector<Line2D> line_set_1 = get_lines_from_points(set_1);
  std::vector<Line2D> line_set_2 = get_lines_from_points(set_2);
  if (bounding_box_collision_check(get_bounding_box(set_1), get_bounding_box(set_2))) {
    for (const auto & i : line_set_1) {
      for (const auto & j : line_set_2) {
        auto p = get_line_collision(i, j);
        collision_points.insert(collision_points.end(), p.begin(), p.end());
      }
    }
  }
  return collision_points;
}

bool Collision2D::check_point_inside_shape(const Vector2& point, const std::vector<Line2D>& lines) {
  auto inside = false;    // By default the point is not inside the Function Set
  for (const Line2D& line : lines) {     // For each line in the std::vector<Function>
    if (line.check_value_in_domain(point.get_x())) {  // Check if the point is within it's domain
      if (line.evaluate(point.get_x()) > point.get_y()) {  // If the point is also above the point
        inside = !inside;   // Toggle the "insideness" of the point
      }
    }
  }
  return inside;  // Return if the point is inside
}

bool Collision2D::bounding_collision_check(Line2D l1, Line2D l2) const {
  if (l1.get_domain_start()> l1.get_domain_end()) {
    return false;
  }
  if (l1.get_domain_end()< l2.get_domain_start()) {
    return false;
  }
  if (l1.get_range_start()> l2.get_range_end()) {
    return false;
  }
  if (l1.get_range_end()< l2.get_range_start()) {
    return false;
  }
  return true;
}
}
