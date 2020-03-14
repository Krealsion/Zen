#include "timer.h"

namespace Zen {

std::chrono::time_point<std::chrono::steady_clock> Timer::_current_time = std::chrono::steady_clock::now();
std::chrono::time_point<std::chrono::steady_clock> Timer::_start_time = std::chrono::steady_clock::now();
bool Timer::_automatic_updates = true;

void Timer::update_time() {
  _current_time = std::chrono::steady_clock::now();
}

void Timer::set_automatic_updates(bool automatic_updates) {
  Timer::_automatic_updates = automatic_updates;
}

Timer::Timer(double delay) {
  _delay = delay;
  _time_multiplier = 1;
  _update_effective_delay();
  _last_update = _current_time;
  _paused = false;
}

bool Timer::is_time() {
  if (peek_is_time()) {
    _last_update += _effective_delay;
    return true;
  }
  return false;
}

bool Timer::peek_is_time() {
  if (_paused) {
    return false;
  }
  if (_automatic_updates) {
    update_time();
  }
  return std::chrono::duration_cast<std::chrono::nanoseconds>(_current_time - _last_update) >= _effective_delay;
}

double Timer::peek_progress() {
  if (_paused) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(_last_update.time_since_epoch()).count();
  }
  return std::chrono::duration_cast<std::chrono::milliseconds>(_current_time - _last_update).count();
}

double Timer::peek_progress_pecentage() {
  return peek_progress() / std::chrono::duration_cast<std::chrono::nanoseconds>(_effective_delay).count() * 1000000;
}

double Timer::get_time_multiplier() {
  return _time_multiplier;
}

void Timer::set_time_multiplier(double TimeMultiplier) {
  if (_paused) {
    _last_update = _last_update - std::chrono::nanoseconds((long long) (
            _last_update.time_since_epoch().count() * (1 - (this->_time_multiplier / TimeMultiplier))));
  } else {
    //Subtract time to set the relative time progress to an equal percentage
    _last_update = _current_time - std::chrono::nanoseconds((long long) (
            (_current_time.time_since_epoch().count() - _last_update.time_since_epoch().count()) *
            this->_time_multiplier / TimeMultiplier));
  }
  this->_time_multiplier = TimeMultiplier;
  _update_effective_delay();
}

bool Timer::is_paused() {
  return _paused;
}

void Timer::pause() {
  if (!_paused) {
    _switch_pause_states();
  }
}

void Timer::resume() {
  if (_paused) {
    _switch_pause_states();
  }
}

void Timer::_update_effective_delay() {
  _effective_delay = std::chrono::nanoseconds((long long) (_delay / _time_multiplier * 1000000));
}

void Timer::_switch_pause_states() {
  _paused = !_paused;
  _last_update = _current_time - _last_update.time_since_epoch();
}

double Timer::get_current_time() {
  if (_automatic_updates)
    update_time();
  return _current_time.time_since_epoch().count();
}

double Timer::get_nanoseconds_since_started() {
  if (_automatic_updates)
    update_time();
  return std::chrono::duration_cast<std::chrono::nanoseconds>((_current_time - _start_time.time_since_epoch()).time_since_epoch()).count();
}

double Timer::get_microseconds_since_started() {
  if (_automatic_updates)
    update_time();
  return get_nanoseconds_since_started() / 1000;
}

double Timer::get_milliseconds_since_started() {
  if (_automatic_updates)
    update_time();
  return get_microseconds_since_started() / 1000;
}

double Timer::get_seconds_since_started() {
  if (_automatic_updates)
    update_time();
  return get_milliseconds_since_started() / 1000;
}
}
