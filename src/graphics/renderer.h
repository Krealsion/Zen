#pragma once

#include "types/rectangle.h"
#include "window.h"
#include "game_graphics.h"

#include <vector>
#include <string>
#include <SDL.h>
#include <map>

namespace Zen {
/**
 * The renderer is in charge of creating and displaying the window
 * as well as any rendering of objects to the window
 * TODO split texture loading and creation into a separate class
 */
class Renderer {
public:
  Renderer(const std::string& name, Rectangle window_rectangle);

  ~Renderer();

  void render_obj(SDL_Texture* texture, SDL_Rect* destination, SDL_Rect* clipping, double angle, SDL_Point* origin);

  void render_obj(SDL_Texture* texture, SDL_Rect* destination);

  SDL_Texture* load_texture(const std::string& path);

  SDL_Texture* get_texture(const std::string& path);

  SDL_Texture* load_text(const std::string& text, std::string font_name, int size, SDL_Color color);

  void render_game_graphics(GameGraphics& game_graphics);

protected:
  std::map<std::string, SDL_Texture*> _texture_map;

  Window game_window;
  SDL_Renderer* renderer;
};
}
