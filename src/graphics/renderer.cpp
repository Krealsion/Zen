#include "renderer.h"

#include "texture_manager.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <string>

namespace Zen {
Renderer::Renderer(const std::string& name, Rectangle window_rectangle) : _game_window(name, window_rectangle) {
  SDL_Init(SDL_INIT_EVERYTHING);
  TTF_Init();
  _renderer = SDL_CreateRenderer(_game_window.get_window(), 0, SDL_RENDERER_ACCELERATED);
  SDL_SetRenderDrawColor(_renderer, 0xFF, 0xFF, 0xFF, 0x00);
  TextureManager::_set_renderer(_renderer);
}

Renderer::~Renderer() {
  SDL_DestroyRenderer(_renderer);
  for (auto const&[key, value] : _texture_map) {
    SDL_DestroyTexture(value);
  }
}

void Renderer::render_game_graphics(GameGraphics& game_graphics) {
  if (game_graphics.get_clear_before_draw()) {
    SDL_SetRenderDrawColor(_renderer, 0, 0, 255, 255);
  }
  SDL_RenderClear(_renderer);
  game_graphics.draw(_renderer);
  SDL_RenderPresent(_renderer);
}

SDL_Texture* Renderer::load_texture(const std::string& path) {
  SDL_Texture* new_texture = IMG_LoadTexture(_renderer, path.c_str());
  return new_texture;
}

SDL_Texture* Renderer::get_texture(const std::string& path) {
  if (_texture_map.count(path) != 0) {
    return _texture_map.find(path)->second;
  }
  auto texture = load_texture(path);
  _texture_map.emplace(path, texture);
  return texture;
}

SDL_Texture* Renderer::load_text(const std::string& text, std::string font_name, int size, SDL_Color color) {
  font_name = "Resources//TTFs//" + font_name;
  TTF_Font* font = TTF_OpenFont(font_name.c_str(), size);
  SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
  SDL_Texture* return_texture = SDL_CreateTextureFromSurface(_renderer, surface);
  TTF_CloseFont(font);
  SDL_FreeSurface(surface);
  return return_texture;
}

void
Renderer::render_obj(SDL_Texture* texture, SDL_Rect* destination, SDL_Rect* clipping, double angle, SDL_Point* origin) {
  SDL_RenderCopyEx(_renderer, texture, clipping, destination, angle, origin, SDL_FLIP_NONE);
}

void Renderer::render_obj(SDL_Texture* texture, SDL_Rect* destination) {
  SDL_RenderCopy(_renderer, texture, nullptr, destination);
}
}
