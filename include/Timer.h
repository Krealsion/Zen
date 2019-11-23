//
// Created by jdemoss on 11/4/19.
//

#ifndef TIMER_H
#define TIMER_H

#include <atomic>
#include <chrono>

class Timer {
public:
    /**
     * Updates the CurrentTime, as understood by the timer.
     * Required to be called at the beginning of an update cycle if
     * SetAutomaticUpdateManagement(true) was not called.
     */
    static void UpdateTime();
    /**
     * This sets whether UpdateTime() will be called.
     * If set to true, each check of IsTime() will update the CurrentTime.
     * Not recommended for situations where order of IsTime() fires are important.
     * @param Automatic False for calling UpdateTime() manually, True for Automatic CurrentTime Updates
     */
    static void SetAutomaticUpdates(bool Automatic);

    explicit Timer(double Delay);

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
    //The current time as understood by the timer class (Not always up to date)
    static std::chrono::time_point<std::chrono::steady_clock> CurrentTime;
    static bool AutomaticUpdates;

    void UpdateEffectiveDelay();
    void SwitchPauseState();

    //Represents the time when the last tick was supposed to fire
    std::chrono::time_point<std::chrono::steady_clock> LastUpdate;
    std::chrono::nanoseconds EffectiveDelay;
    double Delay;
    double TimeMultiplier;
    bool Paused;
};

#endif //TIMER_H
