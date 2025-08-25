#pragma once

#include "color.h"
#include "enums.h"
#include "rectangle.h"
#include "vector2.h"
#include "zsignal.h"

#include <functional>
#include <string>

namespace Zen {
class BuildState; // Forward declaration for friend class
class GameGraphics; // Forward declaration for friend class
class CustomLayout {
public:
  CustomLayout();
  virtual ~CustomLayout();

  Signal on_size_changed;

  virtual void update();

  void set_name(const std::string& name) {_name = name;}
  std::string get_name() {return _name;}

  void enable() { _enabled = true; }
  void disable() { _enabled = false; }
  [[nodiscard]] bool is_enabled() const { return _enabled; }

  virtual void set_visible(bool is_visible) { _visible = is_visible;}
  [[nodiscard]] bool is_visible() const { return _visible; }

  void set_as_root() {
    _is_root = true;
  }
  bool is_root() { return _is_root; }

  virtual void draw(GameGraphics& game_graphics);

  void set_parent(CustomLayout* parent);
  [[nodiscard]] CustomLayout* get_parent() const { return _parent; }

  virtual void add_child(CustomLayout* child);
  virtual void add_child(CustomLayout* child, int position);
  [[nodiscard]] const std::vector<CustomLayout*>& get_children() const { return _children; }

  virtual void remove_child(CustomLayout* child);
  virtual void remove_all_children() { _children.clear(); }

  void set_child_spacing(int spacing) { _child_spacing = spacing; }

  void set_padding(double padding_top, double padding_bottom, double padding_left, double padding_right);
  void set_padding(PaddingTo padding_to_top, double padding_top,
                   PaddingTo padding_to_bottom, double padding_bottom,
                   PaddingTo padding_to_left, double padding_left,
                   PaddingTo padding_to_right, double padding_right);
  void set_padding_sides(double padding);
  void set_padding_sides(double padding_left, double padding_right);
  void set_padding_sides(double padding_left, PaddingTo padding_to_left, double padding_right, PaddingTo padding_to_right);
  void set_padding_vertical(double padding);
  void set_padding_vertical(double padding_top, double padding_bottom);
  void set_padding_vertical(double padding_top, PaddingTo padding_to_top, double padding_bottom, PaddingTo padding_to_bottom);
  void set_padding_top(double padding_top, PaddingTo padding_to_top = PaddingTo::STATIC);
  void set_padding_bottom(double padding_bottom, PaddingTo padding_to_bottom = PaddingTo::STATIC);
  void set_padding_left(double padding_left, PaddingTo padding_to_left = PaddingTo::STATIC);
  void set_padding_right(double padding_right, PaddingTo padding_to_right = PaddingTo::STATIC);
  double get_padding_top();
  double get_padding_bottom();
  double get_padding_left();
  double get_padding_right();

  void set_margin(double margin_top, double margin_bottom, double margin_left, double margin_right);
  void set_margin(PaddingTo margin_to_top, double margin_top,
                   PaddingTo margin_to_bottom, double margin_bottom,
                   PaddingTo margin_to_left, double margin_left,
                   PaddingTo margin_to_right, double margin_right);
  void set_margin_sides(double margin);
  void set_margin_sides(double margin_left, double margin_right);
  void set_margin_sides(double margin_left, PaddingTo margin_to_left, double margin_right, PaddingTo margin_to_right);
  void set_margin_vertical(double margin);
  void set_margin_vertical(double margin_top, double margin_bottom);
  void set_margin_vertical(double margin_top, PaddingTo margin_to_top, double margin_bottom, PaddingTo margin_to_bottom);
  void set_margin_top(double margin_top, PaddingTo margin_to_top = PaddingTo::STATIC);
  void set_margin_bottom(double margin_bottom, PaddingTo margin_to_bottom = PaddingTo::STATIC);
  void set_margin_left(double margin_left, PaddingTo margin_to_left = PaddingTo::STATIC);
  void set_margin_right(double margin_right, PaddingTo margin_to_right = PaddingTo::STATIC);
  double get_margin_top();
  double get_margin_bottom();
  double get_margin_left();
  double get_margin_right();

