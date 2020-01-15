#ifndef VECTOR3_H
#define VECTOR3_H

#include <iostream>

class Vector3 {
public:
    Vector3();
    Vector3(double x, double y, double z);

    //Returns a reference to this object, not a copy
    Vector3& SetX(double x);
    Vector3& SetY(double y);
    Vector3& SetZ(double z);
    Vector3& AddX(double x);
    Vector3& AddY(double y);
    Vector3& AddZ(double z);
    Vector3& Add(const Vector3& o);
    Vector3& Multiply(const Vector3& o);
    Vector3& Scale(double scalar);
    Vector3& Normalize();
    Vector3& Abs();
    Vector3& Negate();
    Vector3& Invert();

    static Vector3 Add(const Vector3& a, const Vector3& b);
    static Vector3 Multiply(const Vector3& a, const Vector3& b);
    static Vector3 Scale(const Vector3& v, const double& s);
    static Vector3 CrossProduct(const Vector3& a, const Vector3& b);
    static double DotProduct(const Vector3& a, const Vector3& b);

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
