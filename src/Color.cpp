//
// Created by jdemoss on 11/21/19.
//

#include "Color.h"

Color::Color(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) : Red(red), Green(green), Blue(blue), Alpha(alpha) { }

void Color::Set(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) {
    SetRed(red);
    SetGreen(green);
    SetBlue(blue);
    SetAlpha(alpha);
}

unsigned char Color::GetRed() const {
    return Red;
}

unsigned char Color::GetGreen() const {
    return Green;
}

unsigned char Color::GetBlue() const {
    return Blue;
}

unsigned char Color::GetAlpha() const {
    return Alpha;
}

void Color::SetRed(unsigned char red) {
    Red = red;
}

void Color::SetGreen(unsigned char green) {
    Green = green;
}

void Color::SetBlue(unsigned char blue) {
    Blue = blue;
}

void Color::SetAlpha(unsigned char alpha) {
    Alpha = alpha;
}
