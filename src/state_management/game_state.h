#pragma once

#include "graphics/game_graphics.h"
#include "game_state_manager.h"


namespace Zen {
class GameStateManager;
class CustomLayout;

class GameState {
friend class GameStateManager;

public:
  GameState();
  virtual ~GameState();
  virtual void update() = 0;
  virtual void draw(GameGraphics& g) = 0;
  void handle_input();

  virtual void exit();
  virtual void pause() {}
  virtual void resume() {}
  virtual void set_game_state_manager(GameStateManager* state_manager) { this->_state_manager = state_manager; }

protected:
  CustomLayout* _root_layout = nullptr;
  GameStateManager* _state_manager = nullptr;
private:
  void draw_gamestate(GameGraphics& g);
};
}

