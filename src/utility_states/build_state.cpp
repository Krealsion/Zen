#include "build_state.h"
#include "input.h"
#include "logic/utils.h"  // For demangle
#include "logger.h"
#include <SDL3/SDL.h>
#include <fstream>
#include <sstream>  // For std::to_string

namespace Zen {

using json = nlohmann::json;

NLOHMANN_JSON_SERIALIZE_ENUM(Layout, {
{Layout::CHILD_CONTROLLED, "CHILD_CONTROLLED"},
{Layout::HORIZONTAL, "HORIZONTAL"},
{Layout::VERTICAL, "VERTICAL"}
});
NLOHMANN_JSON_SERIALIZE_ENUM(PositionTo, {
{PositionTo::RELATIVE, "RELATIVE"},
{PositionTo::LEFT, "LEFT"},
{PositionTo::RIGHT, "RIGHT"},
{PositionTo::CENTER, "CENTER"},
{PositionTo::TOP, "TOP"},
{PositionTo::BOTTOM, "BOTTOM"},
{PositionTo::PARENT_CONTROLLED, "PARENT_CONTROLLED"}
});
NLOHMANN_JSON_SERIALIZE_ENUM(DataType, {
{DataType::BIT, "BIT"},
{DataType::STRING, "STRING"},
{DataType::NUMBER, "NUMBER"},
{DataType::BOOLEAN, "BOOLEAN"}
});
NLOHMANN_JSON_SERIALIZE_ENUM(TextBoxFilterType, {
{TextBoxFilterType::ANY, "ANY"},
{TextBoxFilterType::DATA_TYPE, "DATA_TYPE"},
{TextBoxFilterType::INTEGER, "INTEGER"},
{TextBoxFilterType::EMAIL, "EMAIL"},
{TextBoxFilterType::PLUGIN, "PLUGIN"}
});
NLOHMANN_JSON_SERIALIZE_ENUM(SizeTo, {
{SizeTo::STATIC, "STATIC"},
{SizeTo::PARENT, "PARENT"},
{SizeTo::PARENT_PERCENT, "PARENT_PERCENT"},
{SizeTo::CHILDREN, "CHILDREN"},
{SizeTo::CHILDREN_PERCENT, "CHILDREN_PERCENT"},
{SizeTo::FILL, "FILL"}
});
NLOHMANN_JSON_SERIALIZE_ENUM(PaddingTo, {
{PaddingTo::STATIC, "STATIC"},
{PaddingTo::PERCENT, "PERCENT"}
});

// Non-intrusive for configs (as before)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Text::Config, font_name, font_size, font_color);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TextBox::TextBoxFilter, type, data_type);

BuildState::BuildState() : update_timer(16) {
  _standard_text.font_name = "Basic-Regular.ttf";
  _standard_text.font_size = 16.0f;
  _standard_text.font_color = Color(0, 0, 0);

//  _playground = new CustomLayout();
//  _playground->set_name("Playground");
//  _playground->set_background_color(Color(255, 255, 255));
//  _playground->set_size(SizeTo::STATIC, get_window()->get_width(),
//                        SizeTo::STATIC, get_window()->get_height());
//  _playground->set_position(PositionTo::RELATIVE, 0, PositionTo::RELATIVE, 0);



  // Panels setup
  _create_pane = std::make_unique<ScrollView>();
  _create_pane->set_name("create_pane_scroller");
  _create_pane->set_vertical();
  _create_pane->set_size(SizeTo::PARENT, SizeTo::PARENT_PERCENT, 0.5f);
  _create_pane->set_background_color(Color(210, 240, 240));
  _create_pane->add_child(create_create_pane());

  _tree_pane = std::make_unique<ScrollView>();
  _tree_pane->set_name("tree_pane");
  _tree_pane->set_vertical();
  _tree_pane->set_size(SizeTo::PARENT, SizeTo::PARENT_PERCENT, 0.5f);
  _tree_pane->set_background_color(Color(240, 210, 240));

  _details_pane = std::make_unique<ScrollView>();
  _details_pane->set_name("details_pane");
  _details_pane->set_vertical();
  _details_pane->set_size(SizeTo::PARENT, SizeTo::PARENT);
  _details_pane->set_background_color(Color(240, 240, 210));
  _details_pane->set_visible(false);

  auto left_pane = new CustomLayout();
  left_pane->set_name("left_pane");
  left_pane->set_vertical();
  left_pane->set_size(SizeTo::PARENT_PERCENT, 0.3f, SizeTo::PARENT);
  left_pane->add_child(_create_pane.get());
  left_pane->add_child(_tree_pane.get());
  left_pane->add_child(_details_pane.get());

  _build_root = std::make_unique<CustomLayout>();
  _build_root->set_as_root();
  _build_root->set_name("parent/root");
  _build_root->set_background_color(Color(210, 210, 210));
  _build_root->add_child(create_create_pane());

  auto test = new CustomLayout();
  test->set_name("test");
  test->set_size(SizeTo::PARENT_PERCENT, 0.5f, SizeTo::PARENT_PERCENT, 0.5f);
  test->set_background_color(Color(255, 0, 0));
//  _build_root->add_child(test);

  _root_layout->add_child(left_pane);
  _root_layout->set_name("root_layout");

  _selection = nullptr;
  refresh_tree();
}

BuildState::~BuildState() {
  delete _selection;
  delete _highlight;
}

void BuildState::draw(GameGraphics& g) {
  _build_root->draw(g);
  if (_highlight) {
    _highlight->draw(g);
  }
}

void BuildState::update_build_root() {
  _build_root->set_position_x(PositionTo::RELATIVE, _create_pane->_parent->get_width());
  _build_root->set_position_y(PositionTo::RELATIVE, 0.0f);
  _build_root->set_width(SizeTo::STATIC, _root_layout->get_width() - _build_root->get_position().get_x_int());
  _build_root->set_height(SizeTo::STATIC, _root_layout->get_height());
}

void BuildState::update() {
  Input::update_input();
  if (update_timer.is_time()) {
    if (Input::is_key_down(SDL_SCANCODE_ESCAPE)) {
      exit();
    }
    if (Input::is_mouse_button_down(MouseButton::RIGHT)) {
      enter_creation_mode();
    }
    if (Input::is_mouse_button_down(MouseButton::MIDDLE)) {
      auto log_element = [](CustomLayout* element) {
        Logger::log(LogLevel::INFO, "Element: " + element->get_name() +
                                    " at (" + std::to_string(element->get_position().get_x()) + ", " + std::to_string(element->get_position().get_y()) + ")" +
                                    " with size (" + std::to_string(element->get_width()) + ", " + std::to_string(element->get_height()) + ")");
      };
      log_element(_build_root.get());
      for (auto& child : _build_root->_children) {
        log_element(child);
      }
      _build_root->reset();
    }
    if (Input::is_mouse_button_down(MouseButton::LEFT)) {
      // TODO automate this in Zen Code
      auto mouse_pos = Input::get_mouse_position();
      auto* component_from_root = get_component(mouse_pos, _build_root.get(), true);
      if (component_from_root != _build_root.get()) {
        component_from_root->click();
      } else {
        auto* component = get_component(mouse_pos, _build_root.get());
        if (component != _selection) {
          enter_selection_mode(component);
        } else {
          // Clicked on the item signal?
        }
      }
    }
    update_highlight();
    update_build_root();
  }
}

void BuildState::enter_selection_mode(CustomLayout* selection) {
  _selection = selection;
  _highlight = nullptr;  // Clear any previous highlight
  _tree_pane->set_visible(false);
  _create_pane->set_visible(false);
  _details_pane->set_visible(true);
  refresh_details();
}

void BuildState::enter_creation_mode() {
  _selection = nullptr;
  _highlight = nullptr;  // Clear any previous highlight
  _tree_pane->set_visible(true);
  _create_pane->set_visible(true);
  _details_pane->set_visible(false);
  refresh_tree();
}

void BuildState::delete_selection() {
  if (_selection && _selection != _build_root.get()) {
    if (_selection->_parent) {
      _selection->_parent->remove_child(_selection);
      _selection = _selection->_parent;
      refresh_details();
      refresh_tree();
    }
  }
}

CustomLayout* BuildState::get_component(Vector2 mouse_pos, CustomLayout* layout, bool only_callback_registered) {
  for (auto& child : layout->_children) {
    if (child->is_visible() && child->get_owned_destination().contains(mouse_pos)) {
      auto* found = get_component(mouse_pos, child, only_callback_registered);
      if (!only_callback_registered || (found->has_callback())) {
        return found;
      }
    }
  }
  return layout;
}

void BuildState::update_highlight() {
  auto* default_root = _root_layout; // TODO default_root should be the _build_root
  auto* hovered = get_component(Input::get_mouse_position(), default_root);
  if (hovered == default_root) {
    if (_highlight) {
      _highlight->set_visible(false);
    }
    return;
  }
  if (hovered != _selection && hovered != default_root) {
    if (!_highlight) {
      _highlight = new CustomLayout();
      _highlight->set_name("highlight");
      _highlight->set_as_root();
      _highlight->set_background_color(Color(205, 0, 0, 58)); // TODO Highlight Color
    }
    _highlight->set_visible(true);
    _highlight->set_position(PositionTo::RELATIVE, hovered->get_position().get_x(), PositionTo::RELATIVE, hovered->get_position().get_y());
    _highlight->set_size(SizeTo::STATIC, hovered->get_width(), SizeTo::STATIC, hovered->get_height());
  } else if (_highlight) {
    _highlight->set_visible(false);
  }
}

void BuildState::refresh_details() {
  _details_pane->remove_all_children();
  _details_pane->add_child(create_details_pane(_selection));
}

void BuildState::refresh_tree() {
  _tree_pane->remove_all_children();
  _tree_pane->add_child(create_component_tree(_build_root.get()));
}

CustomLayout* BuildState::create_create_pane() {
  auto* layout = new CustomLayout();
  layout->set_name("create_pane");
  layout->set_size(SizeTo::PARENT, SizeTo::CHILDREN);
  layout->set_vertical();
  layout->set_child_spacing(8);
  layout->set_padding(8, 8, 8, 8);

  auto add_button = [&](const std::string& text, auto create_func) {
    auto* button = new Button();
    button->set_name("create_button_" + text);
    button->set_text(text);
    button->set_size(SizeTo::PARENT, SizeTo::CHILDREN);
    button->set_background_color(Color(200, 200, 200));
    button->set_hovered_bg_color(Color(70, 120, 200));
    button->set_on_click_callback(Action<>([this, create_func]() {
      if (_selection) {
        auto* new_component = create_func();
        _selection->add_child(new_component);
        _selection = new_component;
        refresh_details();
        refresh_tree();
      }
    }));
    layout->add_child(button);
  };

  add_button("Create Layout", []() { return new CustomLayout(); });
  add_button("Create Button", []() { return new Button(); });
  add_button("Create Text", []() { return new Text(); });
  add_button("Create TextBox", []() { return new TextBox(); });
  add_button("Create ScrollView", []() { return new ScrollView(); });

  auto* delete_button = new Button();
  delete_button->set_name("delete_button");
  delete_button->set_text("Delete Selected");
  delete_button->set_size(SizeTo::PARENT, SizeTo::CHILDREN);
  delete_button->set_background_color(Color(215, 100, 100));
  delete_button->set_hovered_bg_color(Color(220, 150, 150));
  delete_button->set_on_click_callback(Action<>([this]() {
    delete_selection();
  }));

  auto* save_button = new Button();
  save_button->set_name("save_button");
  save_button->set_text("Save JSON");
  save_button->set_background_color(Color(200, 200, 200));
  save_button->set_hovered_bg_color(Color(70, 120, 200));
  save_button->set_size(SizeTo::PARENT_PERCENT, 0.5f, SizeTo::CHILDREN);
  save_button->set_on_click_callback(Action<>([this]() { save_to_json("layout.json"); }));

  auto* load_button = new Button();
  load_button->set_name("load_button");
  load_button->set_text("Load JSON");
  load_button->set_background_color(Color(200, 200, 200));
  load_button->set_hovered_bg_color(Color(70, 120, 200));
  load_button->set_size(SizeTo::PARENT_PERCENT, 0.5f, SizeTo::CHILDREN);
  load_button->set_on_click_callback(Action<>([this]() { load_from_json("layout.json"); }));

  auto* save_load_container = new CustomLayout();
  save_load_container->set_name("save_load_container");
  save_load_container->set_horizontal();
  save_load_container->set_size(SizeTo::PARENT, SizeTo::CHILDREN);
  save_load_container->set_child_spacing(8);
  save_load_container->add_child(save_button);
  save_load_container->add_child(load_button);

  auto* button_container = new CustomLayout();
  button_container->set_name("button_container");
  button_container->set_child_spacing(8);
  button_container->set_vertical();
  button_container->set_size(SizeTo::PARENT, SizeTo::CHILDREN);
  button_container->add_child(delete_button);
  button_container->add_child(save_load_container);

//  auto fill = new CustomLayout();
//  fill->set_name("fill_space");
//  fill->set_size(SizeTo::PARENT, SizeTo::FILL); // TODO Make fill more functional in these settings
//  layout->add_child(fill);
  layout->add_child(button_container);

  return layout;
}

CustomLayout* BuildState::create_component_tree(CustomLayout* node) {
  auto* layout = new CustomLayout();
  layout->set_size(SizeTo::PARENT, SizeTo::CHILDREN);
  layout->set_name("component_tree_node_" + node->get_name());
  layout->set_vertical();
  layout->set_child_spacing(4);

  auto* node_text = new Text();
  node_text->set_name("node_text_" + node->get_name());
  node_text->set_text(node->get_name());
  node_text->set_size(SizeTo::CHILDREN, SizeTo::STATIC, 24);
  node_text->set_font(_standard_text.font_name);
  node_text->set_font_size(_standard_text.font_size);
  node_text->set_font_color(_standard_text.font_color);
  node_text->set_on_click_callback(Action<>([this, node]() {
    _selection = node;
    refresh_details();
  }));
  layout->add_child(node_text);

  auto* children_layout = new CustomLayout();
  children_layout->set_size(SizeTo::PARENT, SizeTo::CHILDREN);
  children_layout->set_name("children_layout_" + node->get_name());
  children_layout->set_vertical();
  children_layout->set_margin_left(16);
  for (auto* child : node->_children) {
    children_layout->add_child(create_component_tree(child));
  }
  layout->add_child(children_layout);

  return layout;
}

CustomLayout* BuildState::create_details_pane(CustomLayout* subject) {
  auto* layout = new CustomLayout();
  layout->set_name("details_pane");
  layout->set_vertical();
  layout->set_child_spacing(8);
  layout->set_padding(8, 8, 8, 8);
  layout->set_size(SizeTo::PARENT, SizeTo::CHILDREN);

  if (!subject) {
    auto* no_selection = new Text();
    no_selection->set_text("No component selected");
    layout->add_child(no_selection);
    return layout;
  }

  layout->add_child(create_basic_details(subject));

  if (auto* button = dynamic_cast<Button*>(subject)) {
    layout->add_child(create_button_details(button));
  } else if (auto* text = dynamic_cast<Text*>(subject)) {
    layout->add_child(create_text_details(text));
  } else if (auto* textbox = dynamic_cast<TextBox*>(subject)) {
    layout->add_child(create_textbox_details(textbox));
  } else if (auto* scrollview = dynamic_cast<ScrollView*>(subject)) {
    layout->add_child(create_scrollview_details(scrollview));
  }

  return layout;
}

CustomLayout* BuildState::create_details_row(const std::string& label, TextBox::TextBoxFilter filter, const std::string& value, Action<const std::string&>&& callback) {
  auto* layout = new CustomLayout();
  layout->set_name("DetailsRow: " + label);
  layout->set_horizontal();
  layout->set_size(SizeTo::PARENT, SizeTo::STATIC, _detail_row_height);
  layout->set_padding(4, 4, 4, 4);

  auto* label_text = new Text();
  label_text->set_name("label_text_" + label);
  label_text->set_text(label);
  label_text->set_size(SizeTo::PARENT_PERCENT, 0.4f, SizeTo::PARENT);
  label_text->set_position(PositionTo::LEFT, PositionTo::CENTER);
  layout->add_child(label_text);

  auto* value_text = new TextBox();
  value_text->set_name("value_text_input_" + label);
  value_text->set_text(value);
  value_text->set_filter(filter);
  value_text->set_size(SizeTo::PARENT_PERCENT, 0.6f, SizeTo::PARENT);
  value_text->set_position(PositionTo::RIGHT, PositionTo::CENTER);
  value_text->on_text_committed.connect(this, Action<>([value_text, callback](){
    callback(value_text->get_text());
  }));
  layout->add_child(value_text);

  return layout;
}

CustomLayout* BuildState::create_dropdown_row(const std::string& label, const std::vector<std::string>& options, int selected_index, Action<int>&& callback) {
  auto* layout = new CustomLayout();
  layout->set_name("DropdownRow: " + label);
  layout->set_horizontal();
  layout->set_size(SizeTo::PARENT, SizeTo::STATIC, _detail_row_height);
  layout->set_padding(4, 4, 4, 4);

  auto* label_text = new Text();
  label_text->set_name("label_text_" + label);
  label_text->set_text(label);
  label_text->set_size(SizeTo::PARENT_PERCENT, 0.4f, SizeTo::PARENT);
  label_text->set_position(PositionTo::LEFT, PositionTo::CENTER);
  layout->add_child(label_text);

  auto* dropdown = new DropDown();
  dropdown->set_name("dropdown_" + label);
  dropdown->set_options(options);
  dropdown->set_selected_index(selected_index);
  dropdown->set_size(SizeTo::PARENT_PERCENT, 0.6f, SizeTo::STATIC, _detail_row_height);
  dropdown->set_position(PositionTo::RIGHT, PositionTo::CENTER);
  dropdown->on_selection_changed.connect(this, Action<>([dropdown, callback](){
    callback(dropdown->get_selected_index());
  }));
  layout->add_child(dropdown);

  return layout;
}

CustomLayout* BuildState::create_basic_details(CustomLayout* subject) {
  auto* layout = new CustomLayout();
  layout->set_name("basic_details_rows");
  layout->set_vertical();
  layout->set_child_spacing(4);
  layout->set_size(SizeTo::PARENT, SizeTo::CHILDREN);

  layout->add_child(create_details_row("Name", TextBox::TextBoxFilter(TextBoxFilterType::DATA_TYPE, DataType::STRING), subject->get_name(),
                                       Action<const std::string&>([this, subject](const std::string& value) {
                                         subject->set_name(value);
                                         refresh_tree();
                                       })));

  layout->add_child(create_dropdown_row("Width Type", {"STATIC", "PARENT", "PARENT_PERCENT", "CHILDREN", "CHILDREN_PERCENT", "FILL"},
                                        static_cast<int>(subject->_size_to_width), Action<int>([this, subject](int idx) {
        subject->set_width(static_cast<SizeTo>(idx), subject->_size_to_width_value);
        refresh_details();
        refresh_tree();
      })));

  layout->add_child(create_details_row("Width Value", TextBox::TextBoxFilter(TextBoxFilterType::DATA_TYPE, DataType::NUMBER),
                                       std::to_string(subject->_size_to_width_value), Action<const std::string&>([this, subject](const std::string& value) {
        if (!value.empty()) {
          subject->set_width(subject->_size_to_width, std::stof(value));
          refresh_details();
          refresh_tree();
        }
      })));

  layout->add_child(create_dropdown_row("Height Type", {"STATIC", "PARENT", "PARENT_PERCENT", "CHILDREN", "CHILDREN_PERCENT", "FILL"},
                                        static_cast<int>(subject->_size_to_height), Action<int>([this, subject](int idx) {
        subject->set_height(static_cast<SizeTo>(idx), subject->_size_to_height_value);
        refresh_details();
        refresh_tree();
      })));

  layout->add_child(create_details_row("Height Value", TextBox::TextBoxFilter(TextBoxFilterType::DATA_TYPE, DataType::NUMBER),
                                       std::to_string(subject->_size_to_height_value), Action<const std::string&>([this, subject](const std::string& value) {
        if (!value.empty()) {
          subject->set_height(subject->_size_to_height, std::stof(value));
          refresh_details();
          refresh_tree();
        }
      })));

  layout->add_child(create_dropdown_row("Position X", {"RELATIVE", "LEFT", "RIGHT", "CENTER", "PARENT_CONTROLLED"},
                                        static_cast<int>(subject->_position_to_x), Action<int>([this, subject](int idx) {
        subject->set_position_x(static_cast<PositionTo>(idx), subject->_position_to_x_value);
        refresh_details();
        refresh_tree();
      })));

  layout->add_child(create_details_row("Position X Value", TextBox::TextBoxFilter(TextBoxFilterType::INTEGER),
                                       std::to_string(subject->_position_to_x_value), Action<const std::string&>([this, subject](const std::string& value) {
        if (!value.empty()) {
          subject->set_position_x(subject->_position_to_x, std::stoi(value));
          refresh_details();
          refresh_tree();
        }
      })));

  layout->add_child(create_dropdown_row("Position Y", {"RELATIVE", "TOP", "BOTTOM", "CENTER", "PARENT_CONTROLLED"},
                                        static_cast<int>(subject->_position_to_y), Action<int>([this, subject](int idx) {
        subject->set_position_y(static_cast<PositionTo>(idx), subject->_position_to_y_value);
        refresh_details();
        refresh_tree();
      })));

  layout->add_child(create_details_row("Position Y Value", TextBox::TextBoxFilter(TextBoxFilterType::INTEGER),
                                       std::to_string(subject->_position_to_y_value), Action<const std::string&>([this, subject](const std::string& value) {
        if (!value.empty()) {
          subject->set_position_y(subject->_position_to_y, std::stoi(value));
          refresh_details();
          refresh_tree();
        }
      })));

  layout->add_child(create_dropdown_row("Layout", {"CHILD_CONTROLLED", "HORIZONTAL", "VERTICAL"},
                                        static_cast<int>(subject->_layout), Action<int>([this, subject](int idx) {
        subject->_layout = static_cast<Layout>(idx);
        subject->_request_child_position_update();
        refresh_details();
        refresh_tree();
      })));

  layout->add_child(create_details_row("Child Spacing", TextBox::TextBoxFilter(TextBoxFilterType::INTEGER),
                                       std::to_string(subject->_child_spacing), Action<const std::string&>([this, subject](const std::string& value) {
        if (!value.empty()) {
          subject->set_child_spacing(std::stoi(value));
          refresh_details();
          refresh_tree();
        }
      })));

  layout->add_child(create_dropdown_row("Padding Top Type", {"STATIC", "PERCENT"},
                                        static_cast<int>(subject->_padding_to_top), Action<int>([this, subject](int idx) {
        subject->set_padding_top(subject->_padding_value_top, static_cast<PaddingTo>(idx));
        refresh_details();
      })));

  layout->add_child(create_details_row("Padding Top Value", TextBox::TextBoxFilter(TextBoxFilterType::DATA_TYPE, DataType::NUMBER),
                                       std::to_string(subject->_padding_value_top), Action<const std::string&>([this, subject](const std::string& value) {
        if (!value.empty()) {
          subject->set_padding_top(std::stof(value), subject->_padding_to_top);
          refresh_details();
        }
      })));
  layout->add_child(create_dropdown_row("Padding Bottom Type", {"STATIC", "PERCENT"},
                                        static_cast<int>(subject->_padding_to_bottom), Action<int>([this, subject](int idx) {
        subject->set_padding_bottom(subject->_padding_value_bottom, static_cast<PaddingTo>(idx));
        refresh_details();
      })));

  layout->add_child(create_details_row("Padding Bottom Value", TextBox::TextBoxFilter(TextBoxFilterType::DATA_TYPE, DataType::NUMBER),
                                       std::to_string(subject->_padding_value_bottom), Action<const std::string&>([this, subject](const std::string& value) {
        if (!value.empty()) {
          subject->set_padding_bottom(std::stof(value), subject->_padding_to_bottom);
          refresh_details();
        }
      })));
  layout->add_child(create_dropdown_row("Padding Right Type", {"STATIC", "PERCENT"},
                                        static_cast<int>(subject->_padding_to_right), Action<int>([this, subject](int idx) {
        subject->set_padding_right(subject->_padding_value_right, static_cast<PaddingTo>(idx));
        refresh_details();
      })));

  layout->add_child(create_details_row("Padding Right Value", TextBox::TextBoxFilter(TextBoxFilterType::DATA_TYPE, DataType::NUMBER),
                                       std::to_string(subject->_padding_value_right), Action<const std::string&>([this, subject](const std::string& value) {
        if (!value.empty()) {
          subject->set_padding_right(std::stof(value), subject->_padding_to_right);
          refresh_details();
        }
      })));
  layout->add_child(create_dropdown_row("Padding Left Type", {"STATIC", "PERCENT"},
                                        static_cast<int>(subject->_padding_to_left), Action<int>([this, subject](int idx) {
        subject->set_padding_left(subject->_padding_value_left, static_cast<PaddingTo>(idx));
        refresh_details();
      })));

  layout->add_child(create_details_row("Padding Left Value", TextBox::TextBoxFilter(TextBoxFilterType::DATA_TYPE, DataType::NUMBER),
                                       std::to_string(subject->_padding_value_left), Action<const std::string&>([this, subject](const std::string& value) {
        if (!value.empty()) {
          subject->set_padding_left(std::stof(value), subject->_padding_to_left);
          refresh_details();
        }
      })));
  // Margin (similar to padding; add rows for top/bottom/left/right)
  layout->add_child(create_dropdown_row("Margin Top Type", {"STATIC", "PERCENT"},
                                        static_cast<int>(subject->_margin_to_top), Action<int>([this, subject](int idx) {
        subject->set_margin_top(subject->_margin_value_top, static_cast<PaddingTo>(idx));
        refresh_details();
      })));

  layout->add_child(create_details_row("Margin Top Value", TextBox::TextBoxFilter(TextBoxFilterType::DATA_TYPE, DataType::NUMBER),
                                       std::to_string(subject->_margin_value_top), Action<const std::string&>([this, subject](const std::string& value) {
        if (!value.empty()) {
          subject->set_margin_top(std::stof(value), subject->_margin_to_top);
          refresh_details();
        }
      })));
  layout->add_child(create_dropdown_row("Margin Bottom Type", {"STATIC", "PERCENT"},
                                        static_cast<int>(subject->_margin_to_bottom), Action<int>([this, subject](int idx) {
        subject->set_margin_bottom(subject->_margin_value_bottom, static_cast<PaddingTo>(idx));
        refresh_details();
      })));

  layout->add_child(create_details_row("Margin Bottom Value", TextBox::TextBoxFilter(TextBoxFilterType::DATA_TYPE, DataType::NUMBER),
                                       std::to_string(subject->_margin_value_bottom), Action<const std::string&>([this, subject](const std::string& value) {
        if (!value.empty()) {
          subject->set_margin_bottom(std::stof(value), subject->_margin_to_bottom);
          refresh_details();
        }
      })));
  layout->add_child(create_dropdown_row("Margin Right Type", {"STATIC", "PERCENT"},
                                        static_cast<int>(subject->_margin_to_right), Action<int>([this, subject](int idx) {
        subject->set_margin_right(subject->_margin_value_right, static_cast<PaddingTo>(idx));
        refresh_details();
      })));

  layout->add_child(create_details_row("Margin Right Value", TextBox::TextBoxFilter(TextBoxFilterType::DATA_TYPE, DataType::NUMBER),
                                       std::to_string(subject->_margin_value_right), Action<const std::string&>([this, subject](const std::string& value) {
        if (!value.empty()) {
          subject->set_margin_right(std::stof(value), subject->_margin_to_right);
          refresh_details();
        }
      })));
  layout->add_child(create_dropdown_row("Margin Left Type", {"STATIC", "PERCENT"},
                                        static_cast<int>(subject->_margin_to_left), Action<int>([this, subject](int idx) {
        subject->set_margin_left(subject->_margin_value_left, static_cast<PaddingTo>(idx));
        refresh_details();
      })));

  layout->add_child(create_details_row("Margin Left Value", TextBox::TextBoxFilter(TextBoxFilterType::DATA_TYPE, DataType::NUMBER),
                                       std::to_string(subject->_margin_value_left), Action<const std::string&>([this, subject](const std::string& value) {
        if (!value.empty()) {
          subject->set_margin_left(std::stof(value), subject->_margin_to_left);
          refresh_details();
        }
      })));

  // Background Color (as RGBA)
  layout->add_child(create_details_row("BG Red", TextBox::TextBoxFilter(TextBoxFilterType::INTEGER),
                                       std::to_string(subject->get_background_color().get_red()), Action<const std::string&>([this, subject](const std::string& value) {
        if (!value.empty()) {
          Color color = subject->get_background_color();
          color.set_red(std::stoi(value));
          subject->set_background_color(color);
          refresh_details();
        }
      })));

  layout->add_child(create_details_row("BG Green", TextBox::TextBoxFilter(TextBoxFilterType::INTEGER),
                                       std::to_string(subject->get_background_color().get_green()), Action<const std::string&>([this, subject](const std::string& value) {
        if (!value.empty()) {
          Color color = subject->get_background_color();
          color.set_green(std::stoi(value));
          subject->set_background_color(color);
          refresh_details();
        }
      })));

  layout->add_child(create_details_row("BG Blue", TextBox::TextBoxFilter(TextBoxFilterType::INTEGER),
                                       std::to_string(subject->get_background_color().get_blue()), Action<const std::string&>([this, subject](const std::string& value) {
        if (!value.empty()) {
          Color color = subject->get_background_color();
          color.set_blue(std::stoi(value));
          subject->set_background_color(color);
          refresh_details();
        }
      })));

  layout->add_child(create_details_row("BG Alpha", TextBox::TextBoxFilter(TextBoxFilterType::INTEGER),
                                       std::to_string(subject->get_background_color().get_alpha()), Action<const std::string&>([this, subject](const std::string& value) {
        if (!value.empty()) {
          Color color = subject->get_background_color();
          color.set_alpha(std::stoi(value));
          subject->set_background_color(color);
          refresh_details();
        }
      })));

  return layout;
}

CustomLayout* BuildState::create_button_details(Button* button) {
  auto* layout = new CustomLayout();
  layout->set_name("Button Details");
  layout->set_vertical();
  layout->set_child_spacing(4);

  layout->add_child(create_details_row("Text", TextBox::TextBoxFilter(TextBoxFilterType::DATA_TYPE, DataType::STRING), button->get_text(),
                                       Action<const std::string&>([this, button](const std::string& value) {
                                         button->set_text(value);
                                         refresh_details();
                                         refresh_tree();
                                       })));

  return layout;
}

CustomLayout* BuildState::create_text_details(Text* text) {
  auto* layout = new CustomLayout();
  layout->set_name("Text Details");
  layout->set_vertical();
  layout->set_child_spacing(4);

  layout->add_child(create_details_row("Text", TextBox::TextBoxFilter(TextBoxFilterType::DATA_TYPE, DataType::STRING), text->get_text(),
                                       Action<const std::string&>([this, text](const std::string& value) {
                                         text->set_text(value);
                                         refresh_details();
                                         refresh_tree();
                                       })));

  layout->add_child(create_details_row("Font", TextBox::TextBoxFilter(TextBoxFilterType::DATA_TYPE, DataType::STRING), text->get_font(),
                                       Action<const std::string&>([this, text](const std::string& value) {
                                         text->set_font(value);
                                         refresh_details();
                                       })));

  layout->add_child(create_details_row("Font Size", TextBox::TextBoxFilter(TextBoxFilterType::DATA_TYPE, DataType::NUMBER), std::to_string(text->get_font_size()),
                                       Action<const std::string&>([this, text](const std::string& value) {
                                         if (!value.empty()) {
                                           text->set_font_size(std::stof(value));
                                           refresh_details();
                                         }
                                       })));

  layout->add_child(create_details_row("Wrap (0/1)", TextBox::TextBoxFilter(TextBoxFilterType::INTEGER), std::to_string(text->get_wrap() ? 1 : 0),
                                       Action<const std::string&>([this, text](const std::string& value) {
                                         if (!value.empty()) {
                                           text->set_wrap(std::stoi(value) != 0);
                                           refresh_details();
                                         }
                                       })));

  // Font Color (similar to BG color rows above)
  layout->add_child(create_details_row("Font Red", TextBox::TextBoxFilter(TextBoxFilterType::INTEGER),
                                       std::to_string(text->get_font_color().get_red()), Action<const std::string&>([this, text](const std::string& value) {
        if (!value.empty()) {
          Color color = text->get_font_color();
          color.set_red(std::stoi(value));
          text->set_font_color(color);
          refresh_details();
        }
      })));
  layout->add_child(create_details_row("Font Green", TextBox::TextBoxFilter(TextBoxFilterType::INTEGER),
                                       std::to_string(text->get_font_color().get_green()), Action<const std::string&>([this, text](const std::string& value) {
        if (!value.empty()) {
          Color color = text->get_font_color();
          color.set_green(std::stoi(value));
          text->set_font_color(color);
          refresh_details();
        }
      })));
  layout->add_child(create_details_row("Font Blue", TextBox::TextBoxFilter(TextBoxFilterType::INTEGER),
                                       std::to_string(text->get_font_color().get_blue()), Action<const std::string&>([this, text](const std::string& value) {
        if (!value.empty()) {
          Color color = text->get_font_color();
          color.set_blue(std::stoi(value));
          text->set_font_color(color);
          refresh_details();
        }
      })));


  layout->add_child(create_details_row("BG Alpha", TextBox::TextBoxFilter(TextBoxFilterType::INTEGER),
                                       std::to_string(text->get_background_color().get_alpha()), Action<const std::string&>([this, text](const std::string& value) {
        if (!value.empty()) {
          Color color = text->get_background_color();
          color.set_alpha(std::stoi(value));
          text->set_background_color(color);
          refresh_details();
        }
      })));

  return layout;
}

CustomLayout* BuildState::create_textbox_details(TextBox* textbox) {
  auto* layout = new CustomLayout();
  layout->set_name("TextBox Details");
  layout->set_vertical();
  layout->set_child_spacing(4);

  layout->add_child(create_details_row("Text", TextBox::TextBoxFilter(TextBoxFilterType::DATA_TYPE, DataType::STRING), textbox->get_text(),
                                       Action<const std::string&>([this, textbox](const std::string& value) {
                                         textbox->set_text(value);
                                         refresh_details();
                                         refresh_tree();
                                       })));

  layout->add_child(create_dropdown_row("Filter Type", {"ANY", "DATA_TYPE", "INTEGER", "EMAIL", "PLUGIN"},
                                        static_cast<int>(textbox->get_filter().type), Action<int>([this, textbox](int idx) {
        textbox->set_filter(static_cast<TextBoxFilterType>(idx), textbox->get_filter().data_type);
        refresh_details();
      })));

  layout->add_child(create_dropdown_row("Data Type (if DATA_TYPE)", {"BIT", "STRING", "NUMBER", "BOOLEAN"},
                                        static_cast<int>(textbox->get_filter().data_type), Action<int>([this, textbox](int idx) {
        textbox->set_filter(textbox->get_filter().type, static_cast<DataType>(idx));
        refresh_details();
      })));

  return layout;
}

CustomLayout* BuildState::create_scrollview_details(ScrollView* scrollview) {
  auto* layout = new CustomLayout();
  layout->set_name("ScrollView Details");
  layout->set_vertical();
  layout->set_child_spacing(4);

  // TODO Add scroll-specific props if any (e.g., scroll direction if you add it)

  return layout;
}

void BuildState::save_to_json(const std::string& file_path) {
  json j = serialize_component(_build_root.get());
  std::ofstream file(file_path);
  if (file.is_open()) {
    file << j.dump(4);
    SDL_Log("Saved to %s", file_path.c_str());
  } else {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to save %s", file_path.c_str());
  }
}

void BuildState::load_from_json(const std::string& file_path) {
  std::ifstream file(file_path);
  if (!file.is_open()) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to open %s", file_path.c_str());
    return;
  }
  json j;
  file >> j;
  _build_root = deserialize_component(j);
  _selection = _build_root.get();
  refresh_details();
  refresh_tree();
  SDL_Log("Loaded from %s", file_path.c_str());
}

