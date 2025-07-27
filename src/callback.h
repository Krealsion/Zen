#pragma once

#include <functional>
#include <utility>
#include <stdexcept>

#include <functional>

namespace Zen {

template <typename... ArgTypes>
class Callback {
protected:
  using FuncType = std::function<void(ArgTypes...)>;
  Callback() = default;
  explicit Callback(FuncType func) : func_(std::move(func)) {}

  void call(ArgTypes... args) {
    if (func_) {
      func_(std::forward<ArgTypes>(args)...);
    } else {
      throw std::runtime_error("Callback function is not set");
    }
  }

  void operator()(ArgTypes... args) const {
    call(std::forward<ArgTypes>(args)...);
  }

protected:
  FuncType func_;
};
}