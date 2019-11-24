//
// Created by jdemoss on 11/23/19.
//

#ifndef GAMESTATE_H
#define GAMESTATE_H

class GameGraphics;
class GameStateManager;

class GameState {
public:
    virtual ~GameState() = 0;
    virtual void Update() = 0;
    virtual void Draw(GameGraphics G) {};
    virtual void Pause() {}
    virtual void Resume() {}
    virtual void SetGameStateManager(GameStateManager* StateManager){Manager = StateManager;}

protected:
    GameStateManager* Manager;
};

#endif // GAMESTATE_H
