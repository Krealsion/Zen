#include "texture_manager.h"

#include <cstring>
#include <ranges>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

namespace Zen {
std::map<std::string, std::shared_ptr<Texture>> TextureManager::_texture_map;
SDL_Renderer* TextureManager::_renderer = nullptr;

void TextureManager::init_ttf() {
  if (!TTF_Init()) {
    // TODO Add error handling
    std::string error_message = "Failed to initialize TTF: " + std::string(SDL_GetError());
    std::cout << error_message;
  }
}

void TextureManager::unload_textures() {
  for (const auto& value : _texture_map | std::views::values) {
    SDL_DestroyTexture(value->_get_sdl_texture());
  }
  _texture_map.clear();
}

std::shared_ptr<Texture> TextureManager::get_texture(const std::string& texture_path) {
  if (!_renderer)
    return std::make_shared<Texture>(Texture());

  if (_texture_map.contains(texture_path)) {
    return _texture_map.find(texture_path)->second;
  }

  _texture_map.emplace(texture_path, std::make_shared<Texture>(Texture(_load_sdl_texture(texture_path))));
  return _texture_map.end()->second;
}

std::shared_ptr<Texture> TextureManager::get_text_texture(const std::string& text, const std::string& font_path, float text_size, Zen::Color text_color) {
  std::string usable_font_path = "Resources//TTFs//" + font_path;
  TTF_Font* font = TTF_OpenFont(usable_font_path.c_str(), text_size);
  SDL_Color sdl_color = {.r = text_color.get_red(), .g = text_color.get_green(), .b = text_color.get_blue(), .a = text_color.get_alpha()};
  SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(font, text.c_str(), std::strlen(text.c_str()), sdl_color, 0);
  SDL_Texture* sdl_texture = SDL_CreateTextureFromSurface(_renderer, surface);
  TTF_CloseFont(font);
  SDL_DestroySurface(surface);
  return std::make_shared<Texture>(Texture(sdl_texture));
}

Vector2 TextureManager::get_text_size(const std::string& text, const std::string& font_path, float text_size) {
  return get_text_size(text, font_path, text_size, 0);
}

Vector2 TextureManager::get_text_size(const std::string& text, const std::string& font_path, float text_size, int max_width) {
  std::string usable_font_path = "..//Resources//TTFs//" + font_path;
  TTF_Init();
  TTF_Font* font = TTF_OpenFont(usable_font_path.c_str(), text_size);
  if (!font) {
    // TODO Add error handling
    std::string error_message = "Failed to load font: " + usable_font_path + " Error: " + SDL_GetError();
    std::cout << error_message;
    return {0,0};
  }
  int x, y;
  TTF_GetStringSizeWrapped(font, text.c_str(), std::strlen(text.c_str()), max_width, &x, &y);
  TTF_CloseFont(font);
  return {x, y};
}

SDL_Texture* TextureManager::_load_sdl_texture(const std::string& texture_path){
  return IMG_LoadTexture(_renderer, texture_path.c_str());
}
}
