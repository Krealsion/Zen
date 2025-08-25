#include <iostream>
#include "../src/timer.h"

using namespace Zen;

int main() {
  auto x = 5;
  auto timer1 = Timer(1000);

  std::cout << "Liftoff in: " << x << " seconds" << std::endl;
  while (x > 0) {
    if (timer1.is_time()) {
      std::cout << x-- << "..." << std::endl;
    }
  }
  std::cout << "Liftoff!" << std::endl;




  auto timer2 = Timer(20);

  // Timers do not always trigger in order
  int gold = 0;
  while (x < 10) {
    if (timer1.is_time()) {
      std::cout << "Timer1: " << x << std::endl;
      x++;
    }
    if (timer2.is_time()) {
      gold++;
      if (gold % 10 == 0) {
        std::cout << "Gold: " << gold << std::endl;
      }
    }
  }

  return 0;
}
