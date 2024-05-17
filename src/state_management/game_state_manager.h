#pragma once

#include "graphics/game_graphics.h"
#include "graphics/renderer.h"

#include <vector>
#include <thread>

namespace Zen {

class GameState;

/**
 * TODO for thread saftey, ensure that draw does not draw null objects by having an itterator
 * On gamestate pop, all objects will be deleted, possibly partway through a render
 * Do not complete draw, and continue as normal
 * Atomic bool needed
 * Before draw finish
 */
class GameStateManager {
public:
  explicit GameStateManager(GameState* initial_state);

  ~GameStateManager();

  void push_state(GameState* state);
  void pop_state();
  [[nodiscard]] bool is_running() const;
  void exit();

  Renderer* get_renderer();

protected:
  //Both of these methods are used internally by separate threads
  void update();

  void draw();

  std::thread _update_thread;
  std::thread _draw_thread;
  bool _running;
  Renderer _renderer;
  GameGraphics _graphics;
  std::vector<GameState*> _game_states;
};
}

