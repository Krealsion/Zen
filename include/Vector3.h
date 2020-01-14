#ifndef VECTOR3_H
#define VECTOR3_H

#include <iostream>

class Vector3 {
public:
    Vector3();
    Vector3(double X, double Y, double Z);

    //Returns a reference to this object, not a copy
    Vector3& SetX(double X);
    Vector3& SetY(double Y);
    Vector3& SetZ(double Z);
    Vector3& AddX(double X);
    Vector3& AddY(double Y);
    Vector3& AddZ(double Z);
    Vector3& Add(Vector3 o);
    Vector3& Multiply(Vector3 o);
    Vector3& Scale(double Scalar);
    Vector3& Normalize();
    Vector3& Abs();
    Vector3& Negate();
    Vector3& Invert();

    static Vector3 Add(Vector3 a, Vector3 b);
    static Vector3 Multiply(Vector3 a, Vector3 b);
    static Vector3 Scale(Vector3 v, double s);
    static Vector3 CrossProduct(Vector3 a, Vector3 b);
    static double DotProduct(Vector3 a, Vector3 b);

    double GetX() const;
    double GetY() const;
    double GetZ() const;
    int GetXInt() const;
    int GetYInt() const;
    int GetZInt() const;
    double GetMagnitude() const;

    Vector3 Copy() const;

    friend std::ostream& operator<<(std::ostream& os, const Vector3& v3);

protected:
    double X, Y, Z;
};

#endif // VECTOR3_H