nlohmann::json BuildState::serialize_component(CustomLayout* component) {
  json j;
  j["type"] = Utility::demangle(typeid(*component).name());
  j["name"] = component->get_name();
  j["size_to_width"] = component->_size_to_width;
  j["size_to_width_value"] = component->_size_to_width_value;
  j["size_to_height"] = component->_size_to_height;
  j["size_to_height_value"] = component->_size_to_height_value;
  j["position_to_x"] = component->_position_to_x;
  j["position_to_x_value"] = component->_position_to_x_value;
  j["position_to_y"] = component->_position_to_y;
  j["position_to_y_value"] = component->_position_to_y_value;
  j["layout"] = component->_layout;
  j["child_spacing"] = component->_child_spacing;
  j["padding_to_top"] = component->_padding_to_top;
  j["padding_value_top"] = component->_padding_value_top;
  j["padding_to_bottom"] = component->_padding_to_bottom;
  j["padding_value_bottom"] = component->_padding_value_bottom;
  j["padding_to_left"] = component->_padding_to_left;
  j["padding_value_left"] = component->_padding_value_left;
  j["padding_to_right"] = component->_padding_to_right;
  j["padding_value_right"] = component->_padding_value_right;
  j["margin_to_top"] = component->_margin_to_top;
  j["margin_value_top"] = component->_margin_value_top;
  j["margin_to_bottom"] = component->_margin_to_bottom;
  j["margin_value_bottom"] = component->_margin_value_bottom;
  j["margin_to_left"] = component->_margin_to_left;
  j["margin_value_left"] = component->_margin_value_left;
  j["margin_to_right"] = component->_margin_to_right;
  j["margin_value_right"] = component->_margin_value_right;
  j["has_background_color"] = component->_has_background_color;
  j["background_color"] = {component->get_background_color().get_red(), component->get_background_color().get_green(),
                           component->get_background_color().get_blue(), component->get_background_color().get_alpha()};

  if (auto* text = dynamic_cast<Text*>(component)) {
    j["text"] = text->get_text();
    j["font"] = text->get_font();
    j["font_size"] = text->get_font_size();
    j["font_color"] = {text->get_font_color().get_red(), text->get_font_color().get_green(),
                       text->get_font_color().get_blue(), text->get_font_color().get_alpha()};
    j["wrap"] = text->get_wrap();
  } else if (auto* textbox = dynamic_cast<TextBox*>(component)) {
    j["text"] = textbox->get_text();
    j["filter"] = textbox->get_filter();
  } else if (auto* button = dynamic_cast<Button*>(component)) {
    j["text"] = button->get_text();
  }

  j["children"] = json::array();
  for (auto* child : component->_children) {
    j["children"].push_back(serialize_component(child));
  }

  return j;
}

