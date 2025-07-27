#pragma once


#include "callback.h"
#include "game_state.h"
#include "timer.h"
#include "user_interface/text.h"
#include "user_interface/text_box.h"

namespace Zen {

class BuildState : public GameState {
public:
  BuildState();
  void update() override;
  void draw(GameGraphics& g) override;

  CustomLayout* create_create_pane();
  CustomLayout* get_component(Vector2 mouse_pos, CustomLayout* layout);
  CustomLayout* create_details_pane(CustomLayout* subject);
  CustomLayout* create_parent_child_tree(CustomLayout* node);
  CustomLayout* create_details_row(const std::string& label, TextBox::TextBoxFilter detail_type, const std::string& value, const Callback<void(const std::string&)>& callback);

  CustomLayout* create_basic_details(CustomLayout* subject);
  CustomLayout* create_button_details();
  CustomLayout* create_text_details();

  void update_highlight(CustomLayout* subject);

private:
  Timer update_timer;

  CustomLayout* _build_root = nullptr;

  CustomLayout* _highlight = nullptr;
  CustomLayout* _selection = nullptr;

  Text::Config _standard_text;
  int _detail_row_height = 32;
};

}
