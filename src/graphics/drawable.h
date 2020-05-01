#pragma once

#include "game_graphics.h"

namespace Zen {
class Drawable {
public:
  virtual void set_visible(bool is_visible) { _visible = is_visible;}
  virtual void draw(GameGraphics& game_graphics) = 0;

protected:
  void draw_visible(GameGraphics& game_graphics) {
    if (_visible){
      draw(game_graphics);
    }
  }

  bool _visible = true;

  friend GameGraphics;
};
}
