#pragma once

#include "texture.h"

#include <map>
#include <memory>

namespace Zen {

class TextureManager {
public:
  static void init_ttf();
  static void unload_textures();

  static std::shared_ptr<Texture> get_texture(const std::string& texture_path);
  static std::shared_ptr<Texture> get_font_texture(const std::string& text, const std::string& font_path, int text_height, Zen::Color text_color);

private:
  // Initialization from when the renderer is created, allowing immediately for textures to be loaded
  static void _set_renderer(SDL_Renderer* renderer) { _renderer = renderer; }
  static SDL_Texture* _get_sdl_texture(const std::string& texture_path);

  static std::map<std::string, std::shared_ptr<Texture>> _texture_map;
  static SDL_Renderer* _renderer;

  friend class Renderer;
  friend class Texture;
};
}
