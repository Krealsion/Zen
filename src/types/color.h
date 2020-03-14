#pragma once

namespace Zen {

class Color {
public:
  Color(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 0);

  void set(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 0);
  void set_red(unsigned char red);
  void set_green(unsigned char green);
  void set_blue(unsigned char blue);
  void set_alpha(unsigned char alpha);

  unsigned char get_red() const;
  unsigned char get_green() const;
  unsigned char get_blue() const;
  unsigned char get_alpha() const;

private:
  unsigned char _red, _green, _blue, _alpha;
};
}
