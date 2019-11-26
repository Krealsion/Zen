#include "Renderer.h"

#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <vector>

Renderer::Renderer(const std::string& name, Rectangle windowRectangle) : GameWindow(name, windowRectangle){
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    Render = SDL_CreateRenderer(GameWindow.GetWindow() , 0, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(Render, 0xFF, 0xFF, 0xFF, 0x00);
    Paths.clear();
}

Renderer::~Renderer() {
    SDL_DestroyRenderer(Render);
    Paths.clear();
    Textures.clear();
}

void Renderer::RenderGameGraphics(GameGraphics& gameGraphics) {
    SDL_SetRenderDrawColor(Render, 0, 0, 255, 255);
    SDL_RenderClear(Render);
    gameGraphics.Draw(Render);
    SDL_RenderPresent(Render);
}

SDL_Texture* Renderer::LoadTexture(const std::string& path) {
    SDL_Texture* NewTexture = IMG_LoadTexture(Render, path.c_str());
    return NewTexture;
}

SDL_Texture* Renderer::GetTexture(const std::string& path) {
    for (int i = 0; i < Paths.size(); i++) {
        if (path == Paths[i]) {
            return Textures[i];
        }
    }
    Textures.push_back(LoadTexture(path));
    Paths.push_back(path);
    return Textures[Textures.size() - 1];
}

SDL_Texture* Renderer::LoadText(const std::string& text, std::string fontName, int size, SDL_Color color) {
    fontName = "Resources//TTFs//" + fontName;
    TTF_Font* Font = TTF_OpenFont(fontName.c_str(), size);
    SDL_Surface* Surface = TTF_RenderText_Solid(Font, text.c_str(), color);
    SDL_Texture* TexHolder = SDL_CreateTextureFromSurface(Render, Surface);
    TTF_CloseFont(Font);
    SDL_FreeSurface(Surface);
    return TexHolder;
}

void Renderer::RenderObj(SDL_Texture* texture, SDL_Rect* destination, SDL_Rect* clipping, double angle, SDL_Point* origin) {
    SDL_RenderCopyEx(Render, texture, clipping, destination, angle, origin, SDL_FLIP_NONE);
}

void Renderer::RenderObj(SDL_Texture* texture, SDL_Rect* destination) {
    SDL_RenderCopy(Render, texture, nullptr, destination);
}

