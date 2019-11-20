#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "GameStateManager.h"

class GameGraphics; //TODO create class

class GameState {
public:
    virtual ~GameState(){}
    virtual void Update() = 0;
    virtual void Draw(GameGraphics G) {};
    virtual void Pause() {}
    virtual void Resume() {}
    virtual void SetGameStateManager(GameStateManager* StateManager){Manager = StateManager;}

protected:
    GameStateManager* Manager;
};

#endif // GAMESTATE_H
