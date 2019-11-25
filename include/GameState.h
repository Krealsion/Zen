//
// Created by jdemoss on 11/23/19.
//

#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "GameGraphics.h"

class GameStateManager;

class GameState {
public:
    virtual ~GameState() = default;
    virtual void Update() = 0;
    virtual void Draw(GameGraphics &G) = 0;
    virtual void Pause() {}
    virtual void Resume() {}
    virtual void SetGameStateManager(GameStateManager* StateManager){Manager = StateManager;}

protected:
    GameStateManager* Manager;
};

#endif // GAMESTATE_H
