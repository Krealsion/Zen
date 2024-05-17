#include "texture_manager.h"

#include <SDL_image.h>
#include <SDL_ttf.h>

namespace Zen {
std::map<std::string, std::shared_ptr<Texture>> TextureManager::_texture_map;
SDL_Renderer* TextureManager::_renderer = nullptr;

void TextureManager::init_ttf() {
}

void TextureManager::unload_textures() {
  for (auto [key, value] : _texture_map) {
    SDL_DestroyTexture(value->_get_sdl_texture());
  }
  _texture_map.clear();
}

std::shared_ptr<Texture> TextureManager::get_texture(const std::string& texture_path) {
  if (!_renderer)
    return std::make_shared<Texture>(Texture());

  if (_texture_map.count(texture_path) != 0) {
    return _texture_map.find(texture_path)->second;
  }

  _texture_map.emplace(texture_path, std::make_shared<Texture>(Texture(_get_sdl_texture(texture_path))));
  return _texture_map.end()->second;
}

std::shared_ptr<Texture> TextureManager::get_font_texture(const std::string& text, const std::string& font_path, int text_height, Zen::Color text_color) {
  std::string usable_font_path = "Resources//TTFs//" + font_path;
  TTF_Font* font = TTF_OpenFont(usable_font_path.c_str(), text_height);
  SDL_Color sdl_color = {.r = text_color.get_red(), .g = text_color.get_green(), .b = text_color.get_blue(), .a = text_color.get_alpha()};
  SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), sdl_color);
  SDL_Texture* sdl_texture = SDL_CreateTextureFromSurface(_renderer, surface);
  TTF_CloseFont(font);
  SDL_FreeSurface(surface);
  return std::make_shared<Texture>(Texture(sdl_texture));
}

SDL_Texture* TextureManager::_get_sdl_texture(const std::string& texture_path){
  return IMG_LoadTexture(_renderer, texture_path.c_str());
}
}
