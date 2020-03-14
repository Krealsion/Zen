//
// Created by jdemoss on 11/7/19.
//

#include <iostream>
#include "vector2.h"

int main() {
  Zen::Vector2 a;
  std::cout << a << std::endl;
  Zen::Vector2 b = Zen::Vector2(10, 5);
  std::cout << b << std::endl;
  b.add(b).add(b);
  std::cout << b << std::endl;
  //Use copy in order to preserve data
  std::cout << b.copy().normalize() << std::endl;
  std::cout << b << std::endl;
  std::cout << b.invert() << std::endl;
  std::cout << b << std::endl;
  Zen::Vector2 c = Zen::Vector2(-1, 0);
  std::cout << b.copy().multiply(c).normalize() << std::endl;
}
