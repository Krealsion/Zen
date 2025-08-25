#include <iostream>
#include <vector>

struct Function {
    float Slope, Intercept, DomainStart, DomainEnd;
};
struct FSet {
    std::vector<Function> Functions;
};
struct Vector3 {
    double X, Y, Z;
};
struct Vector2 {
    double X, Y;
};
struct Rectangle {
    double X, Y, Width, Height;
};

// Data Conversion
FSet GetFSet(std::vector <Vector2> Points);
Rectangle GetBoundingBox(std::vector <Vector2> Points);

// Collision Checks
bool BoundingBoxCollisionCheck(Rectangle Rec1, Rectangle Rec2);
bool CheckPointInside(Vector2 Point, FSet Lines);
bool CheckAdvancedCollision(FSet Set1, FSet Set2);

std::vector <Vector2> GetVector2YZ(std::vector <Vector3> Points) { // Converts Vector3s into Vector2s only using two of the X,Y,Z points : Y+Z
    std::vector <Vector2> NewPoints; // The set of new points made from the given set
    for (int i = 0; i < Points.size(); i++) {    // For each 3D point given
        Vector2 temp;   // Create a new 2D point
        temp.X = Points[i].Y;   // Set the X value to the given Y value
        temp.Y = Points[i].Z;   // Set the Y value to the given Z value
        NewPoints.push_back(temp);  // Add the new point to the set to be returned
    }
    return NewPoints;   // Return the new set of points
}

FSet GetFSet(std::vector <Vector2> Points) {    // This creates a set of lines for a set of given points. Line from 0-1 1-2 ... onto n-0. Loops back
    FSet Set;   // Creates an empty set of lines
    for (int i = 0; i < Points.size(); i++) {    // For each of the points
        if (Points[i].X ==
            Points[(i + 1) % Points.size()].X) {  // If both points have the same X, the slope must be undefined
            Set.Slopes.push_back(std::min(Points[i].Y, Points[(i + 1) %
                                                              Points.size()].Y)); // Sets the Slope to the smallest Y, to be used later
            Set.Intercepts.push_back(std::max(Points[i].Y, Points[(i + 1) %
                                                                  Points.size()].Y)); // Sets the Intercept to the largest Y, to be used later
            Set.DomainStarts.push_back(
                    Points[i].X);    // Sets the Domain start and end to the same X value to identify it is undefined
            Set.DomainEnds.push_back(
                    Points[i].X);      // Sets the Domain start and end to the same X value to identify it is undefined
            continue;   // This two point set is done. Continue
        } else {  // If the line is defined we can use algebra to work out the slope and Intercept of the line
            double Slope = (Points[i].Y - Points[(i + 1) % Points.size()].Y) /
                           (Points[i].X - Points[(i + 1) % Points.size()].X);   // (y1-y2)/(x1-x2) gives slope
            Set.Slopes.push_back(
                    Slope);    // Add the slope to the values. Do so seperately because we use slope also to calculate intercept
            Set.Intercepts.push_back(
                    (Slope * Points[i].X) + Points[i].Y);  // (X0 * Slope) + Y0 gives intercept from a point and slope
            Set.DomainStarts.push_back(
                    std::min(Points[i].X, Points[(i + 1) % Points.size()].X));   // Set the domain Start to the min
            Set.DomainEnds.push_back(
                    std::max(Points[i].X, Points[(i + 1) % Points.size()].X));     // Set the domain End to the max
        }
    }
    return Set; // Return the set of lines
}

