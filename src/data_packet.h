#pragma once

#include "var_storage.h"

#include <string>
#include <vector>

namespace Zen {

class DataPacket {
private:
  class DataNode {
  public:
    DataNode();
    DataNode(std::string name, void* data, std::function<void()> delete_func) {
      this->name = std::move(name);
      this->data = data;
      this->delete_func = std::move(delete_func);
    }
    ~DataNode() {
      delete_func();
    }
    std::string name;
    void* data;
    std::function<void()> delete_func;
  };
  std::vector<DataNode> _attached_objects = std::vector<DataNode>();

public:
  template<typename T>
  void attach_object(std::string name, T* object, bool delete_when_destructed) {
    _attached_objects.emplace_back(std::move(name), (void*)object, (delete_when_destructed) ? [](void* data) {
      delete (static_cast<T*>(data));
    } : [](void*) {});
  }

  template<typename T>
  void attach_object(std::string name, T object) {
    auto pointer = new T(object);
    _attached_objects.emplace_back(std::move(name), pointer, [pointer]() {
    });
  }

  template<typename T>
  T* get_object(const std::string& name) const {
    if (_attached_objects.empty()) {
      return nullptr;
    }
    auto it = std::find_if(_attached_objects.begin(), _attached_objects.end(),
                           [name](const DataNode& node) {
                             return node.name == name;
                           });
    return (T*)(it->data);
  }


};
}
