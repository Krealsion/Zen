//
// Created by jdemoss on 11/11/19.
//

#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Vector2.h"

class Rectangle {
public:
    Rectangle();
    Rectangle(double X, double Y, double Width, double Height);
    Rectangle(Vector2 Position, Vector2 Size);

    Rectangle& SetX(double X);
    Rectangle& SetY(double Y);
    Rectangle& SetWidth(double Width);
    Rectangle& SetHeight(double Height);
    Rectangle& SetPosition(Vector2 Position);
    Rectangle& SetSize(Vector2 Size);

    Vector2 GetPosition() const;
    Vector2 GetSize() const;
    double GetX() const;
    double GetY() const;
    double GetWidth() const;
    double GetHeight() const;

    Rectangle Copy() const;
    Rectangle DeepCopy() const;

protected:
    Vector2 Position;
    Vector2 Size;
};

#endif //RECTANGLE_H
