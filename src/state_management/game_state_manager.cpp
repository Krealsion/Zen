#include "game_state_manager.h"

#include "input.h"
#include "game_state.h"
#include "timer.h"

#include "user_interface/custom_layout.h"

namespace Zen {

void GameStateManager::initialize_with_state(GameState* initial_state) {
  if (initial_state == nullptr) {
    throw std::runtime_error("Initial game state cannot be null");
  }
  push_state(initial_state);
  _running = true;
  _update_thread = std::thread([&]() {
    while (is_running())
      update();
  });
//  _draw_thread = std::thread([&]() {
//    while (is_running())
//  });
  KeyCombo AltF4 = {SDL_SCANCODE_F4, {SDL_SCANCODE_LALT}};
  while (is_running()) {
    if (Input::is_key_down(AltF4)) {
      exit(); // Exit on Alt + F4
    }
    Input::update_input();
    draw();
    SDL_Delay(16); // Roughly 60 FPS
  }
  _update_thread.join();
//  _draw_thread.join();
}

GameStateManager::GameStateManager() : _renderer("", Rectangle(0, 0, 1820, 980)) {
  Input::init();
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
    _game_states.back()->_root_layout->update();
  }
  Input::clean_input();
}

void GameStateManager::draw() {
  if (!_game_states.empty()) {
    _game_states.back()->draw_gamestate(_graphics);
  }
  _renderer.render_game_graphics(_graphics);
}

void GameStateManager::push_state(GameState* state) {
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
