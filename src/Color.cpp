//
// Created by jdemoss on 11/21/19.
//

#include "Color.h"

Color::Color(uint8 red, uint8 green, uint8 blue, uint8 alpha) : Red(red), Green(green), Blue(blue), Alpha(alpha) { }

void Color::Set(uint8 red, uint8 green, uint8 blue, uint8 alpha) {
    SetRed(red);
    SetGreen(green);
    SetBlue(blue);
    SetAlpha(alpha);
}

uint8 Color::GetRed() const {
    return Red;
}

uint8 Color::GetGreen() const {
    return Green;
}

uint8 Color::GetBlue() const {
    return Blue;
}

uint8 Color::GetAlpha() const {
    return Alpha;
}

void Color::SetRed(uint8 red) {
    Red = red;
}

void Color::SetGreen(uint8 green) {
    Green = green;
}

void Color::SetBlue(uint8 blue) {
    Blue = blue;
}

void Color::SetAlpha(uint8 alpha) {
    Alpha = alpha;
}
