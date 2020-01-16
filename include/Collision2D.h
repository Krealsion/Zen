//
// Created by jdemoss on 1/15/20.
//

#ifndef LEAVINGTERRA_COLLISION2D_H
#define LEAVINGTERRA_COLLISION2D_H

#include <vector>

#include "Rectangle.h"
#include "Line2D.h"



class Collision2D {
public:
    /**
     * Converts a set of points into the Line2D class from p[n] to p[(n+1) % size]
     * @param points The list of points to turn into a 2D shape
     * @return The lines that the points form
     */
    std::vector<Line2D> GetLinesFromPoints(std::vector<Vector2> points);
    /**
     * This determines if two lines collide with eachother
     * @param l1
     * @param l2
     * @return
     */
    Vector2* GetLineCollision(const Line2D& l1, const Line2D& l2);
    /**
     * This determines if two rectangles collide with eachother.
     * This is usually used to speed up collisions by using bounding boxes
     * @param rect1
     * @param rect2
     * @return
     */
    bool BoundingBoxCollisionCheck(const Rectangle& rect1, const Rectangle& rect2);
    /**
     * This does the actual determining of if two 2D shapes collide with eachother
     * @param Set1
     * @param Set2
     * @return
     */
    bool CheckAdvancedCollision(std::vector<Line2D> Set1, std::vector<Line2D> Set2);
    /**
     * This determines if a point in contained inside of a shape,
     * useful for determining if a shape is completely inside the other as well.
     * @param Point
     * @param Lines
     * @return
     */
    bool CheckPointInside(const Vector2& Point, const std::vector<Line2D>& Lines);
};

#endif //LEAVINGTERRA_COLLISION2D_H
