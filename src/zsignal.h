#pragma once

#include <vector>
#include <functional>

#include <callback.h>

namespace Zen {
class Signal {
public:
  Signal() = default;

  void operator()() const {
    emit();
  }

  void connect(void* key, Action<>&& callback) {
    _callbacks.emplace_back(key, std::move(callback));
  }

  void disconnect(void* key) {
    std::erase_if(_callbacks, [key](const auto& callback_tuple) { return std::get<0>(callback_tuple) == key; });
  }

  void emit() const
  {
    for (auto& callback_tuple : _callbacks) {
      if (std::get<0>(callback_tuple) == nullptr) {
        // TODO add error logging
        continue; // Skip if the key is null
      }
      std::get<1>(callback_tuple)();
    }
  }

private:
  std::vector<std::tuple<void*, Action<>>> _callbacks;
};
}