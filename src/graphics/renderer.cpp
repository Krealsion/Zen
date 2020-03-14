#include "renderer.h"

#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

namespace Zen {
Renderer::Renderer(const std::string& name, Rectangle window_rectangle) : game_window(name, window_rectangle) {
  SDL_Init(SDL_INIT_EVERYTHING);
  TTF_Init();
  renderer = SDL_CreateRenderer(game_window.get_window(), 0, SDL_RENDERER_ACCELERATED);
  SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0x00);
}

Renderer::~Renderer() {
  SDL_DestroyRenderer(renderer);
  for (auto const&[key, value] : _texture_map) {
    SDL_DestroyTexture(value);
  }
}

void Renderer::render_game_graphics(GameGraphics& game_graphics) {
  if (game_graphics.get_clear_before_draw()) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
  }
  SDL_RenderClear(renderer);
  game_graphics.draw(renderer);
  SDL_RenderPresent(renderer);
}

SDL_Texture* Renderer::load_texture(const std::string& path) {
  SDL_Texture* NewTexture = IMG_LoadTexture(renderer, path.c_str());
  return NewTexture;
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
  TTF_Font* Font = TTF_OpenFont(font_name.c_str(), size);
  SDL_Surface* Surface = TTF_RenderText_Solid(Font, text.c_str(), color);
  SDL_Texture* TexHolder = SDL_CreateTextureFromSurface(renderer, Surface);
  TTF_CloseFont(Font);
  SDL_FreeSurface(Surface);
  return TexHolder;
}

void
Renderer::render_obj(SDL_Texture* texture, SDL_Rect* destination, SDL_Rect* clipping, double angle, SDL_Point* origin) {
  SDL_RenderCopyEx(renderer, texture, clipping, destination, angle, origin, SDL_FLIP_NONE);
}

void Renderer::render_obj(SDL_Texture* texture, SDL_Rect* destination) {
  SDL_RenderCopy(renderer, texture, nullptr, destination);
}
}
