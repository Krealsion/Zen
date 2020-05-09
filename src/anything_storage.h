#pragma once

#include <functional>

namespace Zen {
/*
 * Anything storage is a kind of smart storage object that can house many different
 * objects that give an expected result.
 *
 * The purpose of this is to be a concept of a pointer, while allowing diverse
 * modularity, a wide implementation ability, and ease to the user. It does so by
 * keeping as much code and specification off of the user.
 *
 * How to use:
 * First, declare an AnythingStorage with a type specifier.
 * e.g.
 * AnythingStorage<double> any_double;
 *
 * Now you can set it in a variety of ways!
 *
 * Set Constant:
 * Used for non stored variables, or local variables. Just stores the value
 * e.g.
 * any_double.set_c(1.34);
 * any_double.set_c(10);
 *
 * Set Reference:
 * Used to store references to objects that can be implicitly casted to the given type
 * e.g.
 *   any_double.set(a_variable_that_can_cast_to_double);
 *
 * Set Reference with Function:
 * Sometimes what you want to set it to can't be implicitly casted,
 * but can be converted using a function, this is for that.
 * e.g.
 * any_double.set(my_string, &get_double_from_string);
 * where get_double_from_string is a method with header:
 *   double get_double_from_string(std::string input);
 *
 * Set Function:
 * Used to just be a function that returns a value of a given type
 * e.g.
 * any_double.set_f(&get_current_double);
 * where get_current_double is a method with header:
 *   double get_current_double();
 */
template<typename T>
class AnythingStorage {
private:
  std::function<T(void*)>* _get_val = nullptr; // Turns data into output type
  std::function<T(void* /* pointer to a function */, void* /* pointer to data */)>* _conversion_function = nullptr; // Turns conversion_data and data into output type

  void* _data = nullptr; // pointer to data
  void* _conversion_data = nullptr; // pointer to a function

  // This is never directly referenced, as the _constant memory address will only be used as a storage _data points to
  T _constant = T(); // NOTE: Class T must have a default constructor

  void _clean() {
    if (_get_val != nullptr) { // If this is a raw value function
      delete _get_val;
      _get_val = nullptr;
    }
    if (_conversion_function != nullptr) {
      delete _conversion_function;
      _conversion_function = nullptr;
    }
  }

public:
  AnythingStorage() {
    *this = T();
  }

  operator T() {
    return get_value();
  }

  template<typename type>
  friend std::ostream& operator<<(std::ostream& os, AnythingStorage<type>& obj);

  template<typename convertable_to_T>
  AnythingStorage<T>& operator=(const convertable_to_T& object) {
    _clean();
    _constant = T(object); // Store a copy
    _get_val = new std::function<T(void*)>([](void* stored_data) -> T {
      return *(static_cast<T*>(stored_data));
    });
    _data = (void*) (&_constant);
    return *this;
  }

  template<typename anythingthatconvertstoT>
  AnythingStorage<T>& operator=(anythingthatconvertstoT* object) {
    _clean();
    T data_test = T(*object);
    _get_val = new std::function<T(void*)>([](void* stored_data) -> T {
      return T(*(static_cast<anythingthatconvertstoT*>(stored_data)));
    });
    _data = (void*) (object);
    return *this;
  }

  AnythingStorage<T>& operator=(T (* funct)()) {
    _clean();
    T type_test = funct();
    _get_val = new std::function<T(void*)>([](void* stored_data) -> T {
      return reinterpret_cast<T(*)()>(reinterpret_cast<long long>(stored_data))();
    });
    _data = (void*) (*funct);
    return *this;
  }

  template<typename convertable_to_T>
  void set(const convertable_to_T& object) {
    *this = object;
  }

  template<typename anythingthatconvertstoT>
  void set(anythingthatconvertstoT* object) {
    *this = object;
  }

  void set(T (* function)()) {
    *this = function;
  }

  template<typename function_convertable_to_T>
  void set(function_convertable_to_T& object, T (* conversion)(function_convertable_to_T)) {
    _clean();
    _conversion_function = new std::function<T(void*, void*)>([](void* function_data, void* stored_data) -> T {
      auto function_pointer = reinterpret_cast<T (*)(function_convertable_to_T) > (function_data);
      return (*function_pointer)(*(static_cast<function_convertable_to_T*>(stored_data)));
    });
    _data = (void*) (&object);
    _conversion_data = (void*) (*conversion);
  }

  template<typename function_convertable_to_T, typename function_type>
  void set(function_convertable_to_T& object, function_type function) {
    _clean();
    _conversion_function = new std::function<T(void*, void*)>([](void* function_data, void* stored_data) -> T {
      auto function_pointer = reinterpret_cast<function_type*>(function_data);
      return (*function_pointer)(*(static_cast<function_convertable_to_T*>(stored_data)));
    });
    _data = (void*) (&object);
    _conversion_data = (void*) (*function);
  }

  template<typename function_convertable_to_T, typename an_output_that_can_be_T>
  void set(function_convertable_to_T& object, an_output_that_can_be_T (* conversion)(function_convertable_to_T)) {
    _clean();
    _conversion_function = new std::function<T(void*, void*)>([](void* function_data, void* stored_data) -> T {
      auto function_pointer = reinterpret_cast<an_output_that_can_be_T (*)(function_convertable_to_T) > (function_data);
      return T(function_pointer(*(static_cast<function_convertable_to_T*>(stored_data))));
    });
    _data = (void*) (&object);
    _conversion_data = (void*) (*conversion);
  }

  T get_value() {
    if (_data == nullptr) return T();
    if (_conversion_function != nullptr)
      return (*_conversion_function)(_conversion_data, _data);
    return (*_get_val)(_data);
  }
};

template<typename T>
std::ostream& operator<<(std::ostream& os, AnythingStorage<T>& obj) {
  os << obj.get_value();
  return os;
}
}

