#include "texture_manager.h"

#include <SDL_image.h>

namespace Zen {

std::shared_ptr<Texture> TextureManager::get_texture(const std::string& texture_path) {
  if (!_renderer)
    return std::make_shared<Texture>(Texture());

  if (_texture_map.count(texture_path) != 0) {
    return _texture_map.find(texture_path)->second;
  }

  _texture_map.emplace(texture_path, std::make_shared<Texture>(Texture(_get_sdl_texture(texture_path))));
  return _texture_map.end()->second;
}

SDL_Texture* TextureManager::_get_sdl_texture(const std::string& texture_path){
  return IMG_LoadTexture(_renderer, texture_path.c_str());
}
}
