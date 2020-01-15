//
// Created by jdemoss on 1/15/20.
//

#ifndef LEAVINGTERRA_LINE2D_H
#define LEAVINGTERRA_LINE2D_H

#include "Vector2.h"

class Line2D {
public:
    Line2D(Vector2 p1, Vector2 p2);
    double Evaluate(double x);

private:
    double DomainStart, DomainEnd;
    double RangeStart, RangeEnd;
    double Slope, Intercept;
};

#endif //LEAVINGTERRA_LINE2D_H
