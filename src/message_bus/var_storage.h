#pragma once

#include <functional>
#include <iostream>

namespace Zen {
/*
 * VarStorage is a kind of smart storage object that can house many different
 * objects that give an expected result.
 *
 * The purpose of this is to be a concept of a pointer, while allowing diverse
 * modularity, a wide implementation ability, and ease to the user. It does so by
 * keeping as much code and specification off of the user.
 *
 * How to use:
 * First, declare an VarStorage with a type specifier.
 * e.g.
 * VarStorage<double> var_double;
 *
 * Now you can set it in a variety of ways:
 *
 * 1). Setting to a constant
 * var_double = 10.0f;
 * var_double.set(10.0f);
 *
 * 2). Setting to a pointer
 * var_double = new double(10);
 * var_double.set(new double(10));
 * NOTE: VarStorage does not delete data when it is done, this is purely and example of what is possible (don't do this please)
 *
 * 3). Setting to a function that gives the output type
 * var_double = &get_double;
 * var_double = &Zen::Timer::get_current_time;
 *
 * 4). Setting to object/function pair that gives:
 * The output type OR another type of object that can implicitly or explicitly convert to the output type OR a lambda that fufills either previous requirements
 * var_double.set(new string("123"), &get_double_from_string);
 * var_double.set(new string("123"), &get_int_from_string);
 * var_double.set(new string("123"), [](const std::string& s) {
 *     return std::stod(s); // Note: stod can't be used directly because of default parameters causing mismatched function headers
 * });
 * NOTE: This is only for objects that will be continually updated, as if it is a constant, just convert and pass using option 1
 */
template<typename T>
class VarStorage {
private:
  T _constant = T(); // NOTE: Class T must have a default constructor
  std::function<T()> _get_val;

public:
  VarStorage() {
    *this = T();
  }

  explicit operator T() {
    return get_value();
  }

  template<typename type>
  friend std::ostream& operator<<(std::ostream& os, VarStorage<type>& obj) {
    os << obj.get_value();
    return os;
  }

  template<typename convertable_to_T>
  VarStorage<T>& operator=(const convertable_to_T& object) {
    _constant = object;
    _get_val = std::function<T()>([this]() -> T {
      return _constant;
    });
    return *this;
  }

  template<typename convertable_to_T>
  VarStorage<T>& operator=(convertable_to_T* object) {
    T data_test = T(*object);
    _get_val = std::function<T()>([object]() -> T {
      return T(object);
    });
    return *this;
  }

  VarStorage<T>& operator=(T (* funct)()) {
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

