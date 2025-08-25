#pragma once

#include "callback.h"
#include "state_management/game_state.h"
#include "timer.h"
#include "user_interface/text.h"
#include "user_interface/text_box.h"
#include "user_interface/scroll_view.h"
#include "user_interface/drop_down.h"
#include "logic/utils.h"  // For demangle

#include <nlohmann/json.hpp>
#include <memory>

namespace Zen {

class BuildState : public GameState {
public:
  BuildState();
  ~BuildState() override;
  void update() override;
  void update_build_root();
  void draw(GameGraphics& g) override;
  CustomLayout* get_component(Vector2 mouse_pos, CustomLayout* layout, bool only_callback_registered = false);

  // Create UI panels
  CustomLayout* create_create_pane();
  CustomLayout* create_component_tree(CustomLayout* node);
  CustomLayout* create_details_pane(CustomLayout* subject);

  // Save/load UI hierarchy to/from JSON
  void save_to_json(const std::string& file_path);
  void load_from_json(const std::string& file_path);

private:
  // Helpers for details panes
  CustomLayout* create_basic_details(CustomLayout* subject);
  CustomLayout* create_button_details(Button* button);
  CustomLayout* create_text_details(Text* text);
  CustomLayout* create_textbox_details(TextBox* textbox);
  CustomLayout* create_scrollview_details(ScrollView* scrollview);
  CustomLayout* create_details_row(const std::string& label, TextBox::TextBoxFilter filter, const std::string& value, Action<const std::string&>&& callback);
  CustomLayout* create_dropdown_row(const std::string& label, const std::vector<std::string>& options, int selected_index, Action<int>&& callback);

  // Component selection and highlighting
  void update_highlight();
  void enter_selection_mode(CustomLayout* selection);
  void enter_creation_mode();
  void delete_selection();

  // JSON serialization for components
  nlohmann::json serialize_component(CustomLayout* component);
  std::unique_ptr<CustomLayout> deserialize_component(const nlohmann::json& j);

  // Refresh UI after changes
  void refresh_details();
  void refresh_tree();

  Timer update_timer;
  std::unique_ptr<CustomLayout> _build_root;
  CustomLayout* _highlight = nullptr;
  CustomLayout* _selection = nullptr;
  Text::Config _standard_text;
  int _detail_row_height = 32;

  // UI panels
  std::unique_ptr<ScrollView> _create_pane;
  std::unique_ptr<ScrollView> _tree_pane;
  std::unique_ptr<ScrollView> _details_pane;
};

}  // namespace Zen
