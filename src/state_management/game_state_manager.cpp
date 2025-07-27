#include "game_state_manager.h"

#include "input.h"
#include "game_state.h"
#include "timer.h"

namespace Zen {

GameStateManager::GameStateManager(GameState* initial_state) : _renderer("Game", Rectangle(20, 20, 400, 400)) {
  push_state(initial_state);
  _running = true;
  _update_thread = std::thread([&]() {
    while (is_running())
      update();
  });
  _draw_thread = std::thread([&]() {
    while (is_running())
      draw();
  });
  KeyCombo AltF4 = {SDL_SCANCODE_F4, {SDL_SCANCODE_LALT}};
  while (is_running()) {
    if (Input::is_key_down(AltF4)) {
      exit(); // Exit on Alt + F4
    }
    Input::update_input();
  }
  _update_thread.join();
  _draw_thread.join();
}

GameStateManager::~GameStateManager() {
  while (!_game_states.empty()) {
    delete _game_states.back();
    _game_states.pop_back();
  }
}

void GameStateManager::update() {
  Timer::update_time();
  if (!_game_states.empty()) {
    _game_states.back()->update();
  }
  Input::clean(); // Clean up input after each update
}

void GameStateManager::draw() {
  if (!_game_states.empty()) {
    _game_states.back()->draw_gamestate(_graphics);
  }
  _renderer.render_game_graphics(_graphics);
}

void GameStateManager::push_state(GameState* state) {
  state->set_game_state_manager(this);
  if (!_game_states.empty()) {
    _game_states.back()->pause();
  }
  _game_states.push_back(state);
}

void GameStateManager::pop_state() {
  if (!_game_states.empty()) {
    GameState* back = _game_states.back();
    _game_states.pop_back();
    delete back;
    if (!_game_states.empty()) {
      _game_states.back()->resume();
    }
  }
}

bool GameStateManager::is_running() const {
  return _running;
}

void GameStateManager::exit() {
  _running = false;
}

Renderer* GameStateManager::get_renderer() {
  return &_renderer;
}
}
