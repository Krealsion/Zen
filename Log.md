# Format (subject to change):
## (Log Number) (Date): (Log Subject)

#### Problem:
(Short description of the problem)

#### Requirements:
(List of requirements the solution needs to address)

#### Dev Notes:
(Notes about the process of coming up with the solution, general, not all comprehensive)

#### Solution:
(Short description of the solution, as well as an example of the use case if applicable)

#### Final Thoughts:
(List of potential future problems or improvements, or important information)
___
## 0 11/6/2019: Timer Time! (11/4 - 11/6)
##### Terms: 
Timer - A object able to handle running code routinely at different intervals on the same thread
#### Problem:
Timers are not something pre-present in c++. Some implementations allow for similar functionality via either system sleep calls or alternate threads. The problem with these are that Sleep calls halt all other processes on the thred until that time has passed, while using other threads causes: async behaviour, abiguity, race conditions, and an overall more complex codebase.
#### Requirements:
-Needs to be able to handle long and short time frames. Time frames less than 1ms should be possible

-Needs to handle the same time intervals separately

    e.g. Two timers of the same interval of 1 second should be able to fire at different times
    
-The timers should be able to fire in an order dictated by the program, aka if multiple timers are ready to fire, the order in which they fire is able to be controlled

-Needs to be able to have modified timescales after creation, either by changing the delay, or adding a time multiplier (Slow motion, hooooo!)

-Should be able to check progress via percentages, milliseconds, and if it should fire

-Pausing and resuming
#### Dev Notes: 
Given that high precision is required, chrono will be used to keep track of nanoseconds, while the general input/output will be measured in ms, for precision.

Avoiding all threads and sleeps will allow to keep the use case very simple and easy to use.

Avoid internal measurements of time by keeping a static "CurrentTime" - Posed a challenge for time modification, as you need to work off a modified delay instead of modified internal time, causing a switch. Lower space complexity, larger time complexity(rederiving effective delay instead of modifying tick speed)

Pausing posed some potential space waste, as the duration passed since last tick was not compatable with the time_point stored in LastUpdate, which is ideal for storing the time passed when paused, but it was possible to set the time_point to contain just the duration passed

**My favorite problem** Setting a new time modifier was difficult as time passed was not being tracked against a static delay. A dynamic "Effective Delay" made switching the TimeMultiplier not a percentage consistent change when simply updating the multiplier. In order to keep it consistent, the function now also either adds or subtracts time from the "LastUpdate" tracker to maintain the correct percentage when the TimeMultiplier is updated.

An option for manual calls was added, though still under consideration, as it just adds another static field and adds a very small amount of time complexity overall, while only supporting a minor edge case for overall project implementation styles
#### Solution:
The final product is an object that has configurable initial delay, TimeMultiplier, and pause status while adding no new threads or sleep counters to the program. In order to implement a small overhead is necessary once followed by, a declaration and definition for a timer, and a simple one line implementation around the code to be run. e.g.

```c++
#include <iostream>

#include <Timer.h>

int main() {
    std::cout << "Liftoff in " << std::flush;
    Timer t = Timer(1000);
    for (int i = 5;;) {
        Timer::UpdateTime();    //This line updates the global current time REQUIRED
        if (t.IsTime()) {
            if (i == 0)
                break;
            std::cout << i-- << "... " << std::flush;
        }
    }
    std::cout << " Liftoff!" << std::flush;
    return 0;
}
```
#### Final Thoughts:
-ManualUpdate management option seems unnecessary, as it can change predictable functionality, as well as adds overall time complexity

-Nanosecond tracking is overkill for the intended use of the timer

-Keeping the EffectiveDelay as a separate variable would help time complexity, at the expense of space complexity(probably shouldn't care about an extra few bytes)
___
## 1 11/12/2019: GameStates and Management
#### Problem: 
Encapsulation as well as different states/menus of a game are crucially important for readability
#### Requirements: 
GameStates should be easy to create, and simple to implement.

The StateManager should only support the previous requirements, and otherwise work in the backend.

The StateManager should support multithreaded updates and drawing, as to allow for smoother execution.
#### Dev Notes: 
The general format for GameState and GameStateManager came from previous projects utilizing similar functionality, one in c++ and one in Java, and boy did I really underestimate the importance of having those classes be simple in c++ comparatively to Java. Thus the general structure and requirements were created.

GameStates should only really need to have one method which is update. If the user just wants a black splash page (which will be the default draw beneath every frame) they can do so. The only requirement for a GameState is that it provides some functionality, and has some way to either exit the game, add a new gamestate, or pop itself out of the gamestate queue.

Having the threads access IsRunning to determine if they should continue allowed for adding a join clause after the creation in the constructor, essentially encapsulating their functionality to their respective tasks, and alleviating the need for any kind of external wait to have the game run.
#### Solution: 
The current solution meets all the requirements necessary, and is designed to scale. The GameState has multiple virtual methods that allows for easy access to functionality used behind the scenes (pause, resume, update, draw, ect) which do not get called by the user.
```c++
#include <iostream>

#include <GameStateManager.h>
#include <GameState.h>

class Gs : public GameState{
public:
    void Update(){
        static int i = 0;
        std::cout << i++ << std::endl;
    }
};


int main() {
    GameState* S = new Gs();
    GameStateManager sm = GameStateManager(S);
}
```

#### Final Thoughts:
This ended up being a rather basic implementation which will most assuredly need expansion upon later.

One potential problem is regarding popping states. When a GameState is popped, there may be a need to sync up the threads to prevent deleting data that is in use.