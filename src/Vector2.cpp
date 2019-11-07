#include "../include/Vector2.h"
#include <math.h>

Vector2::Vector2() {
    X = 0;
    Y = 0;
}

Vector2::Vector2(double X, double Y) {
    this->X = X;
    this->Y = Y;
}

Vector2 *Vector2::SetX(double X) {
    this->X = X;
    return this;
}

Vector2 *Vector2::SetY(double Y) {
    this->Y = Y;
    return this;
}

Vector2 *Vector2::AddX(double X) {
    this->X += X;
    return this;
}

Vector2 *Vector2::AddY(double Y) {
    this->Y += Y;
    return this;
}

double Vector2::GetX() {
    return this->X;
}

double Vector2::GetY() {
    return this->Y;
}

int Vector2::GetXInt() {
    return round(this->X)
}

int Vector2::GetYInt() {
    return round(this->Y)
}

Vector2 *Vector2::Scale(double Scalar) {
    X *= Scalar;
    Y *= Scalar;
    return this;
}

double Vector2::GetMagnitude() {
    return sqrt(X ^ 2 + Y ^ 2);
}

Vector2 *Vector2::Normalize() {
    return Scale(1 / GetMagnitude());
}

Vector2 *Vector2::Abs() {
    X = abs(X);
    Y = abs(Y);
    return this;
}

Vector2 *Vector2::Invert() {
    X *= -1;
    Y *= -1;
    return this;
}
