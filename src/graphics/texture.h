#pragma once

#include "types/color.h"
#include "types/rectangle.h"

#include <SDL3/SDL_render.h>

namespace Zen {

/**
 * This is a wrapper class for SDL_Texture
 */
class Texture {
public:
  Texture() = default;
  explicit Texture(const std::string& texture_path);

  Vector2 get_size();

private:
  explicit Texture(SDL_Texture* texture) {
    _set_sdl_texture(texture);
  }

  void _update_texture_size();
  void _set_sdl_texture(SDL_Texture* sdl_texture);
  SDL_Texture* _get_sdl_texture() {return _texture;}

  SDL_Texture* _texture = nullptr;
  Vector2 _texture_size;
  Color _color_filter;

  friend class TextureManager;
  friend class GameGraphics;
};
}