Rectangle GetBoundingBox(std::vector <Vector2> Points) {  // Creates a rectangle that encompasses all given points
    Rectangle Rect; // Rectangle that holds new Bounding Box
    double MinX = Points[0].X;  // Sets the minimum X value to the X of a point
    double MaxX = Points[0].X;  // Sets the maximum X value to the X of a point
    double MinY = Points[0].Y;  // Sets the minimum Y value to the Y of a point
    double MaxY = Points[0].Y;  // Sets the maximum Y value to the Y of a point
    for (int i = 1; i < Points.size(); i++) {    // Goes through each point except the one all values are based on
        MinX = std::min(MinX, Points[i].X);     // if the X value of this point is lower then the min, set the min to it
        MaxX = std::max(MaxX,
                        Points[i].X);     // if the X value of this point is higher then the max, set the max to it
        MinY = std::min(MinY, Points[i].Y);     // if the Y value of this point is lower then the min, set the min to it
        MaxY = std::max(MaxY,
                        Points[i].Y);     // if the Y value of this point is higher then the max, set the max to it
    }
    Rect.X = MinX;  // Set the X position to the minimum X value found
    Rect.Y = MinY;  // Set the Y position to the minimum Y value found
    Rect.Width = MaxX - MinX;   // Set the width of the rectangle to the max X value found minus the min X value found
    Rect.Height = MaxY - MinY;  // Set the height of the rectangle to the max Y value found minus the min Y value found
    return Rect;    // Return the bounding Rectangle
}

bool BoundingBoxCollisionCheck(Rectangle Rec1, Rectangle Rec2) {     // Checks if two boxes are possibly colliding for optimization
    if (Rec1.X + Rec1.Width < Rec2.X) {          // Checks if Rec1 is completely to the left of Rec2
        return false;   // Impossible for collision
    } else if (Rec1.Y + Rec1.Height < Rec2.Y) {   // Checks if Rec1 is completely above Rec2
        return false;   // Impossible for collision
    } else if (Rec2.X + Rec2.Width < Rec1.X) {    // Checks if Rec2 is completely to the left of Rec1
        return false;   // Impossible for collision
    } else if (Rec2.Y + Rec2.Height < Rec1.Y) {  // Checks if Rec2 is completely above Rec1
        return false;   // Impossible for collision
    } else {
        return true;    // Possible for a collision
    }
}

bool CheckPointInside(Vector2 Point, FSet Lines) {   // Checks how many lines pass above it
    bool Inside = false;    // By default the point is not inside the Function Set
    for (int i = 0; i < FSet.DomainStarts.size(); i++) {     // For each line in the FSet
        if (FSet.DomainStarts[i] <= Point.X &&
            FSet.DomainEnds[i] >= Point.X) {  // Check if the point is within it's domain
            if (FSet.Slopes[i] * Point.X + FSet.Intercepts > Point.Y) {  // If the point is also above the point
                Inside = !Inside;   // Toggle the "insideness" of the point
            }
        }
    }
    return Inside;  // Return if the point is inside
}

