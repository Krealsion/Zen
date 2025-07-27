#pragma once

#include "custom_layout.h"

namespace Zen {
class Text;

class Button : public CustomLayout {
public:
  Button();

  void update();

  void draw(GameGraphics& game_graphics) override;
  void set_text(const std::string& text);

  void set_hovered_bg_color(const Color& color);
  void set_auto_hover_color(bool auto_hover_color);

private:
  Text* _text;
  bool _mouse_hovered = false;
  bool _auto_hover_color = true;
  Color _hovered_bg_color;
};

} // namespace Zen