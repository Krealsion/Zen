#include <utility>

#include <iostream>
#include <vector>
#include "Vector2.h"
#include "Vector3.h"

std::ostream& operator<<(std::ostream& os, Vector3& v) {
    os << "(" << v.X << ", " << v.Y << ", " << v.Z << ")";
    return os;
}
struct Function {
    double Slope, Intercept, Start, End;
    double Eval(double x) const {
        return Slope * x + Intercept;
    }
};
struct Rectangle {
    double X, Y, Width, Height;
    Rectangle() = default;
    Rectangle(double x, double y, double width, double height) {
        X = x;
        Y = y;
        Width = width;
        Height = height;
    }
};

// Data Conversion
std::vector<Function> GetFSet(std::vector<Vector2> Points);
Rectangle GetBoundingBox(std::vector<Vector2> Points);

// Collision Checks

std::vector<Function> GetFSet(std::vector<Vector2> Points) {    // This creates a set of lines for a set of given points. Line from 0-1 1-2 ... onto n-0. Loops back
    std::vector<Function> set;   // Creates an empty set of lines
    for (int i = 0; i < Points.size(); i++) {    // For each of the points
        Function f {};
        if (Points[i].X == Points[(i + 1) % Points.size()].X) {  // If both points have the same X, slope is undefined
            f.Slope = std::min(Points[i].Y,
                               Points[(i + 1) % Points.size()].Y); // Sets the Slope to the smallest Y, to be used later
            f.Intercept = std::max(Points[i].Y, Points[(i + 1) %
                                                       Points.size()].Y); // Sets the Intercept to the largest Y, to be used later
            f.Start = Points[i].X;    // Sets the Domain Start and end to the same X value to identify it is undefined
            f.End = Points[i].X;      // Sets the Domain Start and end to the same X value to identify it is undefined
        } else {  // If the line is defined we can use algebra to work out the slope and Intercept of the line
            double Slope = (Points[i].Y - Points[(i + 1) % Points.size()].Y) /
                           (Points[i].X - Points[(i + 1) % Points.size()].X);   // (y1-y2)/(x1-x2) gives slope
            f.Slope = Slope;    // Add the slope to the values. Do so separately because we use slope also to calculate intercept
            f.Intercept =
                    (Slope * Points[i].X) + Points[i].Y;  // (X0 * Slope) + Y0 gives intercept from a point and slope
            f.Start = std::min(Points[i].X,
                               Points[(i + 1) % Points.size()].X);   // set the domain Start to the min
            f.End = std::max(Points[i].X, Points[(i + 1) % Points.size()].X);     // set the domain End to the max
        }
        set.push_back(f);
    }
    return set; // Return the set of lines
}
Rectangle GetBoundingBox(std::vector<Vector2> Points) {  // Creates a rectangle that encompasses all given points
    Rectangle Rect; // Rectangle that holds new Bounding Box
    if (Points.empty()) {
        return Rect;
    }
    double MinX = Points[0].X; // Sets the initial values to the first point
    double MaxX = Points[0].X;
    double MinY = Points[0].Y;
    double MaxY = Points[0].Y;
    for (int i = 1; i < Points.size(); i++) { // Goes through each point except the one all values are based on
        MinX = std::min(MinX, Points[i].X); // ensure correct min value
        MaxX = std::max(MaxX, Points[i].X); // ensure correct max value
        MinY = std::min(MinY, Points[i].Y);
        MaxY = std::max(MaxY, Points[i].Y);
    }
    Rect.X = MinX; // Set the X position to the minimum X value found
    Rect.Y = MinY; // Set the Y position to the minimum Y value found
    Rect.Width = MaxX - MinX;  // Set the width of the rectangle to the max X value found minus the min X value found
    Rect.Height = MaxY - MinY; // Set the height of the rectangle to the max Y value found minus the min Y value found
    return Rect; // Return the bounding Rectangle
}
bool BoundingBoxCollisionCheck(Rectangle Rec1, Rectangle Rec2) {     // Checks if two boxes are possibly colliding for optimization
    if (Rec1.X + Rec1.Width < Rec2.X) {          // Checks if Rec1 is completely to the left of Rec2
        return false;   // Impossible for collision
    } else if (Rec1.Y + Rec1.Height < Rec2.Y) {   // Checks if Rec1 is completely above Rec2
        return false;   // Impossible for collision
    } else if (Rec2.X + Rec2.Width < Rec1.X) {    // Checks if Rec2 is completely to the left of Rec1
        return false;   // Impossible for collision
    } else return Rec2.Y + Rec2.Height >= Rec1.Y;
}
bool CheckPointInside(const Vector2& Point, const std::vector<Function>& Lines) {   // Checks how many lines pass above it
    bool Inside = false;    // By default the point is not inside the Function Set
    for (const Function& Line : Lines) {     // For each line in the std::vector<Function>
        if (Line.Start <= Point.X && Line.End >= Point.X) {  // Check if the point is within it's domain
            if (Line.Eval(Point.X) > Point.Y) {  // If the point is also above the point
                Inside = !Inside;   // Toggle the "insideness" of the point
            }
        }
    }
    return Inside;  // Return if the point is inside
}
static bool CheckSharesDomain(const Function& f1, const Function& f2) {
    return !(f1.Start > f2.End || f1.End < f2.Start);
}
std::vector<Vector2> GetPointsOfCollision(const std::vector<Function>& Set1, const std::vector<Function>& Set2) {  // Gets the points of collision for two given 2D sets
    std::vector<Vector2> CollisionPoints;
    for (int i = 0; i < Set1.size(); i++) {     // For each line in set 1
        for (int j = 0; i < Set2.size(); j++) { // Compare to each line in set 2
            if (std::max(Set1[i].Start, Set2[j].Start) >=
                std::min(Set1[i].End, Set2[j].End)) {   // If the lines share a domain they can collide
                if ((Set1[i].Start == Set1[i].End) && (Set2[j].Start == Set2[j].End)) {  // If both lines are undefined
                    if (std::max(Set1[i].Slope, Set2[j].Slope) >
                        std::min(Set1[i].Intercept, Set2[j].Intercept)) {    // If they share a common Y
                        CollisionPoints.emplace_back(Set1[i].Start, std::max(Set1[i].Slope,
                                                                             Set2[j].Slope));   // Add the beginning of the shared Y
                        CollisionPoints.emplace_back(Set1[i].Start, std::min(Set1[i].Intercept,
                                                                             Set2[j].Intercept));  // Add the end of the shared Y
                        continue;
                    }
                }
                if (Set1[i].Start == Set1[i].End) {    // If the first line is undefined
                    if ((Set1[i].Slope < Set2[j].Slope * Set1[i].End + Set2[j].Intercept) !=
                        // Checks if the Y-value of the undefined function is above on one side
                        (Set1[i].Intercept < Set2[j].Slope * Set1[i].End +
                                             Set2[j].Intercept)) {   // But not the other meaning it crosses at some point
                        CollisionPoints.emplace_back(Set1[i].End, Set2[j].Slope * Set1[i].End +
                                                                  Set2[j].Intercept);   // Add the point where they cross
                        continue;
                    }
                }
                if (Set2[j].Start == Set2[j].End) {  // If the second line is undefined
                    if ((Set2[j].Slope < Set1[i].Slope * Set2[j].End + Set1[i].Intercept) !=
                        // Checks if the Y-value of the undefined function is above on one side
                        (Set2[j].Intercept < Set1[i].Slope * Set2[j].End +
                                             Set1[i].Intercept)) {   // But not the other meaning it crosses at some point
                        CollisionPoints.emplace_back(Set2[j].End, Set1[i].Slope * Set2[j].End +
                                                                  Set1[i].Intercept);   // Add the point where they cross
                        continue;
                    }
                }
                if (Set1[i].Slope == Set2[j].Slope) {    // If both lines have the same slope
                    if (Set1[i].Intercept == Set2[j].Intercept) {  // If both are the same line
                        CollisionPoints.emplace_back(std::max(Set1[i].Start, Set2[j].Start),
                                                     Set1[i].Slope *
                                                     std::max(Set1[i].Start, Set2[j].Start) +
                                                     Set1[i].Intercept);
                        CollisionPoints.emplace_back(std::min(Set1[i].End, Set2[j].End),
                                                     Set1[i].Slope *
                                                     std::min(Set1[i].End, Set2[j].End) +
                                                     Set1[i].Intercept);
                        continue;
                    }
                }
                // Function vs Function Scenario
                double DomainStart = std::min(Set1[i].End, Set2[j].End); // Where the shared domain Starts
                double DomainEnd = std::max(Set1[i].Start, Set2[j].Start);    // Where the shared domain Ends
                if ((Set1[i].Eval(DomainStart) < Set2[j].Eval(DomainStart)) !=
                    // Checks if on one side of two lines shared domains
                    (Set1[i].Eval(DomainEnd) < Set2[j].Eval(
                            DomainEnd))) {  // One line is above and is below on the other aka they swap who is on top
                    CollisionPoints.emplace_back(
                            (Set2[j].Intercept - Set1[i].Intercept) / (Set1[i].Slope - Set2[j].Slope),
                            Set1[i].Slope *
                            ((Set2[j].Intercept - Set1[i].Intercept) / (Set1[i].Slope - Set2[j].Slope)) +
                            Set1[i].Intercept); // Gets the point where the two functions cross (X = (I2-I1)/(S1-S2))
                }
            }
        }
    }
    if (CollisionPoints.empty()) {   // If there were no line collisions check if one object is inside the other
        if (CheckPointInside(Vector2(Set1[0].Start, Set1[0].Slope * Set1[0].Start + Set1[0].Intercept),
                             Set2)) { // Checks if a point of the first set is within the second
            for (int i = 0; i <
                            Set1.size(); i++) { // Add each point of the first set inside the colliding points because the entire object is inside it
                CollisionPoints.emplace_back(Set1[i].Start, Set1[i].Start * Set1[i].Slope +
                                                            Set1[i].Intercept);    // Add the first point of each function
            }
        } else   // Only one object can be completely inside the other... Unless 4D I guess
        if (CheckPointInside(Vector2(Set2[0].Start, Set2[0].Slope * Set2[0].Start + Set2[0].Intercept),
                             Set1)) { // Checks if a point of the second set is within the first
            for (int i = 0; i <
                            Set2.size(); i++) { // Add each point of the second set inside the colliding points because the entire object is inside it
                CollisionPoints.emplace_back(Set2[i].Start, Set2[i].Start * Set2[i].Slope +
                                                            Set2[i].Intercept);    // Add the first point of each function
            }
        }
    }
    return CollisionPoints; // Return all the points of collision on the 2D plane
}
bool CheckAdvancedCollision(std::vector<Function> Set1, std::vector<Function> Set2) {  // Checks if two sets of line segments collide
    return !GetPointsOfCollision(std::move(Set1), std::move(Set2)).empty();
}

