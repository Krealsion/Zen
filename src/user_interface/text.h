#pragma once

#include "custom_layout.h"

#include <string>

namespace Zen {

class Text : public CustomLayout {
public:
  class Config {
  public:
    Color font_color;
    float font_size;
    std::string font_name;
  };

  Text();
  explicit Text(Config& config);


  void draw(GameGraphics& game_graphics) override;
  // Set Width will auto-set wrapping if the value is no
  void set_width(SizeTo size_to, double value) override;
  void set_height(SizeTo size_to, double value) override;

  void set_wrap(bool should_wrap);
  void set_text_wrap_width(int value);

  void set_text(const std::string& text);
  void set_font(const std::string& font);
  void set_font_size(float font_size);
  void set_font_color(Color font_color);

private:
  PositionTo _position_to = PositionTo::CENTER;
  std::string _font;
  std::string _text;
  int _max_width = 0;
  float _font_size = -1;
  bool _wrap = false;
  Color _font_color = Color();

  void _update_text();
};

} // namespace Zen