//
// Created by jdemoss on 11/4/19.
//

#ifndef LEAVINGTERRA_TIMER_H
#define LEAVINGTERRA_TIMER_H

#include <atomic>
#include <chrono>

class Timer {
public:
    static void UpdateTime();
    /**
     * This sets whether UpdateTime() will be called.
     * If set to false, each check of IsTime() will update the CurrentTime
     * Not recommended for situations where order of IsTime() fires are important
     * @param ManualCalls True for calling UpdateTime() manually, False for Automatic CurrentTime Updates
     */
    static void SetManualUpdateManagement(bool ManualCalls);

    Timer(double Delay);

    /**
     * returns true if it is time to handle a tick
     * Only use this if the tick will be handled after the call
     * @return a bool representing if it is time for an tick
     */
    bool IsTime();
    /**
     * returns true if it is time to handle a tick
     * Use this if the tick will not be handled by the call
     * @return a bool representing if it is time for an tick
     */
    bool PeekIsTime();
    /**
     * @return a double representing the progress in milliseconds so far
     */
    double PeekProgress();
    /**
     * @return a double representing the progress in percent form. (0 = 0%, 1 = 100%)
     */
    double PeekProgressPercentage();

    double GetTimeMultiplier();
    void SetTimeMultiplier(double TimeMultiplier);

    bool IsPaused();
    void Pause();
    void Resume();

private:
    static std::chrono::time_point<std::chrono::steady_clock> CurrentTime;
    static bool ManualCalls;

    void UpdateEffectiveDelay();
    void SwitchPauseState();

    std::chrono::time_point<std::chrono::steady_clock> LastUpdate;
    std::chrono::nanoseconds EffectiveDelay;
    double Delay;
    double TimeMultiplier;
    bool Paused;
};

#endif //LEAVINGTERRA_TIMER_H
