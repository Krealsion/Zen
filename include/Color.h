//
// Created by jdemoss on 11/21/19.
//

#ifndef COLOR_H
#define COLOR_H

#include <tiff.h>

class Color {
public:
    Color(uint8 red, uint8 green, uint8 blue, uint8 alpha = 0);

    void Set(uint8 red, uint8 green, uint8 blue, uint8 alpha = 0);
    void SetRed(uint8 red);
    void SetGreen(uint8 green);
    void SetBlue(uint8 blue);
    void SetAlpha(uint8 alpha);

    uint8 GetRed() const;
    uint8 GetGreen() const;
    uint8 GetBlue() const;
    uint8 GetAlpha() const;
private:
    uint8 Red, Green, Blue, Alpha;
};

#endif //COLOR_H
