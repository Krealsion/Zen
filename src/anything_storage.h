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
 * Now you can set it in a variety of ways:
 *
 * 1). Setting to a constant
 * any_double = 10.0f;
 * any_double.set(10.0f);
 *
 * 2). Setting to a pointer
 * any_double = new double(10);
 * any_double.set(new double(10));
 * NOTE: AnythingStorage does not delete data when it is done, this is purely and example of what is possible (don't do this please)
 *
 * 3). Setting to a function that gives the output type
 * any_double = &get_double;
 * any_double = &Zen::Timer::get_current_time;
 *
 * 4). Setting to object/function pair that gives:
 * The output type OR another type of object that can implicitly or explicitly convert to the output type OR a lambda that fufills either previous requirements
 * any_double.set(new string("123"), &get_double_from_string);
 * any_double.set(new string("123"), &get_int_from_string);
 * any_double.set(new string("123"), [](const std::string& s) {
 *     return std::stod(s); // Note: stod can't be used directly because of default parameters causing mismatched function headers
 * });
 * NOTE: This is only for objects that will be continually updated, as if it is a constant, just convert and pass using option 1
 */
template<typename T>
class AnythingStorage {
private:
  T _constant = T(); // NOTE: Class T must have a default constructor
  std::function<T()> _get_val;

public:
  AnythingStorage() {
    *this = T();
  }

  explicit operator T() {
    return get_value();
  }

  template<typename type>
  friend std::ostream& operator<<(std::ostream& os, AnythingStorage<type>& obj) {
    os << obj.get_value();
    return os;
  }

  template<typename convertable_to_T>
  AnythingStorage<T>& operator=(const convertable_to_T& object) {
    _constant = object;
    _get_val = std::function<T()>([this]() -> T {
      return _constant;
    });
    return *this;
  }

  template<typename convertable_to_T>
  AnythingStorage<T>& operator=(convertable_to_T* object) {
    T data_test = T(*object);
    _get_val = std::function<T()>([object]() -> T {
      return T(object);
    });
    return *this;
  }

  AnythingStorage<T>& operator=(T (* funct)()) {
    T type_test = funct();
    _get_val = std::function<T()>(funct);
    return *this;
  }

  template<typename convertable_to_T>
  void set(const convertable_to_T& object) {
    *this = object;
  }

  template<typename convertable_to_T>
  void set(convertable_to_T* object) {
    *this = object;
  }

  void set(T (* function)()) {
    *this = function;
  }

  template<typename function_convertable_to_T, typename function_output_that_can_be_T>
  void set(function_convertable_to_T* object, function_output_that_can_be_T (* conversion)(function_convertable_to_T)) {
    _get_val = std::function<T()>([object, conversion]() -> T {
      return conversion(*object);
    });
  }

  template<typename function_convertable_to_T, typename lambda_function>
  void set(function_convertable_to_T* object, lambda_function function) {
    _get_val = std::function<T()>([object, function]() -> T {
      return function(*object);
    });
  }

  T get_value() {
    return _get_val();
  }
};
}

