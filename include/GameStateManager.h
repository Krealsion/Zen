#ifndef GAMESTATEMANAGER_H
#define GAMESTATEMANAGER_H

#include <vector>
#include <thread>

// Forward declaration, included in implementation
class GameState;

class GameGraphics {
}; //Layer between the user and SDL_Graphics TODO

/**
 * TODO for thread saftey, ensure that Draw does not draw null objects by having an itterator
 * On gamestate pop, all objects will be deleted, possibly partway through a render
 * Do not complete draw, and continue as normal
 * Atomic bool needed
 * Before Draw finish
 */
class GameStateManager {
public:
    explicit GameStateManager(GameState* InitialState);
    ~GameStateManager();

    //Methods for adding and removing GameStates
    void PushState(GameState* State);
    void PopState();

    bool IsRunning();
    void Exit();

protected:
    //Both of these methods are used internally by separate threads
    void Update();
    void Draw();

    std::thread UpdateThread;
    std::thread DrawThread;
    bool GameRunning;
    GameGraphics Graphics;
    std::vector<GameState*> GameStates;
};

#endif // GAMESTATEMANAGER_H
