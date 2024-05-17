#include "src/message_bus/var_storage.h"
#include <iostream>
#include <timer.h>

using namespace Zen;

class FunctionAdd {
public:
  VarStorage<double> a;
  VarStorage<double> b;

  double get_val() {
    return double(*this);
  }

  explicit operator double() {
    return double(a) + double(b);
  }
};

double get_double() {
  return 88.8f;
}

int main() {
  FunctionAdd adder;

  adder.a = 100;
  adder.b = 10;
  std::cout << adder.get_val() << std::endl;

  int i = 90;
  adder.a = i; // Value set (Will not update)
//adder.b = 10;
  std::cout << adder.get_val() << std::endl;

  std::string num_string = "123";
//adder.a = 90;
  adder.b.set(&num_string, [](const std::string& s) -> double { // Pointer set (will continuously update)
    return std::stod(s);
  });
  std::cout << adder.get_val() << std::endl;

  num_string = "444"; // Updates value when getting val from adder
  std::cout << adder.get_val() << std::endl;

  adder.b.set(&num_string, [](const std::string& s) -> int { // similar set, but returns int, which can convert into double
    return std::stoi(s);
  });
  std::cout << adder.get_val() << std::endl;

  adder.a = &get_double; // get_double returns 88.8f (found above)
//adder.b = 123
  std::cout << adder.get_val() << std::endl;

  double start = Zen::Timer::get_current_time() / 1000000;
  for (int l = 0; l < 10000000; l++) { // Aka until int max
    adder.get_val();
  }
  double end = Zen::Timer::get_current_time() / 1000000;
  std::cout << "start: " << start << std::endl << "duration: " << (end - start) << std::endl << "nanoseconds per call: " << (end - start) / 10 << std::endl;

  auto f = [](const std::string& s) -> int { // similar set, but returns int, which can convert into double
    return std::stoi(s);
  };

  start = Zen::Timer::get_current_time() / 1000000;
  for (int l = 0; l < 10000000; l++) { // Aka until int max
    double(f(num_string)) + get_double();
  }
  end = Zen::Timer::get_current_time() / 1000000;
  std::cout << "start: " << start << std::endl << "duration: " << (end - start) << std::endl << "nanoseconds per call: " << (end - start) / 10 << std::endl;
}