std::unique_ptr<CustomLayout> BuildState::deserialize_component(const json& j) {
  std::string type = j["type"];
  std::unique_ptr<CustomLayout> component;

  if (type.find("Button") != std::string::npos) {
    component = std::make_unique<Button>();
    dynamic_cast<Button*>(component.get())->set_text(j["text"]);
  } else if (type.find("Text") != std::string::npos) {
    component = std::make_unique<Text>();
    auto* text = dynamic_cast<Text*>(component.get());
    text->set_text(j["text"]);
    text->set_font(j["font"]);
    text->set_font_size(j["font_size"]);
    auto fc = j["font_color"];
    text->set_font_color(Color(fc[0], fc[1], fc[2], fc[3]));
    text->set_wrap(j["wrap"]);
  } else if (type.find("TextBox") != std::string::npos) {
    component = std::make_unique<TextBox>();
    auto* textbox = dynamic_cast<TextBox*>(component.get());
    textbox->set_text(j["text"]);
    textbox->set_filter(j["filter"]);
  } else if (type.find("ScrollView") != std::string::npos) {
    component = std::make_unique<ScrollView>();
  } else {
    component = std::make_unique<CustomLayout>();
  }

  component->set_name(j["name"]);
  component->set_size(j["size_to_width"], j["size_to_width_value"], j["size_to_height"], j["size_to_height_value"]);
  component->set_position(j["position_to_x"], j["position_to_x_value"], j["position_to_y"], j["position_to_y_value"]);
  component->_layout = j["layout"];
  component->set_child_spacing(j["child_spacing"]);
  component->set_padding(j["padding_to_top"], j["padding_value_top"], j["padding_to_bottom"], j["padding_value_bottom"],
                         j["padding_to_left"], j["padding_value_left"], j["padding_to_right"], j["padding_value_right"]);
  component->set_margin(j["margin_to_top"], j["margin_value_top"], j["margin_to_bottom"], j["margin_value_bottom"],
                        j["margin_to_left"], j["margin_value_left"], j["margin_to_right"], j["margin_value_right"]);
  if (j["has_background_color"]) {
    auto bc = j["background_color"];
    component->set_background_color(Color(bc[0], bc[1], bc[2], bc[3]));
  }

  for (const auto& child_json : j["children"]) {
    auto child = deserialize_component(child_json);
    component->add_child(child.release());
  }

  return component;
}

}  // namespace Zen