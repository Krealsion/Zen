//
// Created by jdemoss on 1/15/20.
//

#include "Collision2D.h"

std::vector<Line2D> Collision2D::GetLinesFromPoints(std::vector<Vector2> points) {
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
