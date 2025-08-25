#include <iostream>
#include <types/vector2.h>

using namespace Zen;

int main() {
  Vector2 a;
  std::cout << a << std::endl; // Output (0, 0)

  Vector2 b = Vector2(10, 5);
  std::cout << b << std::endl; // Output (10, 5)

  b.add(b * 2);
  std::cout << b << std::endl; // Output (30, 15)

  //Use copy in order to preserve data
  std::cout << b.copy().normalize() << std::endl; // Output (0.894427, 0.447214) - Normalized vector
  std::cout << b << std::endl; // Output (30, 15) - Original vector remains unchanged

  std::cout << b.invert() << std::endl; // Output (0.0333333, 0.0666667) - Inverted vector
  std::cout << b << std::endl; // Output (30, 15) - Original vector IS changed

  Vector2 c = Vector2(-1, 0);
  std::cout << b.copy().multiply(c) << std::endl; // Output (-0.0333333, 0) - Multiplied vector
}
