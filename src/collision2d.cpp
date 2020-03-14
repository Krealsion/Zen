
#include "collision2d.h"

namespace Zen {

std::vector<Line2D> Collision2D::GetLinesFromPoints(const std::vector<Vector2>& points) {
  std::vector<Line2D> lines;
  for (int i = 0; i < points.size(); i++) {
    lines.emplace_back(points[i], points[(i + 1) % points.size()]);
  }
  return lines;
}
Vector2* Collision2D::GetLineCollision(const Line2D& l1, const Line2D& l2) {
  if (!Line2D::shares_domain_and_range(l1, l2)) {
    return nullptr;
  }
  if (Line2D::check_lines_parallel(l1, l2)) {
    return nullptr;
  }
  if (l1.is_undefined()) {
    if (l2.is_undefined()) {
      // TODO get the exact start and end point of the shared undefined line
//            Rectangle* Line2D::
      return new Vector2[2]{};
    }
  }

  double x = (l2.get_intercept() - l1.get_intercept()) / (l1.get_slope() - l2.get_slope());
  if (l1.check_value_in_domain(x) && l2.check_value_in_domain(x)) {
    return new Vector2(x, l1.evaluate(x));
  }
  return nullptr;
}
Rectangle Collision2D::GetBoundingBox(const std::vector<Vector2>& points) {  // Creates a rectangle that encompasses all given points
  Rectangle Rect; // Rectangle that holds new Bounding Box
  if (points.empty()) {
    return Rect;
  }
  double MinX = points[0].get_x(); // Sets the initial values to the first point
  double MaxX = points[0].get_x();
  double MinY = points[0].get_y();
  double MaxY = points[0].get_y();
  for (int i = 1; i < points.size(); i++) { // Goes through each point except the one all values are based on
    MinX = std::min(MinX, points[i].get_x()); // ensure correct min value
    MaxX = std::max(MaxX, points[i].get_x()); // ensure correct max value
    MinY = std::min(MinY, points[i].get_y());
    MaxY = std::max(MaxY, points[i].get_y());
  }
  Rect.set_x(MinX); // Set the X position to the minimum X value found
  Rect.set_y(MinY); // Set the Y position to the minimum Y value found
  Rect.set_width(MaxX - MinX);  // Set the width of the rectangle to the max X value found minus the min X value found
  Rect.set_height(MaxY - MinY); // Set the height of the rectangle to the max Y value found minus the min Y value found
  return Rect; // Return the bounding Rectangle
}
bool Collision2D::BoundingBoxCollisionCheck(const Rectangle& rect1, const Rectangle& rect2) {
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
bool Collision2D::CheckAdvancedCollision(const std::vector<Vector2>& set1, const std::vector<Vector2>& set2) {
  return !GetPointsOfCollision(set1, set2).empty();
}
std::vector<Vector2*> Collision2D::GetPointsOfCollision(const std::vector<Vector2>& set1, const std::vector<Vector2>& set2) {
  std::vector<Vector2*> collisionPoints;
  std::vector<Line2D> lineSet1 = GetLinesFromPoints(set1);
  std::vector<Line2D> lineSet2 = GetLinesFromPoints(set2);
  if (BoundingBoxCollisionCheck(GetBoundingBox(set1), GetBoundingBox(set2))) {
    for (const auto & i : lineSet1) {
      for (const auto & j : lineSet2) {
        Vector2* p = GetLineCollision(i, j);
        if (p != nullptr) {
          collisionPoints.push_back(p);
        }
      }
    }
  }
  return collisionPoints;
}
bool Collision2D::CheckPointInside(const Vector2& point, const std::vector<Line2D>& lines) {
  bool Inside = false;    // By default the point is not inside the Function Set
  for (const Line2D& Line : lines) {     // For each line in the std::vector<Function>
    if (Line.check_value_in_domain(point.get_x())) {  // Check if the point is within it's domain
      if (Line.evaluate(point.get_x()) > point.get_y()) {  // If the point is also above the point
        Inside = !Inside;   // Toggle the "insideness" of the point
      }
    }
  }
  return Inside;  // Return if the point is inside
}
}
