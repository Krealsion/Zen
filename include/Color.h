//
// Created by jdemoss on 11/21/19.
//

#ifndef COLOR_H
#define COLOR_H

class Color {
public:
    Color(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 0);

    void Set(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 0);
    void SetRed(unsigned char red);
    void SetGreen(unsigned char green);
    void SetBlue(unsigned char blue);
    void SetAlpha(unsigned char alpha);

    unsigned char GetRed() const;
    unsigned char GetGreen() const;
    unsigned char GetBlue() const;
    unsigned char GetAlpha() const;

private:
    unsigned char Red, Green, Blue, Alpha;
};

#endif //COLOR_H
