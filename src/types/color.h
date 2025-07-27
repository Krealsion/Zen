#pragma once
#include <SDL3/SDL_stdinc.h>

namespace Zen {

class Color {
public:
  Color() = default;
  Color(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha = 255);

  void set(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha = 255);
  void set_red(Uint8 red);
  void set_green(Uint8 green);
  void set_blue(Uint8 blue);
  void set_alpha(Uint8 alpha);

  Uint8 get_red() const;
  Uint8 get_green() const;
  Uint8 get_blue() const;
  Uint8 get_alpha() const;

private:
  Uint8 _red, _green, _blue, _alpha;
};
}
