#include "build_state.h"


#include "callback.h"
#include "user_interface/custom_layout.h"

#include <input.h>

#include <cmath>

#include "user_interface/button.h"
#include "user_interface/text_box.h"
#include "user_interface/scroll_view.h"

using namespace Zen;

BuildState::BuildState() : update_timer(16){
  _root_layout->set_vertical();
  _root_layout->set_child_spacing(16);

  _standard_text.font_size = 16;
  _standard_text.font_name = "Basic-Regular.ttf";
  _standard_text.font_color = Color(0, 0, 0);
}

CustomLayout* BuildState::create_create_pane() {
  auto* layout = new CustomLayout();
  layout->set_vertical();

  auto* create_layout_button = new Button();
  create_layout_button->set_text("Create Layout");
  create_layout_button->set_size(SizeTo::PARENT, 0, SizeTo::CHILDREN, 0);
  create_layout_button->set_on_click_callback(std::function<void()>([this]() {
    if (_selection != nullptr) {
      auto* layout = new CustomLayout();
      _selection->add_child(layout);
    }
  }));
  layout->add_child(create_layout_button);

  auto* create_button_button = new Button();
  create_button_button->set_text("Create Button");
  create_button_button->set_size(SizeTo::PARENT, 0, SizeTo::CHILDREN, 0);
  create_button_button->set_on_click_callback(std::function<void()>([this]() {
    if (_selection != nullptr) {
      auto* button = new Button();
      _selection->add_child(button);
    }
  }));
  layout->add_child(create_button_button);

  auto* create_text_button = new Button();
  create_text_button->set_text("Create Text");
  create_text_button->set_size(SizeTo::PARENT, 0, SizeTo::CHILDREN, 0);
  create_text_button->set_on_click_callback(std::function<void()>([this]() {
    if (_selection != nullptr) {
      auto* text = new Text();
      _selection->add_child(text);
    }
  }));
  layout->add_child(create_text_button);

  auto* create_textbox_button = new Button();
  create_textbox_button->set_text("Create Textbox");
  create_textbox_button->set_size(SizeTo::PARENT, 0, SizeTo::CHILDREN, 0);
  create_textbox_button->set_on_click_callback(std::function<void()>([this]() {
    if (_selection != nullptr) {
      auto* textbox = new TextBox();
      _selection->add_child(textbox);
    }
  }));
  layout->add_child(create_textbox_button);

  auto* create_scrollview_button = new Button();
  create_scrollview_button->set_text("Create Textbox");
  create_scrollview_button->set_size(SizeTo::PARENT, 0, SizeTo::CHILDREN, 0);
  create_scrollview_button->set_on_click_callback(std::function<void()>([this]() {
    if (_selection != nullptr) {
      auto* scrollview = new ScrollView();
      _selection->add_child(scrollview);
    }
  }));
  layout->add_child(create_scrollview_button);

  return layout;
}

CustomLayout* BuildState::get_component(Vector2 mouse_pos, CustomLayout* layout) {
  for (auto& child : layout->_children) {
    if (child->is_visible() && child->get_owned_destination().contains(mouse_pos)) {
      return get_component(mouse_pos, child);
    }
  }
  return layout;
}

CustomLayout* BuildState::create_details_row(const std::string& label, TextBox::TextBoxFilter detail_type, const std::string& value, const Callback<void(const std::string&)>& callback) {
  auto layout = new CustomLayout();
  layout->set_size(SizeTo::PARENT, 0, SizeTo::STATIC, _detail_row_height);

  auto label_text = new Text();
  label_text->set_text(label);
  label_text->set_size(SizeTo::CHILDREN, SizeTo::PARENT);
  label_text->set_position(PositionTo::LEFT, PositionTo::CENTER);

  auto value_text = new TextBox();
  value_text->set_text(value);
  value_text->set_size(SizeTo::CHILDREN, SizeTo::PARENT);
  value_text->set_position(PositionTo::RIGHT, PositionTo::CENTER);
  value_text->set_filter(detail_type);
  value_text->on_text_changed.connect(this, [value_text, &callback]() {
    callback(value_text->get_text());
  });

  layout->add_child(label_text);
  layout->add_child(value_text);

  return layout;
}

CustomLayout* BuildState::create_details_pane(CustomLayout* subject) {
  auto layout = new CustomLayout();
  layout->set_vertical();
  return layout;
}

CustomLayout* BuildState::create_basic_details(CustomLayout* subject) {
  auto layout = new CustomLayout();
  layout->set_vertical();
  layout->set_child_spacing(8);
  layout->set_padding(8, 8, 8, 8);
  layout->set_size(SizeTo::PARENT, SizeTo::CHILDREN);

  layout->add_child(create_details_row("Name", TextBox::TextBoxFilter(), subject->get_name()));
  layout->add_child(create_details_row("Width", TextBox::TextBoxFilter(), subject->get_name()));
  // Height
  // Position
  // Padding
  // Margin
  // Background Color
  // Layout
  // child spacing
  // callbacks
  // border

  return layout;
}
CustomLayout* BuildState::create_button_details() {
  auto layout = new CustomLayout();
  //
  return layout;
}
CustomLayout* BuildState::create_text_details() {
  auto layout = new CustomLayout();
  return layout;
}

void BuildState::update_highlight(CustomLayout* subject) {
  if (!subject) {
    _highlight = nullptr;
    return;
  }
  auto* new_highlight = get_component(Input::get_mouse_position(), _root_layout);
  auto highlight_destination = new_highlight->get_owned_destination();
  if (_highlight) {
    _highlight->set_position(PositionTo::RELATIVE, new_highlight->get_position().get_x(),
                             PositionTo::RELATIVE, new_highlight->get_position().get_y());
  } else {
    _highlight = new CustomLayout();
    _highlight->set_background_color(Color(255, 0, 0, 128));
    _highlight->set_position(PositionTo::RELATIVE, new_highlight->get_position().get_x(),
                             PositionTo::RELATIVE, new_highlight->get_position().get_y());
    _root_layout->add_child(_highlight);
  }
}

void BuildState::update() {
  Input::update_input();
  if (update_timer.is_time()) {
    if (Input::is_key_down(SDL_SCANCODE_ESCAPE)) {
      this->exit();
    }

    if (Input::is_mouse_button_down(MouseButton::LEFT)) {
      auto mouse_pos = Input::get_mouse_position();
      auto* component = get_component(mouse_pos, _build_root);
      if (component != _selection) {
        _selection = component;
      }
    }
    if (!_selection) {
      update_highlight(_root_layout); // TODO update root layout with a layout for the built ui
    }
  }
}

void BuildState::draw(GameGraphics& g) {
  g.fill_rectangle(Rectangle(0,0,
                   get_window()->get_width(),
                   get_window()->get_height()),
                   Color(255, 255, 255));
}