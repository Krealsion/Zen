#pragma once

#include "graphics/game_graphics.h"

namespace Zen {
class GameStateManager;

class GameState {
public:
  virtual ~GameState() = default;
  virtual void update() = 0;
  virtual void draw(GameGraphics& g) = 0;

  virtual void pause() {}
  virtual void resume() {}
  virtual void set_game_state_manager(GameStateManager* state_manager) { this->_state_manager = state_manager; }

protected:
  GameStateManager* _state_manager = nullptr;
};
}

