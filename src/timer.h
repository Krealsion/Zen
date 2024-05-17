#pragma once

#include <atomic>
#include <chrono>
#include <iostream>

namespace Zen {

class Timer {
public:
  explicit Timer(double delay);
  /**
   * Updates the CurrentTime, as understood by the timer.
   * Required to be called at the beginning of an update cycle if
   * SetAutomaticUpdateManagement(true) was not called.
   */
  static void update_time();

  /**
   * This sets whether update_time() will be called.
   * If set to true, each check of is_time() will update the CurrentTime.
   * Not recommended for situations where order of is_time() fires are important.
   * @param Automatic False for calling update_time() manually, True for Automatic CurrentTime Updates
   */
  static void set_automatic_updates(bool automatic);


  /**
   * returns true if it is time to handle a tick
   * Only use this if the tick will be handled after the call
   * @return a bool representing if it is time for an tick
   */
  bool is_time();

  /**
   * returns true if it is time to handle a tick
   * Use this if the tick will not be handled by the call
   * @return a bool representing if it is time for an tick
   */
  bool peek_is_time();

  /**
   * @return a double representing the progress in milliseconds so far
   */
  double peek_progress();

  /**
   * @return a double representing the progress in percent form. (0 = 0%, 1 = 100%)
   */
  double peek_progress_percentage();

  [[nodiscard]] double get_time_multiplier() const;

  void set_time_multiplier(double time_multiplier);

  [[nodiscard]] bool is_paused() const;

  void pause();

  void resume();

  static long long get_current_time();
  static long long get_nanoseconds_since_started();
  static long long get_microseconds_since_started();
  static long long get_milliseconds_since_started();
  static long long get_seconds_since_started();

private:
  //The current time as understood by the timer class (Not always up to date)
  static std::chrono::time_point<std::chrono::steady_clock> _current_time;
  static std::chrono::time_point<std::chrono::steady_clock> _start_time;
  static bool _automatic_updates;
  //Represents the time when the last tick was supposed to fire
  std::chrono::time_point<std::chrono::steady_clock> _last_update;
  std::chrono::nanoseconds _effective_delay;
  double _delay;
  double _time_multiplier;
  bool _paused;

  void _update_effective_delay();
  void _switch_pause_states();
};
}
