//
// Created by jdemoss on 1/15/20.
//

#ifndef LEAVINGTERRA_PLANEBOUNDED_H
#define LEAVINGTERRA_PLANEBOUNDED_H

#include <vector>

#include "Vector3.h"

class PlaneBounded {
public:
    PlaneBounded(const std::vector<Vector3>& planarPoints);
    //TODO Implement checking a point inside the Bounded Plane

private:
    Vector3 PlanarNormal(const Vector3& a, const Vector3& b, const Vector3& c);
    std::vector<Vector3> BoundedPoints;
    Vector3 Normal;
};

#endif //LEAVINGTERRA_PLANEBOUNDED_H
