#pragma once

#include <cxxabi.h>
#include <memory>
#include <string>
#include <typeinfo>

namespace Zen {
class Utility {
public:
  static std::string demangle(const char* mangled_name) {
    int status = 0;
    std::unique_ptr<char, void (*)(void*)> demangled(
        abi::__cxa_demangle(mangled_name, nullptr, nullptr, &status),
        std::free
    );
    return (status == 0 && demangled) ? demangled.get() : mangled_name;
  }
};
}