#include "game_state.h"

#include "game_state_manager.h"
#include "input.h"
#include "user_interface/custom_layout.h"

namespace Zen {
void GameState::exit() {
}

GameState::GameState() {
  Input::set_active_window(get_window());
  _root_layout = new CustomLayout();
  _root_layout->set_as_root();
  _root_layout->set_size(SizeTo::STATIC, 0, SizeTo::STATIC, 0);
}

GameState::~GameState() {
  delete _root_layout;
}

void GameState::draw_gamestate(GameGraphics& g) {
  _root_layout->set_size(SizeTo::STATIC, get_window()->get_width(),
                         SizeTo::STATIC, get_window()->get_height());
  draw(g);
  _root_layout->draw(g);
}
}