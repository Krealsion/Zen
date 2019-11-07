#ifndef VECTOR2_H
#define VECTOR2_H

#include <iostream>

class Vector2 {
public:
    Vector2();
    Vector2(double X, double Y);

    //Returns a reference to this object, not a copy
    Vector2* SetX(double X);
    Vector2* SetY(double Y);
    Vector2* AddX(double X);
    Vector2* AddY(double Y);
    Vector2* Scale(double Scalar);
    Vector2* Normalize();
    Vector2* Abs();
    Vector2* Invert();

    double GetX();
    double GetY();
    int GetXInt();
    int GetYInt();
    double GetMagnitude();

    friend std::ostream& operator<<(std::ostream &os, const Vector2 &v2);

protected:
    double X, Y;
};

#endif // VECTOR2_H
