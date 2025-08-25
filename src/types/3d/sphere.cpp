#include "sphere.h"


#include <math.h>

namespace Zen {
Sphere::Sphere(Vector3 center, double radius) : center(center), radius(radius) {

}

void Sphere::generate_sphere() {

}

std::vector<Vector3> Sphere::generate_points(int recursionLevel) {
    std::vector<Vector3> points;

    float t = (1.0 + std::sqrt(5.0)) / 2.0;

    // Define 12 vertices of icosahedron
    points.emplace_back(-1,  t, 0);     // 0
    points.emplace_back( 1,  t, 0);     // 1
    points.emplace_back(-1, -t, 0);     // 2
    points.emplace_back( 1, -t, 0);     // 3

    points.emplace_back(0, -1,  t);     // 4
    points.emplace_back(0,  1,  t);     // 5
    points.emplace_back(0, -1, -t);     // 6
    points.emplace_back(0,  1, -t);     // 7

    points.emplace_back( t, 0, -1);     // 8
    points.emplace_back( t, 0,  1);     // 9
    points.emplace_back(-t, 0, -1);     // 10
    points.emplace_back(-t, 0,  1);     // 11

    /**
     *
     * (-1,  t, 0)
     * ( 1,  t, 0)
     * (-1, -t, 0)
     * ( 1, -t, 0)
     *
     * (0, -1,  t)
     * (0,  1,  t)
     * (0, -1, -t)
     * (0,  1, -t)
     *
     * ( t, 0, -1)
     * ( t, 0,  1)
     * (-t, 0, -1)
     * (-t, 0,  1)
     *
     */

    // Normalize the 12 vertices to be on the sphere
    for (auto& point : points) {
        float length = std::sqrt(point.get_x() * point.get_x() + point.get_y() * point.get_y() + point.get_z() * point.get_z());
        point.set_x(radius * point.get_x() / length);
        point.set_y(radius * point.get_y() / length);
        point.set_z(radius * point.get_z() / length);
    }

    std::vector<Triangle> triangles;

    auto getMiddleVector3 = [](Vector3 point1, Vector3 point2) {
        Vector3 middle = Vector3(
            (point1.get_x() + point2.get_x()) / 2.0,
            (point1.get_y() + point2.get_y()) / 2.0,
            (point1.get_z() + point2.get_z()) / 2.0
        );
        return middle;
    };

    // Subdivide the triangles to get more points
    for (int i = 0; i < recursionLevel; ++i) {
        for (const auto& triangle : triangles) {
            Vector3 a = getMiddleVector3(triangle.p1, triangle.p2);
            Vector3 b = getMiddleVector3(triangle.p2, triangle.p3);
            Vector3 c = getMiddleVector3(triangle.p3, triangle.p1);

            triangles.emplace_back(triangle.p1, a, c);
            triangles.emplace_back(triangle.p2, b, a);
            triangles.emplace_back(triangle.p3, c, b);
            triangles.emplace_back(a, b, c);
        }

        points.clear(); // Clear the original points
    }

    return points;
}
}