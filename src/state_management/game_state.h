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

  virtual void exit();
  virtual void pause() {}
  virtual void resume() {}

  Window* get_window() {
    return get_renderer()->get_window();
  }
  Renderer* get_renderer() {
    return GameStateManager::singleton().get_renderer();
  }

protected:
  CustomLayout* _root_layout = nullptr;
private:
  void draw_gamestate(GameGraphics& g);
};
}

