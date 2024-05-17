#include "renderer.h"

#include "texture_manager.h"

#include <SDL.h>
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
}

void Renderer::render_game_graphics(GameGraphics& game_graphics) {
  if (game_graphics.get_clear_before_draw()) {
    SDL_SetRenderDrawColor(_renderer, 255, 0, 255, 255);
    SDL_RenderClear(_renderer);
  }
  game_graphics.draw(_renderer);
  SDL_RenderPresent(_renderer);
}

Window* Renderer::get_window() {
  return &_game_window;
}
}