  virtual void set_width(SizeTo size_to, double value);
  virtual void set_height(SizeTo size_to, double value);
  virtual void set_size(SizeTo size_to_width, SizeTo size_to_height);
  virtual void set_size(SizeTo size_to_width, double width_value, SizeTo size_to_height, double height_value);
  virtual void set_size(SizeTo size_to_width, SizeTo size_to_height, double height_value);
  virtual void set_size(SizeTo size_to_width, double width_value, SizeTo size_to_height);
  Vector2 get_size();
  double get_width();
  double get_height();

  virtual void set_position(PositionTo position_to_x, PositionTo position_to_y);
  virtual void set_position(PositionTo position_to_x, int value_x, PositionTo position_to_y, int value_y);
  virtual void set_position_x(PositionTo position_to_x, int VALUE_X);
  virtual void set_position_y(PositionTo position_to_y, int value_y);
  virtual Vector2 get_position();
  double get_x();
  double get_y();

  Rectangle get_owned_destination();
  Rectangle get_background_destination();
  Rectangle get_inside_destination();

  double get_inside_width() {
    return get_background_destination().get_width() - get_padding_left() - get_padding_right();
  }
  double get_inside_height() {
    return get_background_destination().get_height() - get_padding_top() - get_padding_bottom();
  }

  void set_vertical();
  void set_horizontal();
  void center();

  virtual void set_background_color(Color background_color) { _has_background_color = true; _background_color = background_color; }
  Color get_background_color() { return _background_color; }

  virtual void set_on_click_callback(std::function<void()> callback) { _on_click_callback = std::move(callback); }
  bool has_callback() const { return _on_click_callback != nullptr; }
  // This is a method used to simulate a click on this object
  virtual void click();

  void reset();

protected:
  void _request_child_position_update();
  void _child_size_changed();
  void _on_position_changed();
  void _on_size_changed();

  std::string _name;
  static int _next_id;

  CustomLayout* _parent = nullptr;
  std::vector<CustomLayout*> _children;

  Layout _layout = Layout::CHILD_CONTROLLED;

  bool _is_root = false;
  bool _enabled = true;
  bool _visible = true;

  bool _x_current = false;
  bool _y_current = false;
  double _pos_x = 0;
  double _pos_y = 0;
  double _width = 0;
  double _height = 0;
  bool _width_current = false; // set to false when any width related to SizeTo values changes
  bool _height_current = false; // set to false when any height related to SizeTo

  // Padding can either be a static value or a percentage of the layout, else throw error, check on set
  PaddingTo _padding_to_top = PaddingTo::STATIC;
  double _padding_value_top = 0.0;
  PaddingTo _padding_to_bottom = PaddingTo::STATIC;
  double _padding_value_bottom = 0.0;
  PaddingTo _padding_to_left = PaddingTo::STATIC;
  double _padding_value_left = 0.0;
  PaddingTo _padding_to_right = PaddingTo::STATIC;
  double _padding_value_right = 0.0;

  // Padding can either be a static value or a percentage of the layout, else throw error, check on set
  PaddingTo _margin_to_top = PaddingTo::STATIC;
  double _margin_value_top = 0.0;
  PaddingTo _margin_to_bottom = PaddingTo::STATIC;
  double _margin_value_bottom = 0.0;
  PaddingTo _margin_to_left = PaddingTo::STATIC;
  double _margin_value_left = 0.0;
  PaddingTo _margin_to_right = PaddingTo::STATIC;
  double _margin_value_right = 0.0;

  // Should this be broken up into a Horizontal and Vertical position?
  PositionTo _position_to_x = PositionTo::RELATIVE;
  int _position_to_x_value = 0;
  PositionTo _position_to_y = PositionTo::RELATIVE;
  int _position_to_y_value = 0;

  SizeTo _size_to_width = SizeTo::STATIC;
  double _size_to_width_value = 0.0;
  SizeTo _size_to_height = SizeTo::STATIC;
  double _size_to_height_value = 0.0;

  // Maximum and minimum width and height, used for size constraints
  // TODO consider adding a SizeTo enum for these to allow for percentage based constraints
  double _max_width = 0.0;
  double _max_height = 0.0;
  double _min_width = 0.0;
  double _min_height = 0.0;

  // Spacing between children, used in Vertical and Horizontal layouts
  int _child_spacing = 0;

  bool _has_background_color = false;
  Color _background_color = Color(0, 0, 0, 0);

  std::function<void()> _on_click_callback;

  friend GameGraphics;
  friend BuildState;
};
}
