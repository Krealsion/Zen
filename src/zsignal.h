#pragma once

#include <vector>
#include <functional>

namespace Zen {
class Signal {
public:
  Signal() = default;

  void operator()() {
    emit();
  }

  void connect(void* key, std::function<void()>&& callback) {
    _callbacks.emplace_back(key, std::move(callback));
  }

  void disconnect(void* key) {
    _callbacks.erase(std::remove_if(_callbacks.begin(), _callbacks.end(), [key](const auto& callback_tuple) {
      return std::get<0>(callback_tuple) == key;
    }), _callbacks.end());
  }

  void emit() {
    for (auto& callback_tuple : _callbacks) {
      if (std::get<0>(callback_tuple) == nullptr) {
        // TODO add error logging
        continue; // Skip if the key is null
      }
      std::get<1>(callback_tuple)();
    }
  }

private:
  std::vector<std::tuple<void*, std::function<void()>>> _callbacks;
};
}