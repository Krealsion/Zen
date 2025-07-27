#include "color.h"

#include <SDL3/SDL_stdinc.h>

namespace Zen {
Color::Color(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha) : _red(red), _green(green), _blue(blue), _alpha(alpha) {}

void Color::set(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha) {
  set_red(red);
  set_green(green);
  set_blue(blue);
  set_alpha(alpha);
}

Uint8 Color::get_red() const {
  return _red;
}

Uint8 Color::get_green() const {
  return _green;
}

Uint8 Color::get_blue() const {
  return _blue;
}

Uint8 Color::get_alpha() const {
  return _alpha;
}

void Color::set_red(Uint8 red) {
  _red = red;
}

void Color::set_green(Uint8 green) {
  _green = green;
}

void Color::set_blue(Uint8 blue) {
  _blue = blue;
}

void Color::set_alpha(Uint8 alpha) {
  _alpha = alpha;
}
}