bool CheckAdvancedCollision(FSet Set1, FSet Set2) {  // Checks if two sets of line segments collide
    for (int i = 0; i < Set1.DomainStarts.size(); i++) {     // For each line in set 1
        for (int j = 0; i < Set2.DomainStarts.size(); j++) { // Compare to each line in set 2
            if (std::max(Set1.DomainStarts[i], Set2.DomainStarts[j]) >=
                std::min(Set1.DomainEnds[i], Set2.DomainEnds[j])) {   // If the lines share a domain they can collide
                if ((Set1.DomainStarts[i] == Set1.DomainEnds[i]) &&
                    (Set2.DomainStarts[j] == Set2.DomainEnds[j])) {  // If both lines are undefined
                    if (std::max(Set1.Slope[i], Set2.Slopes[j]) >
                        std::min(Set1.Intercepts[i], Set2.Intercepts[j])) {    // If they share a common Y
                        return true;    // Collision Occurred!
                    }
                } else if (Set1.DomainStarts[i] == Set1.DomainEnds[i]) {    // If the first line is undefined
                    if ((Set1.Slopes[i] < Set2.Slopes[j] * Set1.DomainEnds[i] + Set2.Intercepts[j]) !=
                        // Checks if the Y-value of the undefined function is above on one side
                        (Set1.intercepts[i] < Set2.Slopes[j] * Set1.DomainEnds[i] +
                                              Set2.Intercepts[j])) {   // But not the other meaning it crosses at some point
                        return true;    // Collision Occurred!
                    }
                } else if (Set2.DomainStarts[j] == Set2.DomainEnds[j]) {  // If the second line is undefined
                    if ((Set2.Slopes[j] < Set1.Slopes[i] * Set2.DomainEnds[j] + Set1.Intercepts[i]) !=
                        // Checks if the Y-value of the undefined function is above on one side
                        (Set2.intercepts[j] < Set1.Slopes[i] * Set2.DomainEnds[j] +
                                              Set1.Intercepts[i])) {   // But not the other meaning it crosses at some point
                        return true;    // Collision Occurred!
                    }
                } else if (Set1.Slopes[i] == Set2.Slopes[j]) {    // If both lines have the same slope
                    if (Set1.Intercepts[i] ==
                        Set2.Intercepts[j]) {  // If both are the same line (already checked for same domain)
                        return true;    // Collision Occurred!
                    }
                } else {
                    double DomainStart = std::min(Set1.DomainEnds[i],
                                                  Set2.DomainEnds[j]);     // Where the shared domain Starts
                    double DomainEnd = std::max(Set1.DomainStarts[i],
                                                Set2.DomainStarts[j]);    // Where the shared domain Ends
                    if ((Set1.Slopes[i] * DomainStart + Set1.Intercepts[i] <
                         Set2.Slopes[j] * DomainStart + Set2.Intercepts[j]) !=
                        // Checks if on one side of two lines shared domains
                        (Set1.Slopes[i] * DomainEnd + Set1.Intercepts[i] <= Set2.Slopes[j] * DomainEnd +
                                                                            Set2.Intercepts[j])) {  // One line is above and is below on the other aka they swap who is on top
                        return true;    // Collision Occurred!
                    }
                }
            }
        }
    }
    if (CheckPointInside(Vector2(Set1.DomainStarts[i], Set1.Slopes[i] * Set1.DomainStarts[i] + Set1.Intercepts[i]),
                         Set2)) { // Checks if a point of the first set is within the second
        return true;    // Collision Occurred!
    }
    if (CheckPointInside(Vector2(Set2.DomainStarts[j], Set2.Slopes[j] * Set2.DomainStarts[j] + Set2.Intercepts[j]),
                         Set1)) { // Checks if a point of the second set is within the first
        return true;    // Collision Occurred!
    }
    return false;   // No collision occurred
}