struct Line {
    Vector3 l0;
    Vector3 ld;
    Line(Vector3 start, Vector3 end) {
        l0 = start;
        ld = Vector3(end.X - start.X, end.Y - start.Y, end.Z - start.Z);
    }
    Vector3* evaluate(double d) const {
        return new Vector3(l0.X + d * ld.X, l0.Y + d * ld.Y, l0.Z + d * ld.Z);
    }
};
struct Plane {
    Vector3 p; // Point on the plane
    Vector3 n; // Normal Vector
    Plane(Vector3 point, Vector3 normal){
        p = point;
        n = normal;
    }
};
Vector3 CrossProduct(Vector3 a, Vector3 b) {
    return Vector3(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X);
}
double DotProduct(Vector3 a, Vector3 b) {
    return a.X * b.X + a.Y * b.Y + a.Z * b.Z;
}
Vector3 PlanarNormal(Vector3 a, Vector3 b, Vector3 c) {
    return CrossProduct(Vector3::sum(b, Vector3::scale(a, -1)), Vector3::sum(c, Vector3::scale(a, -1)));
}
std::vector<Line> GetLines(std::vector<Vector3> Points) {
    std::vector<Line> set;
    for (int i = 0; i < Points.size(); i++) {
        set.emplace_back(Points[i], Points[(i + 1) % Points.size()]);
    }
    return set;
}
Plane* getPlane(const std::vector<Vector3> &points) {
    if (points.size() < 3)
        return nullptr;
    Vector3 normal = PlanarNormal(points[0], points[1], points[2]);
    for (int i = 3; i < points.size(); i++) {
        if (DotProduct(points[i], normal) != 0)
            return nullptr;
    }
    return new Plane(points[0], normal);
}
Vector3* getLinePlaneCollision(const Plane &plane, const std::vector<Line> &lines){
    for (int i = 0; i < lines.size(); i++) {
        if (DotProduct(lines[i].ld, plane.n) == 0 ){
            if ((DotProduct(Vector3::sum(plane.p, Vector3::scale(lines[i].l0, -1)), plane.n)) == 0){
                return nullptr;
            }
            return nullptr; // TODO check for shared linespace
        }
        double d = DotProduct(Vector3::sum(plane.p, Vector3::scale(lines[i].l0, -1)), plane.n) /
        DotProduct(lines[i].ld, plane.n);
        if (d >= 0 && d <= 1) { // Collision happened in our lifetime!
            return lines[i].evaluate(d);
        }
    }
}
std::vector<Vector3> GetPointsOfCollision3D(const std::vector<Vector3>& Set1, const std::vector<Vector3>& Set2) {  // Gets the points of collision for two given 3D sets
    std::vector<Line> lines1 = GetLines(Set1);
    std::vector<Line> lines2 = GetLines(Set2);
    Plane* plane1 = getPlane(Set1);
    Plane* plane2 = getPlane(Set2);

}

int main() {
    Vector3 a = Vector3(0,0,0);
    Vector3 b = Vector3(4, 4, 0);
    Vector3 c = Vector3(4, 4, 4);
    std::vector<Vector3> v = std::vector<Vector3>();
    v.push_back(a);
    v.push_back(b);
    v.push_back(c);
    Plane *plane = getPlane(v);
    std::vector<Line> testLines = std::vector<Line>();
    testLines.emplace_back(Vector3(1,0,0), Vector3(1, 1, 2));
    std::cout << *getLinePlaneCollision(*plane, testLines) << std::endl;
    return 0;
}
