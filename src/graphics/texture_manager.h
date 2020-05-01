#pragma once

#include "texture.h"

#include <map>

namespace Zen {

class TextureManager {
public:
  static std::shared_ptr<Texture> get_texture(const std::string& texture_path);

private:
  // Initialization from when the renderer is created, allowing immediately for textures to be loaded
  static void _set_renderer(SDL_Renderer* renderer) { _renderer = renderer;}
  static SDL_Texture* _get_sdl_texture(const std::string& texture_path);

  static std::map<std::string, std::shared_ptr<Texture>> _texture_map;
  static SDL_Renderer* _renderer;

  friend class Renderer;
  friend class Texture;
};
}
