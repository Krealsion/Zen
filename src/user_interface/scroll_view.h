#pragma once

#include "custom_layout.h"

namespace Zen {

class ScrollView : public CustomLayout {
public:
  ScrollView() = default;
  ~ScrollView() override = default;

  void update() override;
  void draw(GameGraphics& game_graphics) override;

  void scroll_to_percent(double percent);

private:
  void _manage_input();

  Vector2 _get_children_total_size();
  double _scroll_offset = 0.0;
};
}