std::vector <Vector2> GetPointsOfCollision(FSet Set1, FSet Set2) {  // Gets the points of collision for two given 2D sets
    std::vector <Vector2> CollisionPoints;
    for (int i = 0; i < Set1.DomainStarts.size(); i++) {     // For each line in set 1
        for (int j = 0; i < Set2.DomainStarts.size(); j++) { // Compare to each line in set 2
            if (std::max(Set1.DomainStarts[i], Set2.DomainStarts[j]) >=
                std::min(Set1.DomainEnds[i], Set2.DomainEnds[j])) {   // If the lines share a domain they can collide
                if ((Set1.DomainStarts[i] == Set1.DomainEnds[i]) &&
                    (Set2.DomainStarts[j] == Set2.DomainEnds[j])) {  // If both lines are undefined
                    if (std::max(Set1.Slope[i], Set2.Slopes[j]) >
                        std::min(Set1.Intercepts[i], Set2.Intercepts[j])) {    // If they share a common Y
                        CollisionPoints.push_back(Vector2(Set1.DomainStarts[i], std::max(Set1.Slope[i],
                                                                                         Set2.Slopes[j])));   // Add the beginning of the shared Y
                        CollisionPoints.push_back(Vector2(Set1.DomainStarts[i], std::min(Set1.Intercepts[i],
                                                                                         Set2.Intercepts[j])));  // Add the end of the shared Y
                        continue;
                    }
                } else if (Set1.DomainStarts[i] == Set1.DomainEnds[i]) {    // If the first line is undefined
                    if ((Set1.Slopes[i] < Set2.Slopes[j] * Set1.DomainEnds[i] + Set2.Intercepts[j]) !=
                        // Checks if the Y-value of the undefined function is above on one side
                        (Set1.intercepts[i] < Set2.Slopes[j] * Set1.DomainEnds[i] +
                                              Set2.Intercepts[j])) {   // But not the other meaning it crosses at some point
                        CollisionPoints.push_back(Vector2(Set1.DomainEnds[i], Set2.Slopes[j] * Set1.DomainEnds[i] +
                                                                              Set2.Intercepts[j]));   // Add the point where they cross
                        continue;
                    }
                } else if (Set2.DomainStarts[j] == Set2.DomainEnds[j]) {  // If the second line is undefined
                    if ((Set2.Slopes[j] < Set1.Slopes[i] * Set2.DomainEnds[j] + Set1.Intercepts[i]) !=
                        // Checks if the Y-value of the undefined function is above on one side
                        (Set2.intercepts[j] < Set1.Slopes[i] * Set2.DomainEnds[j] +
                                              Set1.Intercepts[i])) {   // But not the other meaning it crosses at some point
                        CollisionPoints.push_back(Vector2(Set2.DomainEnds[j], Set1.Slopes[i] * Set2.DomainEnds[j] +
                                                                              Set1.Intercepts[i]));   // Add the point where they cross
                        continue;
                    }
                } else if (Set1.Slopes[i] == Set2.Slopes[j]) {    // If both lines have the same slope
                    if (Set1.Intercepts[i] ==
                        Set2.Intercepts[j]) {  // If both are the same line (already checked for same domain)
                        CollisionPoints.push_back(Vector2(std::max(Set1.DomainStarts[i], Set2.DomainStarts[j]),
                                                          Set1.Slopes[i] *
                                                          std::max(Set1.DomainStarts[i], Set2.DomainStarts[j]) +
                                                          Set1.Intercepts[i]));
                        CollisionPoints.push_back(Vector2(std::min(Set1.DomainEnds[i], Set2.DomainEnds[j]),
                                                          Set1.Slopes[i] *
                                                          std::min(Set1.DomainEnds[i], Set2.DomainEnds[j]) +
                                                          Set1.Intercepts[i]));
                        continue;
                    }
                } else {
                    double DomainStart = std::min(Set1.DomainEnds[i],
                                                  Set2.DomainEnds[j]);     // Where the shared domain Starts
                    double DomainEnd = std::max(Set1.DomainStarts[i],
                                                Set2.DomainStarts[j]);    // Where the shared domain Ends
                    if ((Set1.Slopes[i] * DomainStart + Set1.Intercepts[i] <
                         Set2.Slopes[j] * DomainStart + Set2.Intercepts[j]) !=
                        // Checks if on one side of two lines shared domains
                        (Set1.Slopes[i] * DomainEnd + Set1.Intercepts[i] <= Set2.Slopes[j] * DomainEnd +
                                                                            Set2.Intercepts[j])) {  // One line is above and is below on the other aka they swap who is on top
                        CollisionPoints.push_back(
                                (Set2.Intercepts[j] - Set1.Intecepts[i]) / (Set1.Slopes[i] - Set2.Slopes[j]),
                                Set1.Slopes[i] *
                                ((Set2.Intercepts[j] - Set1.Intecepts[i]) / (Set1.Slopes[i] - Set2.Slopes[j])) +
                                Set1.Intercepts[i]); // Gets the point where the two functions cross (X = (I2-I1)/(S1-S2))
                        continue;
                    }
                }
            }
        }
    }
    if (CollisionPoints.empty()) {   // If there were no line collisions check if one object is inside the other
        if (CheckPointInside(Vector2(Set1.DomainStarts[0], Set1.Slopes[0] * Set1.DomainStarts[0] + Set1.Intercepts[0]),
                             Set2)) { // Checks if a point of the first set is within the second
            for (int i = 0; i <
                            Set1.DomainStarts.size(); i++) { // Add each point of the first set inside the colliding points because the entire object is inside it
                CollisionPoints.push_back(Set1.DomainStarts[i], Set1.DomainStarts[i] * Set1.Slopes[i] +
                                                                Set1.Intercepts[i]);    // Add the first point of each function
            }
        } else   // Only one object can be completely inside the other... Unless 4D I guess
        if (CheckPointInside(Vector2(Set2.DomainStarts[0], Set2.Slopes[0] * Set2.DomainStarts[0] + Set2.Intercepts[0]),
                             Set1)) { // Checks if a point of the second set is within the first
            for (int i = 0; i <
                            Set2.DomainStarts.size(); i++) { // Add each point of the second set inside the colliding points because the entire object is inside it
                CollisionPoints.push_back(Set2.DomainStarts[i], Set2.DomainStarts[i] * Set2.Slopes[i] +
                                                                Set2.Intercepts[i]);    // Add the first point of each function
            }
        }
    }
    return CollisionPoints; // Return all the points of collision on the 2D plane
}

