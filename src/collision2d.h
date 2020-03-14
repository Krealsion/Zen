#pragma once

#include <vector>

#include "rectangle.h"
#include "line2d.h"

namespace Zen {

class Collision2D {
public:
  /**
   * Converts a set of points into the Line2D class from p[n] to p[(n+1) % size]
   * @param points The list of points to turn into a 2D shape
   * @return The lines that the points form
   */
  std::vector<Line2D> get_lines_from_points(const std::vector<Vector2>& points);
  /**
   * This determines if two lines collide with eachother
   * @param l1
   * @param l2
   * @return
   */
  std::vector<Vector2> get_line_collision(const Line2D& l1, const Line2D& l2);
  Rectangle get_bounding_box(const std::vector<Vector2>& points);
  /**
   * This determines if two rectangles collide with eachother.
   * This is usually used to speed up collisions by using bounding boxes
   * @param rect1
   * @param rect2
   * @return
   */
  bool bounding_box_collision_check(const Rectangle& rect1, const Rectangle& rect2);
  /**
   * This gets an array of points that correspond to where the intersection of two 2DShapes occured
   * @param set_1
   * @param set_2
   * @return
   */
  std::vector<Vector2> get_points_of_collision(const std::vector<Vector2>& set_1, const std::vector<Vector2>& set_2);
  /**
   * This does the actual determining of if two 2D shapes collide with eachother
   * @param set_1
   * @param set_2
   * @return
   */
  bool check_advanced_collision(const std::vector<Vector2>& set_1, const std::vector<Vector2>& set_2);
  /**
   * This determines if a point in contained inside of a shape,
   * useful for determining if a shape is completely inside the other as well.
   * @param point
   * @param lines
   * @return
   */
  bool check_point_inside_shape(const Vector2& point, const std::vector<Line2D>& lines);
  bool bounding_collision_check(Line2D l1, Line2D l2) const;
};
}

