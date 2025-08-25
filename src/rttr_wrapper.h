#pragma once

#include <rttr/registration.h>
#include <string>
#include <unordered_map>

namespace Zen {

class RttrWrapper {
public:
  static void register_class(const std::string& class_name, const std::string& method_name, std::function<bool(const std::string&)> func) {
    rttr::registration::class_<RttrWrapper>(class_name)
        .method(method_name, func);
    // New: Reverse lookup registration
    method_to_class[method_name] = class_name;
  }

  static bool invoke_dynamic(const std::string& method_name, const std::string& arg) {
    auto it = method_to_class.find(method_name);
    if (it == method_to_class.end()) return false;  // Existence check
    std::string class_name = it->second;
    rttr::type t = rttr::type::get_by_name(class_name);
    if (!t.is_valid()) return false;
    rttr::method meth = t.get_method(method_name);
    if (!meth.is_valid()) return false;
    rttr::variant result = meth.invoke({}, arg);
    if (!result.is_valid()) return false;
    return result.get_value<bool>();
  }

  static bool method_exists(const std::string& method_name) {
    return method_to_class.count(method_name) > 0;
  }

private:
  static std::unordered_map<std::string, std::string> method_to_class;
};

std::unordered_map<std::string, std::string> RttrWrapper::method_to_class;

}  // namespace Zen
