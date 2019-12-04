#include "Vector2.h"

#include <cmath>
#include <iostream>

Vector2::Vector2() {
    X = 0;
    Y = 0;
}

Vector2::Vector2(double X, double Y) {
    this->X = X;
    this->Y = Y;
}

Vector2& Vector2::SetX(double X) {
    this->X = X;
    return *this;
}

Vector2& Vector2::SetY(double Y) {
    this->Y = Y;
    return *this;
}

Vector2& Vector2::AddX(double X) {
    this->X += X;
    return *this;
}

Vector2& Vector2::AddY(double Y) {
    this->Y += Y;
    return *this;
}

double Vector2::GetX() const {
    return this->X;
}

double Vector2::GetY() const {
    return this->Y;
}

int Vector2::GetXInt() const {
    return (int)round(X);
}

int Vector2::GetYInt() const {
    return (int)round(Y);
}

Vector2& Vector2::Add(Vector2 o) {
    X += o.GetX();
    Y += o.GetY();
    return *this;
}

Vector2& Vector2::Multiply(Vector2 o) {
    X *= o.GetX();
    Y *= o.GetY();
    return *this;
}

Vector2& Vector2::Scale(double Scalar) {
    X *= Scalar;
    Y *= Scalar;
    return *this;
}

double Vector2::GetMagnitude() const {
    return sqrt(X * X + Y * Y);
}

Vector2& Vector2::Normalize() {
    return Scale(1 / GetMagnitude());
}

Vector2& Vector2::Abs() {
    X = fabs(X);
    Y = fabs(Y);
    return *this;
}

Vector2& Vector2::Negate() {
    X *= -1;
    Y *= -1;
    return *this;
}

Vector2& Vector2::Invert() {
    X = 1 / X;
    Y = 1 / Y;
    return *this;
}

Vector2 Vector2::Copy() const {
    return {X, Y};
}

std::ostream& operator<<(std::ostream& os, const Vector2& v2) {
    os << "(" << v2.X << ", " << v2.Y << ")";
    return os;
}
