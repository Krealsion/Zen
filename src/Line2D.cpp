//
// Created by jdemoss on 1/15/20.
//

#include "Line2D.h"

Line2D::Line2D(Vector2 p1, Vector2 p2) {
    Slope = (p2.GetY() - p1.GetY()) / (p2.GetX() - p1.GetX());
    Intercept = p1.GetY() - p1.GetX() * Slope;
    RangeStart = std::min(p1.GetY(), p2.GetY());
    RangeEnd = std::max(p1.GetY(), p2.GetY());
    DomainStart = std::min(p1.GetX(), p2.GetX());
    DomainEnd = std::max(p1.GetX(), p2.GetX());
}
double Line2D::Evaluate(double x) {
    return x * Slope + Intercept;
}

