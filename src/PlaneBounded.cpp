//
// Created by jdemoss on 1/15/20.
//
#include "PlaneBounded.h"

PlaneBounded::PlaneBounded(const std::vector <Vector3>& planarPoints) {
    if (planarPoints.size() < 3)
        return;
    Vector3 normal = PlanarNormal(planarPoints[0], planarPoints[1], planarPoints[2]);
    for (int i = 3; i < planarPoints.size(); i++) {
        if (Vector3::DotProduct(planarPoints[i], normal) != 0)
            return;
    }
    BoundedPoints = planarPoints;
    Normal = normal;
}

Vector3 PlaneBounded::PlanarNormal(const Vector3& a, const Vector3& b, const Vector3& c) {
    return Vector3::CrossProduct(Vector3::Add(b, Vector3::Scale(a, -1)), Vector3::Add(c, Vector3::Scale(a, -1)));
}
