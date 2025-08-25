#pragma once

#include <functional>
#include <utility>

// A generic Callback class that can store and invoke any callable object,
// including lambdas, function pointers, or functors. It uses std::function
// internally for type erasure and flexibility.

template <typename Ret, typename... Args>
class Callback {
private:
  std::function<Ret(Args...)> m_func;

public:
  // Default constructor: creates an empty callback (invoking it will do nothing or throw if not set).
  Callback() = default;

  // Constructor that takes any callable (e.g., lambda) matching the signature.
  template <typename Func>
  Callback(Func&& func) : m_func(std::move(func)) {}

  // Assignment operator for reassigning a new callable.
  template <typename Func>
  Callback& operator=(Func&& func) {
    m_func = std::forward<Func>(func);
    return *this;
  }

  // Invoke the callback with the provided arguments.
  Ret operator()(Args... args) const {
    if (m_func) {
      return m_func(std::forward<Args>(args)...);
    }
    // If no function is set, handle gracefully (e.g., return default-constructed Ret).
    // For void return, this is a no-op.
    return Ret{};
  }

  // Check if the callback is set.
  explicit operator bool() const {
    return static_cast<bool>(m_func);
  }
};

// Specialization for void return type to avoid returning anything.
template <typename... Args>
class Callback<void, Args...> {
private:
  std::function<void(Args...)> m_func;

public:
  Callback() = default;

  template <typename Func>
  explicit Callback(Func&& func) : m_func(std::move(func)) {}

  template <typename Func>
  Callback& operator=(Func&& func) {
    m_func = std::forward<Func>(func);
    return *this;
  }

  void operator()(Args... args) const {
    if (m_func) {
      m_func(std::forward<Args>(args)...);
    }
    // No-op if not set.
  }

  explicit operator bool() const {
    return static_cast<bool>(m_func);
  }
};

// Alias for void-returning callbacks to simplify syntax (inspired by C# Action).
// This makes non-returning callbacks less syntax-intensive: no need to specify 'void'.
template <typename... Args>
using Action = Callback<void, Args...>;
