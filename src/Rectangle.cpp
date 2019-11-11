//
// Created by jdemoss on 11/11/19.
//

#include "../include/Rectangle.h"

Rectangle::Rectangle() = default;

Rectangle::Rectangle(Vector2 Position, Vector2 Size) {
    // Ensure that this is a copy set, not a reference
    this->Position = Position;
    this->Size = Size;
}

Vector2 Rectangle::GetPosition() {
    return Position;
}

Vector2 Rectangle::GetSize() {
    return Size;
}

Rectangle& Rectangle::SetPosition(Vector2 Position) {
    this->Position = Position;
    return *this;
}

Rectangle& Rectangle::SetSize(Vector2 Size) {
    this->Size = Size;
    return *this;
}

Rectangle Rectangle::Copy() {
    return Rectangle(Position, Size);
}

Rectangle Rectangle::DeepCopy() {
    return Rectangle(Position.Copy(), Size.Copy());
}