std::vector <Vector3> GetPointsOfCollision3D(FSet Set1XY, FSet Set1XZ, FSet Set2XY, FSet Set2XZ) {  // Gets the points of collision for two given 3D sets
    std::vector <Vector3> CollisionPoints1;
    std::vector <Vector3> CollisionPoints2;
    for (int i = 0; i < Set1XY.DomainStarts.size(); i++) {     // For each line in set 1
        for (int j = 0; i < Set2XY.DomainStarts.size(); j++) { // Compare to each line in set 2
            if (std::max(Set1XY.DomainStarts[i], Set2XY.DomainStarts[j]) >= std::min(Set1XY.DomainEnds[i],
                                                                                     Set2XY.DomainEnds[j])) {   // If the lines share a domain they can collide
                if ((Set1XY.DomainStarts[i] == Set1XY.DomainEnds[i]) &&
                    (Set2XY.DomainStarts[j] == Set2XY.DomainEnds[j])) {  // If both lines are undefined
                    if (std::max(Set1XY.Slope[i], Set2XY.Slopes[j]) >
                        std::min(Set1XY.Intercepts[i], Set2XY.Intercepts[j])) {    // If they share a common Y
                        CollisionPoints1.push_back(Vector2(Set1XY.DomainStarts[i], std::max(Set1XY.Slope[i],
                                                                                            Set2XY.Slopes[j]),));   // Add the beginning of the shared Y
                        CollisionPoints1.push_back(Vector2(Set1XY.DomainStarts[i], std::min(Set1XY.Intercepts[i],
                                                                                            Set2XY.Intercepts[j]),));  // Add the end of the shared Y
                        CollisionPoints2.push_back(Vector2(Set1XY.DomainStarts[i], std::max(Set1XY.Slope[i],
                                                                                            Set2XY.Slopes[j]),));   // Add the beginning of the shared Y
                        CollisionPoints2.push_back(Vector2(Set1XY.DomainStarts[i], std::min(Set1XY.Intercepts[i],
                                                                                            Set2XY.Intercepts[j]),));  // Add the end of the shared Y
                        continue;
                    }
                } else if (Set1XY.DomainStarts[i] == Set1XY.DomainEnds[i]) {    // If the first line is undefined
                    if ((Set1XY.Slopes[i] < Set2XY.Slopes[j] * Set1XY.DomainEnds[i] + Set2XY.Intercepts[j]) !=
                        // Checks if the Y-value of the undefined function is above on one side
                        (Set1XY.intercepts[i] < Set2XY.Slopes[j] * Set1XY.DomainEnds[i] +
                                                Set2XY.Intercepts[j])) {   // But not the other meaning it crosses at some point
                        CollisionPoints.push_back(Vector2(Set1XY.DomainEnds[i],
                                                          Set2XY.Slopes[j] * Set1XY.DomainEnds[i] +
                                                          Set2XY.Intercepts[j]));   // Add the point where they cross
                        continue;
                    }
                } else if (Set2XY.DomainStarts[j] == Set2XY.DomainEnds[j]) {  // If the second line is undefined
                    if ((Set2XY.Slopes[j] < Set1XY.Slopes[i] * Set2XY.DomainEnds[j] + Set1XY.Intercepts[i]) !=
                        // Checks if the Y-value of the undefined function is above on one side
                        (Set2XY.intercepts[j] < Set1XY.Slopes[i] * Set2XY.DomainEnds[j] +
                                                Set1XY.Intercepts[i])) {   // But not the other meaning it crosses at some point
                        CollisionPoints.push_back(Vector2(Set2XY.DomainEnds[j],
                                                          Set1XY.Slopes[i] * Set2XY.DomainEnds[j] +
                                                          Set1XY.Intercepts[i]));   // Add the point where they cross
                        continue;
                    }
                } else if (Set1XY.Slopes[i] == Set2XY.Slopes[j]) {    // If both lines have the same slope
                    if (Set1XY.Intercepts[i] ==
                        Set2XY.Intercepts[j]) {  // If both are the same line (already checked for same domain)
                        CollisionPoints.push_back(Vector2(std::max(Set1XY.DomainStarts[i], Set2XY.DomainStarts[j]),
                                                          Set1XY.Slopes[i] *
                                                          std::max(Set1XY.DomainStarts[i], Set2XY.DomainStarts[j]) +
                                                          Set1XY.Intercepts[i]));
                        CollisionPoints.push_back(Vector2(std::min(Set1XY.DomainEnds[i], Set2XY.DomainEnds[j]),
                                                          Set1XY.Slopes[i] *
                                                          std::min(Set1XY.DomainEnds[i], Set2XY.DomainEnds[j]) +
                                                          Set1XY.Intercepts[i]));
                        continue;
                    }
                } else {
                    double DomainStart = std::min(Set1XY.DomainEnds[i],
                                                  Set2XY.DomainEnds[j]);     // Where the shared domain Starts
                    double DomainEnd = std::max(Set1XY.DomainStarts[i],
                                                Set2XY.DomainStarts[j]);    // Where the shared domain Ends
                    if ((Set1XY.Slopes[i] * DomainStart + Set1XY.Intercepts[i] <
                         Set2XY.Slopes[j] * DomainStart + Set2XY.Intercepts[j]) !=
                        // Checks if on one side of two lines shared domains
                        (Set1XY.Slopes[i] * DomainEnd + Set1XY.Intercepts[i] <= Set2XY.Slopes[j] * DomainEnd +
                                                                                Set2XY.Intercepts[j])) {  // One line is above and is below on the other aka they swap who is on top
                        CollisionPoints.push_back(
                                (Set2XY.Intercepts[j] - Set1XY.Intecepts[i]) / (Set1XY.Slopes[i] - Set2XY.Slopes[j]),
                                Set1XY.Slopes[i] *
                                ((Set2XY.Intercepts[j] - Set1XY.Intecepts[i]) / (Set1XY.Slopes[i] - Set2XY.Slopes[j])) +
                                Set1XY.Intercepts[i]); // Gets the point where the two functions cross (X = (I2-I1)/(S1-S2))
                        continue;
                    }
                }
            }
        }
    }
    if (CollisionPoints.empty()) {   // If there were no line collisions check if one object is inside the other
        if (CheckPointInside(
                Vector2(Set1XY.DomainStarts[0], Set1XY.Slopes[0] * Set1XY.DomainStarts[0] + Set1XY.Intercepts[0]),
                Set2XY)) { // Checks if a point of the first set is within the second
            for (int i = 0; i <
                            Set1XY.DomainStarts.size(); i++) { // Add each point of the first set inside the colliding points because the entire object is inside it
                CollisionPoints.push_back(Set1XY.DomainStarts[i], Set1XY.DomainStarts[i] * Set1XY.Slopes[i] +
                                                                  Set1XY.Intercepts[i]);    // Add the first point of each function
            }
        } else   // Only one object can be completely inside the other... Unless 4D I guess
        if (CheckPointInside(
                Vector2(Set2XY.DomainStarts[0], Set2XY.Slopes[0] * Set2XY.DomainStarts[0] + Set2XY.Intercepts[0]),
                Set1XY)) { // Checks if a point of the second set is within the first
            for (int i = 0; i <
                            Set2XY.DomainStarts.size(); i++) { // Add each point of the second set inside the colliding points because the entire object is inside it
                CollisionPoints.push_back(Set2XY.DomainStarts[i], Set2XY.DomainStarts[i] * Set2XY.Slopes[i] +
                                                                  Set2XY.Intercepts[i]);    // Add the first point of each function
            }
        }
    }
    return CollisionPoints; // Return all the points of collision on the 2D plane
}

int main() {

    return 0;
}
