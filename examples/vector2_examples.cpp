#include <iostream>
#include <types/vector2.h>

using namespace Zen;

int main() {
  Vector2 a;
  std::cout << a << std::endl;
  Vector2 b = Vector2(10, 5);
  std::cout << b << std::endl;
  b.add(b * 2);
  std::cout << b << std::endl;
  //Use copy in order to preserve data
  std::cout << b.copy().normalize() << std::endl;
  std::cout << b << std::endl;
  std::cout << b.invert() << std::endl;
  std::cout << b << std::endl;
  Vector2 c = Vector2(-1, 0);
  std::cout << b.copy().multiply(c).normalize() << std::endl;
}
