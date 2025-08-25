#pragma once

#include <string>
#include <vector>
#include <functional>
#include <nlohmann/json.hpp>
#include <rttr/registration.h>

namespace Zen {

class DataPacket {
private:
  class DataNode {
  public:
    DataNode(std::string name, rttr::variant data_var)
        : name(std::move(name)), data_var(data_var) {}
    std::string name;
    rttr::variant data_var;
  };
  std::vector<DataNode> _attached_objects;

public:
  void attach_object(const std::string& name, const rttr::variant& value) {
    _attached_objects.emplace_back(name, value);
  }

  rttr::variant get_object(const std::string& name) const {
    auto it = std::find_if(_attached_objects.begin(), _attached_objects.end(),
                           [&name](const DataNode& node) { return node.name == name; });
    if (it != _attached_objects.end()) return it->data_var;
    return rttr::variant();
  }

  nlohmann::json to_json() const {
    nlohmann::json j;
    for (const auto& node : _attached_objects) {
      rttr::type t = node.data_var.get_type();
      std::string type_name = t.get_name().to_string();
      if (t.is_arithmetic() || t.is_enumeration()) {
        std::string str_val;
        node.data_var.convert(str_val);  // to_string via convert
        j[node.name + ":" + type_name] = str_val;
      } else if (t.is_string()) {
        j[node.name + ":" + type_name] = node.data_var.get_value<std::string>();
      } else if (t.is_class()) {
        nlohmann::json obj;
        for (auto& prop : t.get_properties()) {
          rttr::variant prop_val = prop.get_value(node.data_var);
          obj[prop.get_name().to_string()] = prop_val.to_string();  // Recursive to_string if needed
        }
        j[node.name + ":" + type_name] = obj;
      } else if (t.is_array() || t.is_sequential_container()) {
        nlohmann::json arr;
        rttr::array_range range = node.data_var.create_array_range();
        for (auto& item : range) {
          arr.push_back(item.to_string());  // to_string for elements
        }
        j[node.name + ":" + type_name] = arr;
      } else {
        // Fallback for unknown: to_string if possible
        std::string str_val;
        if (node.data_var.convert(str_val)) {
          j[node.name + ":" + type_name] = str_val;
        }
      }
    }
    return j;
  }

  static DataPacket from_json(const nlohmann::json& j) {
    DataPacket packet;
    for (auto& [key, val] : j.items()) {
      size_t colon_pos = key.find(':');
      if (colon_pos == std::string::npos) continue;
      std::string name = key.substr(0, colon_pos);
      std::string type_str = key.substr(colon_pos + 1);
      rttr::type t = rttr::type::get_by_name(type_str);
      if (!t.is_valid()) continue;
      rttr::variant v = t.create();  // Create instance
      if (t.is_arithmetic() || t.is_enumeration()) {
        v = val.get<double>();  // Example; adapt per type
      } else if (t.is_string()) {
        v = val.get<std::string>();
      } else if (t.is_class()) {
        for (auto& prop : t.get_properties()) {
          if (val.contains(prop.get_name().to_string())) {
            rttr::variant prop_val(val[prop.get_name().to_string()].get<std::string>());  // From string
            prop.set_value(v, prop_val);
          }
        }
      } else if (t.is_array() || t.is_sequential_container()) {
        rttr::array_range range = v.create_array_range();
        for (auto& item : val) {
          rttr::variant elem(item.get<std::string>());
          range.insert(range.end(), elem);
        }
      } else {
        // Fallback: Convert from string
        v.convert(val.get<std::string>());
      }
      packet.attach_object(name, v);
    }
    return packet;
  }
};

}  // namespace Zen
