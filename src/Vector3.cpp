#include "Vector3.h"

#include <cmath>
#include <iostream>

Vector3::Vector3() {
    X = 0;
    Y = 0;
    Z = 0;
}

Vector3::Vector3(double x,  double y,  double z) {
    X = x;
    Y = y;
    Z = z;
}

Vector3& Vector3::SetX( double x) {
    X = x;
    return *this;
}

Vector3& Vector3::SetY( double y) {
    Y = y;
    return *this;
}

Vector3& Vector3::SetZ( double z) {
    Z = z;
    return *this;
}

Vector3& Vector3::AddX( double x) {
    X += x;
    return *this;
}

Vector3& Vector3::AddY( double y) {
    Y += y;
    return *this;
}

Vector3& Vector3::AddZ( double z) {
    Z += z;
    return *this;
}

double Vector3::GetX() const {
    return X;
}

double Vector3::GetY() const {
    return Y;
}

double Vector3::GetZ() const {
    return Z;
}

int Vector3::GetXInt() const {
    return (int)round(X);
}

int Vector3::GetYInt() const {
    return (int)round(Y);
}

int Vector3::GetZInt() const {
    return (int)round(Z);
}

Vector3& Vector3::Add(const Vector3& o) {
    X += o.GetX();
    Y += o.GetY();
    Y += o.GetZ();
    return *this;
}

Vector3& Vector3::Multiply(const Vector3& o) {
    X *= o.GetX();
    Y *= o.GetY();
    Z *= o.GetZ();
    return *this;
}

Vector3& Vector3::Scale(double scalar) {
    X *= scalar;
    Y *= scalar;
    Z *= scalar;
    return *this;
}

double Vector3::GetMagnitude() const {
    return sqrt(X * X + Y * Y + Z * Z);
}

Vector3& Vector3::Normalize() {
    return Scale(1 / GetMagnitude());
}

Vector3& Vector3::Abs() {
    X = fabs(X);
    Y = fabs(Y);
    Z = fabs(Z);
    return *this;
}

Vector3& Vector3::Negate() {
    X *= -1;
    Y *= -1;
    Z *= -1;
    return *this;
}

Vector3& Vector3::Invert() {
    X = 1 / X;
    Y = 1 / Y;
    Z = 1 / Z;
    return *this;
}

Vector3 Vector3::Add(const Vector3 &a, const Vector3& b){
    return {a.GetX() + b.GetX(), a.GetY() + b.GetY(), a.GetZ() + b.GetZ()};
}

Vector3 Vector3::Multiply(const Vector3& a, const Vector3& b) {
    return {a.GetX() * b.GetX(), a.GetY() * b.GetY(), a.GetZ() * b.GetZ()};
}

Vector3 Vector3::Scale(const Vector3& v, const double& s) {

    return {v.GetX() * s, v.GetY() * s, v.GetZ() * s};
}

Vector3 Vector3::CrossProduct(const Vector3& a, const Vector3& b) {
    return {a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X};
}

double Vector3::DotProduct(const Vector3& a, const Vector3& b) {
    return a.X * b.X + a.Y * b.Y + a.Z * b.Z;
}

Vector3 Vector3::Copy() const {
    return {X, Y, Z};
}

std::ostream& operator<<(std::ostream& os, const Vector3& v3) {
    os << "(" << v3.X << ", " << v3.Y  << ", " << v3.GetZ() << ")";
    return os;
}
