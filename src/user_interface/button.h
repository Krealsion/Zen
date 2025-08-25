#pragma once

#include "custom_layout.h"

namespace Zen {
class Text;

class Button : public CustomLayout {
public:
  Button();

  void update()override ;

  void draw(GameGraphics& game_graphics) override;
  void set_text(const std::string& text);
  const std::string& get_text() const;

  void set_background_color(Color color) override;
  void set_hovered_bg_color(const Color& color);
  void set_auto_hover_color(bool auto_hover_color);

private:
  Text* _text;
  CustomLayout* _inner = nullptr;
  bool _mouse_hovered = false;
  bool _auto_hover_color = true;
  Color _default_background_color;
  Color _hovered_bg_color;
  Color _border_color;
};

} // namespace Zen