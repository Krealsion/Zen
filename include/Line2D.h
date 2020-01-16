//
// Created by jdemoss on 1/15/20.
//

#ifndef LEAVINGTERRA_LINE2D_H
#define LEAVINGTERRA_LINE2D_H

#include "Vector2.h"

class Line2D {
public:
    Line2D(Vector2 p1, Vector2 p2);
    double GetSlope() const;
    double GetIntercept() const;

    bool CheckValueInDomain(double x);
    double Evaluate(double x) const;
    bool BoundingCollisionCheck(Line2D o) const;

    static bool SharesDomainRange(const Line2D& l1, const Line2D& l2);
    static bool CheckLinesParallel(const Line2D& l1, const Line2D& l2);

private:
    double DomainStart, DomainEnd;
    double RangeStart, RangeEnd;
    double Slope, Intercept;
};

#endif //LEAVINGTERRA_LINE2D_H
