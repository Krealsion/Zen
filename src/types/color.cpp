#include "color.h"

namespace Zen {
Color::Color(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) : _red(red),
                                                                                                _green(green),
                                                                                                _blue(blue),
                                                                                                _alpha(alpha) {}

void Color::set(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) {
  set_red(red);
  set_green(green);
  set_blue(blue);
  set_alpha(alpha);
}

unsigned char Color::get_red() const {
  return _red;
}

unsigned char Color::get_green() const {
  return _green;
}

unsigned char Color::get_blue() const {
  return _blue;
}

unsigned char Color::get_alpha() const {
  return _alpha;
}

void Color::set_red(unsigned char red) {
  _red = red;
}

void Color::set_green(unsigned char green) {
green = green;
}

void Color::set_blue(unsigned char blue) {
  _blue = blue;
}

void Color::set_alpha(unsigned char alpha) {
  _alpha = alpha;
}
}
