#include "texture.h"

#include "texture_manager.h"

namespace Zen {

Texture::Texture(const std::string& texture_path) {
  _set_sdl_texture(TextureManager::_load_sdl_texture(texture_path));
}
Vector2 Texture::get_size() {
  return _texture_size;
}
void Texture::_update_texture_size() {
  float texture_height, texture_width;
  SDL_GetTextureSize(_texture, &texture_width, &texture_height);
  _texture_size.set_x(texture_width);
  _texture_size.set_y(texture_height);
}
void Texture::_set_sdl_texture(SDL_Texture* sdl_texture) {
  _texture = sdl_texture;
  _update_texture_size();
}
}
