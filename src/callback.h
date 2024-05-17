#pragma once

#include <functional>

namespace Zen {
class Callback {
public:
  Callback(std::function<void()>&& callback) : _callback(std::move(callback)) {}

private:
  std::function<void()> _callback;

};
}