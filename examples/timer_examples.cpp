#include <iostream>
#include <timer.h>

using namespace Zen;

int main() {
  auto x = 0;
  auto timer1 = Timer(1000);
  auto timer2 = Timer(20);
  while (x < 3) {
    Timer::update_time();
    if (timer2.is_time()) {
      //Toggle the pause state every 20ms
      if (timer1.is_paused()) {
        timer1.resume();
      } else {
        timer1.pause();
      }
    }
    std::cout << timer1.peek_progress_pecentage() << std::endl;
    if (timer1.is_time()) {
      x += 1;
      //Decrease the speed of the timer, effectively adding 1 second every loop
      timer1.set_time_multiplier((double) 1 / (double) (x + 1));
      std::cout << x << std::endl;
      if (timer2.is_paused()) {
        timer2.resume();
      } else {
        timer2.pause();
      }
    }
  }

  // TESTING std::chrono::steady_clock::now() FOR PERFORMANCE
  auto loop_count = 100000000;
  auto start = std::chrono::steady_clock::now();
  for (long i = 0; i < loop_count; i++) {
    //Empty loop to count loop time cost
  }
  auto end = std::chrono::steady_clock::now();
  auto time = end - start;
  double ns_per_loop = ((std::chrono::duration_cast<std::chrono::nanoseconds>(time).count()) / (double) loop_count);

  start = std::chrono::steady_clock::now();
  for (long i = 0; i < loop_count; i++) {
    auto time_test = std::chrono::steady_clock::now();
  }
  end = std::chrono::steady_clock::now();
  time = end - start;
  std::cout << "Each std::chrono::steady_clock::now() call took roughly " <<
            ((std::chrono::duration_cast<std::chrono::nanoseconds>(time).count()) / (double) loop_count - ns_per_loop)
            << "ns.";
  return 0;
}
