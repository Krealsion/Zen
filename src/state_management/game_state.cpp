#include "game_state.h"

#include "game_state_manager.h"
#include "input.h"
#include "user_interface/custom_layout.h"

namespace Zen {
void GameState::exit() {
  _state_manager->exit();
}
void GameState::handle_input() {

}

GameState::GameState() {
  _root_layout = new CustomLayout();
  _root_layout->set_as_root();
  _root_layout->set_size(SizeTo::STATIC, 0, SizeTo::STATIC, 0);
  Input::set_active_window(get_window());
}

GameState::~GameState() {
  delete _root_layout;
}

void GameState::draw_gamestate(GameGraphics& g) {
  _root_layout->set_size(SizeTo::STATIC, _state_manager->get_renderer()->get_window()->get_width(),
                         SizeTo::STATIC, _state_manager->get_renderer()->get_window()->get_height());
  draw(g);
  _root_layout->draw(g);
}
}