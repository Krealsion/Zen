#include "timer.h"

namespace Zen {

std::chrono::time_point<std::chrono::steady_clock> Timer::current_time = std::chrono::steady_clock::now();
std::chrono::time_point<std::chrono::steady_clock> Timer::start_time = std::chrono::steady_clock::now();
bool Timer::automatic_updates = true;

void Timer::update_time() {
  current_time = std::chrono::steady_clock::now();
}

void Timer::set_automatic_updates(bool automatic_updates) {
  Timer::automatic_updates = automatic_updates;
}

Timer::Timer(double Delay) {
  this->delay = Delay;
  time_multiplier = 1;
  update_effective_delay();
  last_update = current_time;
  paused = false;
}

bool Timer::is_time() {
  if (peek_is_time()) {
    last_update += effective_delay;
    return true;
  }
  return false;
}

bool Timer::peek_is_time() {
  if (paused) {
    return false;
  }
  if (automatic_updates) {
    update_time();
  }
  return std::chrono::duration_cast<std::chrono::nanoseconds>(current_time - last_update) >= effective_delay;
}

double Timer::peek_progress() {
  if (paused) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(last_update.time_since_epoch()).count();
  }
  return std::chrono::duration_cast<std::chrono::milliseconds>(current_time - last_update).count();
}

double Timer::peek_progress_pecentage() {
  return peek_progress() / std::chrono::duration_cast<std::chrono::nanoseconds>(effective_delay).count() * 1000000;
}

double Timer::get_time_multiplier() {
  return time_multiplier;
}

void Timer::set_time_multiplier(double TimeMultiplier) {
  if (paused) {
    last_update = last_update - std::chrono::nanoseconds((long long) (
            last_update.time_since_epoch().count() * (1 - (this->time_multiplier / TimeMultiplier))));
  } else {
    //Subtract time to set the relative time progress to an equal percentage
    last_update = current_time - std::chrono::nanoseconds((long long) (
            (current_time.time_since_epoch().count() - last_update.time_since_epoch().count()) *
            this->time_multiplier / TimeMultiplier));
  }
  this->time_multiplier = TimeMultiplier;
  update_effective_delay();
}

bool Timer::is_paused() {
  return paused;
}

void Timer::pause() {
  if (!paused) {
    switch_pause_states();
  }
}

void Timer::resume() {
  if (paused) {
    switch_pause_states();
  }
}

void Timer::update_effective_delay() {
  effective_delay = std::chrono::nanoseconds((long long) (delay / time_multiplier * 1000000));
}

void Timer::switch_pause_states() {
  paused = !paused;
  last_update = current_time - last_update.time_since_epoch();
}
double Timer::get_current_time() {
  if (automatic_updates)
    update_time();
  return current_time.time_since_epoch().count();
}
double Timer::get_nanoseconds_since_started() {
  if (automatic_updates)
    update_time();
  return std::chrono::duration_cast<std::chrono::nanoseconds>((current_time - start_time.time_since_epoch()).time_since_epoch()).count();
}
double Timer::get_microseconds_since_started() {
  if (automatic_updates)
    update_time();
  return get_nanoseconds_since_started() / 1000;
}
double Timer::get_milliseconds_since_started() {
  if (automatic_updates)
    update_time();
  return get_microseconds_since_started() / 1000;
}
double Timer::get_seconds_since_started() {
  if (automatic_updates)
    update_time();
  return get_milliseconds_since_started() / 1000;
}
}
