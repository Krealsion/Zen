#pragma once
#include <functional>
#include <string>

namespace Zen {
class Console {
public:
  Console();
  ~Console();

  void register_command(std::string command, std::function<void(std::string)> callback);

private:

};
}

