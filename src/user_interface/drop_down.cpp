#include "drop_down.h"
#include "input.h"

namespace Zen {

DropDown::DropDown() {
  set_vertical();  // Overall layout vertical for display + panel

  // Display button with text
  _display_button = new Button();
  _display_button->set_size(SizeTo::PARENT, SizeTo::CHILDREN);
  _display_button->set_background_color(Color(200, 200, 200));  // Light gray default
  _display_button->set_auto_hover_color(true);
  _display_button->set_on_click_callback([this]() { _toggle_panel(); });
  this->add_child(_display_button);

  _display_text = new Text();
  _display_text->set_text("Select...");  // Default placeholder
  _display_text->set_font("Basic-Regular.ttf");
  _display_text->set_font_size(16.0f);
  _display_text->set_font_color(Color(0, 0, 0));
  _display_text->set_position(PositionTo::CENTER, PositionTo::CENTER);
  _display_button->add_child(_display_text);

  // Option panel (hidden ScrollView for long lists)
  _option_panel = new ScrollView();
  _option_panel->set_vertical();
  _option_panel->set_size(SizeTo::PARENT, 0, SizeTo::CHILDREN_PERCENT, 2.0);
  _option_panel->set_background_color(Color(255, 255, 255));
  _option_panel->set_visible(false);
  _option_panel->set_position(PositionTo::LEFT, PositionTo::BOTTOM);  // Below display
  this->add_child(_option_panel);
}

void DropDown::update() {
  CustomLayout::update();

  // Hide panel on click outside
  if (_panel_visible && Input::is_mouse_button_down(MouseButton::LEFT)) {
    auto mouse_pos = Input::get_mouse_position();
    if (!get_owned_destination().contains(mouse_pos) && !_option_panel->get_owned_destination().contains(mouse_pos)) {
      _hide_panel();
    }
  }
}

void DropDown::draw(GameGraphics& game_graphics) {
  CustomLayout::draw(game_graphics);
}

void DropDown::set_options(const std::vector<std::string>& options) {
  _options = options;
  _rebuild_options();
  if (_selected_index >= 0 && _selected_index < _options.size()) {
    _display_text->set_text(_options[_selected_index]);
  } else {
    _selected_index = -1;
    _display_text->set_text("Select...");
  }
}

void DropDown::set_selected_index(int index) {
  if (index >= 0 && index < _options.size()) {
    _selected_index = index;
    _display_text->set_text(_options[index]);
    on_selection_changed();
    _hide_panel();
  }
}

std::string DropDown::get_selected_text() const {
  if (_selected_index >= 0 && _selected_index < _options.size()) {
    return _options[_selected_index];
  }
  return "";
}

void DropDown::_toggle_panel() {
  _panel_visible = !_panel_visible;
  _option_panel->set_visible(_panel_visible);
}

void DropDown::_hide_panel() {
  _panel_visible = false;
  _option_panel->set_visible(false);
}

void DropDown::_rebuild_options() {
  _option_panel->remove_all_children();

  for (size_t i = 0; i < _options.size(); ++i) {
    auto* option_button = new Button();
    option_button->set_text(_options[i]);
    option_button->set_size(SizeTo::PARENT, SizeTo::CHILDREN);
    option_button->set_background_color(Color(220, 220, 220));
    option_button->set_auto_hover_color(true);

    // Capture index for callback
    int idx = static_cast<int>(i);
    option_button->set_on_click_callback([this, idx]() {
      set_selected_index(idx);
    });

    _option_panel->add_child(option_button);
  }
}

}  // namespace Zen
