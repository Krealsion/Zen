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
  static std::shared_ptr<Texture> get_text_texture(const std::string& text, const std::string& font_path, float text_size, Color text_color);

  static Vector2 get_text_size(const std::string& text, const std::string& font_path, float text_size);
  static Vector2 get_text_size(const std::string& text, const std::string& font_path, float text_size, int max_width);

private:
  // Initialization from when the renderer is created, allowing immediately for textures to be loaded
  static void _set_renderer(SDL_Renderer* renderer) { _renderer = renderer; }
  static SDL_Texture* _load_sdl_texture(const std::string& texture_path);

  static std::map<std::string, std::shared_ptr<Texture>> _texture_map;
  static SDL_Renderer* _renderer;

  friend class Renderer;
  friend class Texture;
};
}
