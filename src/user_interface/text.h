#pragma once

#include "custom_layout.h"

#include <string>

namespace Zen {

class BaseText : public CustomLayout{
public:
  void draw(GameGraphics& game_graphics) override;
  void _update_text();
  std::string _font;
  std::string _text;
  int _max_width = 0;
  float _font_size = -1;
  bool _wrap = false;
  Color _font_color = Color();
};

class Text : public CustomLayout {
public:
  class Config {
  public:
    Color font_color;
    float font_size;
    std::string font_name;
  };

  Text();
  explicit Text(Config& config); // TODO use this at some point

  void set_wrap(bool should_wrap);

  void set_text(const std::string& text);
  void set_font(const std::string& font);
  void set_font_size(float font_size);
  void set_font_color(Color font_color);
  void _create_base_text();

  const std::string& get_text() const;
  const std::string& get_font() const;
  float get_font_size() const;
  Color get_font_color() const;
  bool get_wrap() const;
private:
  BaseText* _base_text = nullptr;
};

} // namespace Zen