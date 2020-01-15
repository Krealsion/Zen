#ifndef VECTOR2_H
#define VECTOR2_H

#include <iostream>

class Vector2 {
public:
    Vector2();
    Vector2(double x, double y);

    //Returns a reference to this object, not a copy
    Vector2& SetX(double x);
    Vector2& SetY(double y);
    Vector2& AddX(double x);
    Vector2& AddY(double y);
    Vector2& Add(Vector2 o);
    Vector2& Multiply(Vector2 o);
    Vector2& Scale(double scalar);
    Vector2& Normalize();
    Vector2& Abs();
    Vector2& Negate();
    Vector2& Invert();

    double GetX() const;
    double GetY() const;
    int GetXInt() const;
    int GetYInt() const;
    double GetMagnitude() const;

    Vector2 Copy() const;

    friend std::ostream& operator<<(std::ostream& os, const Vector2& v2);

protected:
    double X, Y;
};

#endif // VECTOR2_H
