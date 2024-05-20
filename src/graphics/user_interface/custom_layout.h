#pragma once

#include "game_graphics.h"
#include "enums.h"
#include "zsingal.h"

#include <functional>

namespace Zen {
class CustomLayout {
public:
  CustomLayout() = default;
  virtual ~CustomLayout();

  Signal on_size_changed;

  void enable() { _enabled = true; }
  void disable() { _enabled = false; }
  bool is_enabled() { return _enabled; }

  virtual void set_visible(bool is_visible) { _visible = is_visible;}
  bool is_visible() { return _visible; }

  void set_as_root() { _is_root = true; }
  bool is_root() { return (_parent == nullptr); }

  bool is_position_current() { return _position_current; }
  bool is_size_current() { return _size_current; }

  void set_parent(CustomLayout* parent);
  virtual void remove_child(CustomLayout* child);
  virtual void add_child(CustomLayout* child, int position = -1);

  void set_child_spacing(int spacing) { _child_spacing = spacing; }
  void set_padding(double padding_top, double padding_down, double padding_left, double padding_right) {
    set_padding(PaddingTo::STATIC, padding_top, PaddingTo::STATIC, padding_down, PaddingTo::STATIC, padding_left, PaddingTo::STATIC, padding_right);
  }
  void set_padding(PaddingTo padding_to_top, double padding_top,
                   PaddingTo padding_to_down, double padding_down,
                   PaddingTo padding_to_left, double padding_left,
                   PaddingTo padding_to_right, double padding_right);
  void set_padding_top(double padding_top, PaddingTo padding_to_top);
  void set_padding_down(double padding_down, PaddingTo padding_to_down);
  void set_padding_left(double padding_left, PaddingTo padding_to_left);
  void set_padding_right(double padding_right, PaddingTo padding_to_right);
  double get_padding_top();
  double get_padding_down();
  double get_padding_left();
  double get_padding_right();

  virtual void set_width(SizeTo size_to, int value);
  virtual void set_height(SizeTo size_to, int value);
  virtual void set_size(SizeTo size_to_width, SizeTo size_to_height, double width_value, double height_value);

  void center();

  void set_vertical();
  void set_horizontal();

  virtual void set_position(PositionTo position_to_x, int value_x, PositionTo position_to_y, int value_y);
  virtual void set_position_x(PositionTo position_to_x, int value_x);
  virtual void set_position_y(PositionTo position_to_y, int value_y);

  Vector2 get_size();
  double get_width() { if (_size_current) { return _width; } else { return get_size().get_x(); } }
  double get_height() { if (_size_current) { return _height; } else { return get_size().get_y(); } }
  Vector2 get_position();

  void child_size_changed() { _size_current = false; }

  virtual void set_background_color(Color background_color) { _has_background_color = true; _background_color = background_color; }
  Color get_background_color() { return _background_color; }
  virtual void set_on_click_callback(std::function<void()> callback) { _on_click_callback = std::move(callback); }

  virtual void click();

  virtual void draw(GameGraphics& game_graphics);

protected:
  void _draw_visible(GameGraphics& game_graphics); // What is this doing here, are we going to add a draw implementation class?

  void _request_child_position_update();

  CustomLayout* _parent = nullptr;
  std::vector<CustomLayout*> _children;

  Layout _layout = Layout::NONE;

  bool _is_root = false;
  bool _enabled = true;
  bool _visible = true;

  bool _position_current = false; // set to false when any position related to PositionTo values changes
  bool _size_current = false; // set to false when any size related to SizeTo values changes
  double _pos_x = 0;
  double _pos_y = 0;
  double _width = 0;
  double _height = 0;

  // Padding can either be a static value or a percentage of the layout, else throw error, check on set
  PaddingTo _padding_to_top = PaddingTo::STATIC;
  double _padding_value_top = 0.0;
  PaddingTo _padding_to_down = PaddingTo::STATIC;
  double _padding_value_down = 0.0;
  PaddingTo _padding_to_left = PaddingTo::STATIC;
  double _padding_value_left = 0.0;
  PaddingTo _padding_to_right = PaddingTo::STATIC;
  double _padding_value_right = 0.0;

  // Should this be broken up into a Horizontal and Vertical position?
  PositionTo _position_to_x = PositionTo::NONE;
  int _position_to_x_value = 0;
  PositionTo _position_to_y = PositionTo::NONE;
  int _position_to_y_value = 0;

  SizeTo _size_to_width = SizeTo::NONE;
  double _size_to_width_value = 0.0;
  SizeTo _size_to_height = SizeTo::NONE;
  double _size_to_height_value = 0.0;

  int _child_spacing = 0;

  bool _has_background_color = false;
  Color _background_color = Color(0, 0, 0, 0);

  std::function<void()> _on_click_callback;

  friend GameGraphics;
};
}
