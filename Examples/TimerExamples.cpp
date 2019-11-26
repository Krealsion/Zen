//
// Created by jdemoss on 11/6/19.
//

#include <iostream>
#include "Timer.h"

int main() {
    long x = 0;
    Timer t = Timer(1000);
    Timer t2 = Timer(20);
    while (x < 5) {
        Timer::UpdateTime();
        if (t2.IsTime()) {
            //Toggle the pause state every 20ms
            if (t.IsPaused()) {
                t.Resume();
            } else {
                t.Pause();
            }
        }
        std::cout << t.PeekProgressPercentage() << std::endl;
        if (t.IsTime()) {
            x += 1;
            //Decrease the speed of the timer, effectively adding 1 second every loop
            t.SetTimeMultiplier((double) 1 / (double) (x + 1));
            std::cout << x << std::endl;
            if (t2.IsPaused()) {
                t2.Resume();
            } else {
                t2.Pause();
            }
        }
    }

    // TESTING std::chrono::steady_clock::now() FOR PERFORMANCE
    long LoopCount = 100000000;
    auto start = std::chrono::steady_clock::now();
    for (long i = 0; i < LoopCount; i++) {
        //Empty loop to count loop time cost
    }
    auto end = std::chrono::steady_clock::now();
    auto time = end - start;
    double NsPerLoop = ((std::chrono::duration_cast<std::chrono::nanoseconds>(time).count()) / (double) LoopCount);

    start = std::chrono::steady_clock::now();
    for (long i = 0; i < LoopCount; i++) {
        auto time = std::chrono::steady_clock::now();
    }
    end = std::chrono::steady_clock::now();
    time = end - start;
    std::cout << "Each std::chrono::steady_clock::now() call took roughly " <<
              ((std::chrono::duration_cast<std::chrono::nanoseconds>(time).count()) / (double) LoopCount - NsPerLoop)
              << "ns.";
    return 0;
}
