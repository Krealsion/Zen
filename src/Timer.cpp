//
// Created by jdemoss on 11/4/19.
//

#include "../include/Timer.h"

std::chrono::time_point<std::chrono::steady_clock> Timer::CurrentTime = std::chrono::steady_clock::now();
bool Timer::ManualCalls = true;

void Timer::UpdateTime() {
    CurrentTime = std::chrono::steady_clock::now();
}

void Timer::SetManualUpdateManagement(bool ManualCalls) {
    Timer::ManualCalls = ManualCalls;
}

Timer::Timer(double Delay) {
    this->Delay = Delay;
    TimeMultiplier = 1;
    UpdateEffectiveDelay();
    LastUpdate = CurrentTime;
    Paused = false;
}

bool Timer::IsTime() {
    if (PeekIsTime()) {
        LastUpdate += EffectiveDelay;
        return true;
    }
    return false;
}

bool Timer::PeekIsTime() {
    if (Paused) {
        return false;
    }
    if (!ManualCalls) {
        UpdateTime();
    }
    return std::chrono::duration_cast<std::chrono::nanoseconds>(CurrentTime - LastUpdate) >= EffectiveDelay;
}

double Timer::PeekProgress() {
    if (Paused) {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(LastUpdate.time_since_epoch()).count() / 1000000.0;
    }
    return std::chrono::duration_cast<std::chrono::nanoseconds>(CurrentTime - LastUpdate).count() / 1000000.0;
}

double Timer::PeekProgressPercentage() {
    return PeekProgress() / std::chrono::duration_cast<std::chrono::nanoseconds>(EffectiveDelay).count() * 1000000;
}

double Timer::GetTimeMultiplier() {
    return TimeMultiplier;
}

void Timer::SetTimeMultiplier(double TimeMultiplier) {
    if (Paused) {
        LastUpdate = LastUpdate - std::chrono::nanoseconds((long) (
                LastUpdate.time_since_epoch().count() * (1 - (this->TimeMultiplier / TimeMultiplier))));
    } else {
        //Subtract time to set the relative time progress to an equal percentage
        LastUpdate = CurrentTime - std::chrono::nanoseconds((long) (
                (CurrentTime.time_since_epoch().count() - LastUpdate.time_since_epoch().count()) *
                this->TimeMultiplier / TimeMultiplier));
    }
    this->TimeMultiplier = TimeMultiplier;
    UpdateEffectiveDelay();
}

bool Timer::IsPaused() {
    return Paused;
}

void Timer::Pause() {
    if (!Paused) {
        SwitchPauseState();
    }
}

void Timer::Resume() {
    if (Paused) {
        SwitchPauseState();
    }
}

void Timer::UpdateEffectiveDelay() {
    EffectiveDelay = std::chrono::nanoseconds((long) (Delay / TimeMultiplier * 1000000));
}

/**
 * Internal Function
 * Swaps the Pause state by either storing or restoring the time progressed since the Pause
 */
void Timer::SwitchPauseState() {
    Paused = !Paused;
    LastUpdate = CurrentTime - LastUpdate.time_since_epoch();
}

