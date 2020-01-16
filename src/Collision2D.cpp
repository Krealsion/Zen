//
// Created by jdemoss on 1/15/20.
//

#include "Collision2D.h"

std::vector<Line2D> Collision2D::GetLinesFromPoints(const std::vector<Vector2>& points) {
    std::vector<Line2D> lines;
    for (int i = 0; i < points.size(); i++){
        lines.emplace_back(points[i], points[(i + 1) % points.size()]);
    }
    return lines;
}
Vector2* Collision2D::GetLineCollision(const Line2D& l1, const Line2D& l2) {
    if (Line2D::SharesDomainRange(l1, l2)){
        if (Line2D::CheckLinesParallel(l1, l2)){
            return nullptr;
        }
        double x = (l2.GetIntercept() - l1.GetIntercept()) / (l1.GetSlope() - l2.GetSlope());
        if (l1.CheckValueInDomain(x) && l2.CheckValueInDomain(x)) {
            return new Vector2(x, l1.Evaluate(x));
        }
    }
    return nullptr;
}
Rectangle Collision2D::GetBoundingBox(const std::vector<Vector2>& Points) {  // Creates a rectangle that encompasses all given points
    Rectangle Rect; // Rectangle that holds new Bounding Box
    if (Points.empty()) {
        return Rect;
    }
    double MinX = Points[0].GetX(); // Sets the initial values to the first point
    double MaxX = Points[0].GetX();
    double MinY = Points[0].GetY();
    double MaxY = Points[0].GetY();
    for (int i = 1; i < Points.size(); i++) { // Goes through each point except the one all values are based on
        MinX = std::min(MinX, Points[i].GetX()); // ensure correct min value
        MaxX = std::max(MaxX, Points[i].GetX()); // ensure correct max value
        MinY = std::min(MinY, Points[i].GetY());
        MaxY = std::max(MaxY, Points[i].GetY());
    }
    Rect.SetX(MinX); // Set the X position to the minimum X value found
    Rect.SetY(MinY); // Set the Y position to the minimum Y value found
    Rect.SetWidth(MaxX - MinX);  // Set the width of the rectangle to the max X value found minus the min X value found
    Rect.SetHeight(MaxY - MinY); // Set the height of the rectangle to the max Y value found minus the min Y value found
    return Rect; // Return the bounding Rectangle
}
bool Collision2D::BoundingBoxCollisionCheck(const Rectangle& rect1, const Rectangle& rect2) {
    if (rect1.GetX() + rect1.GetWidth() < rect2.GetX()) {
        return false;
    }
    if (rect2.GetX() + rect2.GetWidth() < rect1.GetX()) {
        return false;
    }
    if (rect1.GetY() + rect1.GetHeight() < rect2.GetY()) {
        return false;
    }
    if (rect2.GetY() + rect2.GetHeight() < rect1.GetY()) {
        return false;
    }
    return true;
}
bool Collision2D::CheckAdvancedCollision(const std::vector<Vector2>& Set1, const std::vector<Vector2>& Set2) {
    if (BoundingBoxCollisionCheck(GetBoundingBox(Set1), GetBoundingBox(Set2))){

    }
}
bool Collision2D::CheckPointInside(const Vector2& Point, const std::vector<Line2D>& Lines) {
    return false;
}
