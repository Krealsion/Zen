#pragma once

#include <functional>

namespace Zen {
/*
 * Anything storage is a kind of smart storage object that can house many different
 * objects that give an expected result.
 *
 * The purpose of this is to be a better concept of a pointer, while allowing diverse
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
  std::function<T(void*)>* get_val = nullptr;
  void* data = nullptr;
  std::function<T(void*, void*)>* conversion_function = nullptr;
  void* conversion_data = nullptr;
  T constant = T(); // NOTE: Class T must have a default constructor

public:
  AnythingStorage() {
    set_c(T());
  }

  operator T() {
    return get_value();
  }

  template<typename type>
  friend std::ostream& operator<<(std::ostream& os, AnythingStorage<type>& obj);

  void _clean() {
    if (get_val != nullptr) {
      delete get_val;
      get_val = nullptr;
    }
    if (conversion_function != nullptr) {
      delete conversion_function;
      conversion_function = nullptr;
    }
  }

  template<typename anythingthatconvertstoT>
  void set_c(anythingthatconvertstoT data) {
    _clean();
    constant = T(data);
    get_val = new std::function<T(void*)>([](void* data) -> T {
      return *(static_cast<T*>(data));
    });
    this->data = (void*) (&constant);
  }

  template<typename anythingthatconvertstoT>
  void set(anythingthatconvertstoT& data) {
    _clean();
    T data_test = T(data);
    get_val = new std::function<T(void*)>([](void* data) -> T {
      return T(*(static_cast<anythingthatconvertstoT*>(data)));
    });
    this->data = (void*) (&data);
  }

  template<typename anythingthatconvertstoTthroughafunction>
  void set(anythingthatconvertstoTthroughafunction& data, T (* conversion)(anythingthatconvertstoTthroughafunction)) {
    _clean();
    conversion_function = new std::function<T(void*, void*)>([](void* function_data, void* data) -> T {
      typedef T (* fptr)(anythingthatconvertstoTthroughafunction);
      fptr my_fptr = reinterpret_cast<fptr>(reinterpret_cast<long long>(function_data));
      return my_fptr(*(static_cast<anythingthatconvertstoTthroughafunction*>(data)));
    });
    this->data = (void*) (&data);
    conversion_data = (void*) (*conversion);
  }

  void set_f(T (* funct)()) {
    _clean();
    T type_test = funct();
    get_val = new std::function<T(void*)>([](void* data) -> T {
      typedef T (* fptr)();
      return reinterpret_cast<fptr>(reinterpret_cast<long long>(data))();
    });
    this->data = (void*) (*funct);
  }

  T get_value() {
    if (data == nullptr) return T();
    if (conversion_function != nullptr)
      return (*conversion_function)(conversion_data, data);
    return (*get_val)(data);
  }
};

template<typename T>
std::ostream& operator<<(std::ostream& os, AnythingStorage<T>& obj) {
  os << obj.get_value();
  return os;
}
}

