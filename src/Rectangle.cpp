//
// Created by jdemoss on 11/11/19.
//

#include "Rectangle.h"

Rectangle::Rectangle() = default;

Rectangle::Rectangle(double X, double Y, double Width, double Height) {
    Position = Vector2(X, Y);
    Size = Vector2(Width, Height);
}

Rectangle::Rectangle(Vector2 Position, Vector2 Size) {
    // Ensure that this is a copy set, not a reference
    this->Position = Position;
    this->Size = Size;
}

Rectangle& Rectangle::SetPosition(Vector2 Position) {
    this->Position = Position;
    return *this;
}

Rectangle& Rectangle::SetSize(Vector2 Size) {
    this->Size = Size;
    return *this;
}

Vector2 Rectangle::GetPosition() {
    return Position;
}

Vector2 Rectangle::GetSize() {
    return Size;
}

double Rectangle::GetX() const {
    return Position.GetX();
}

double Rectangle::GetY() const {
    return Position.GetY();
}

double Rectangle::GetWidth() const {
    return Size.GetX();
}

double Rectangle::GetHeight() const {
    return Size.GetY();
}

Rectangle Rectangle::Copy() const {
    return {Position, Size};
}

Rectangle Rectangle::DeepCopy() const {
    return {Position.Copy(), Size.Copy()};
}
