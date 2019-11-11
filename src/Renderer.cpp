#include <iostream>
#include <string>
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <vector>

#include "../include/Renderer.h"

Renderer::Renderer(std::string Name, int PosX, int PosY, int ScreenWidth, int ScreenHeight) {
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    Window = SDL_CreateWindow(Name.c_str(), PosX, PosY, ScreenWidth, ScreenHeight, SDL_WINDOW_SHOWN);
    Render = SDL_CreateRenderer(Window, 0, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(Render, 0xFF, 0xFF, 0xFF, 0x00);
    Camera.x = 0;
    Camera.y = 0;
    Paths.clear();
}

Renderer::~Renderer() {
    SDL_DestroyRenderer(Render);
    SDL_DestroyWindow(Window);
    Paths.clear();
    Textures.clear();
}

SDL_Renderer* Renderer::GetRenderer() {
    return Render;
}

void Renderer::RenderStart() {
    SDL_SetRenderDrawColor(Render, 0, 0, 255, 255);
    SDL_RenderClear(Render);
}

void Renderer::RenderFinish() {
    SDL_RenderPresent(Render);
}

SDL_Texture* Renderer::LoadTexture(const std::string& Path) {
    SDL_Texture* NewTexture = IMG_LoadTexture(Render, Path.c_str());
    return NewTexture;
}

SDL_Texture* Renderer::GetTexture(const std::string& Path) {
    for (int i = 0; i < Paths.size(); i++) {
        if (Path == Paths[i]) {
            return Textures[i];
        }
    }
    Textures.push_back(LoadTexture(Path));
    Paths.push_back(Path);
    return Textures[Textures.size() - 1];
}

SDL_Texture* Renderer::LoadText(std::string Text, std::string FontName, int Size, SDL_Color Color) {
    FontName = "Resources//TTFs//" + FontName;
    TTF_Font* Font = TTF_OpenFont(FontName.c_str(), Size);
    SDL_Surface* Surface = TTF_RenderText_Solid(Font, Text.c_str(), Color);
    SDL_Texture* TexHolder = SDL_CreateTextureFromSurface(Render, Surface);
    TTF_CloseFont(Font);
    SDL_FreeSurface(Surface);
    return TexHolder;
}

SDL_Rect* Renderer::CameraShift(SDL_Rect* TranslatedObj) {
    TranslatedObj->x -= Camera.x;
    TranslatedObj->y -= Camera.y;
    return TranslatedObj;
}

void
Renderer::RenderObj(SDL_Texture* Texture, SDL_Rect* Destination, SDL_Rect* Cliping, double Angle, SDL_Point* Origin) {
    SDL_RenderCopyEx(Render, Texture, Cliping, Destination, Angle, Origin, SDL_FLIP_NONE);
}

void Renderer::RenderObj(SDL_Texture* Texture, SDL_Rect* Destination) {
    SDL_RenderCopy(Render, Texture, NULL, Destination);
}

void Renderer::SetX(int NewX) {
    int OldY;
    SDL_GetWindowPosition(Window, nullptr, &OldY);
    SDL_SetWindowPosition(Window, NewX, OldY);
}

void Renderer::SetY(int NewY) {
    int OldX;
    SDL_GetWindowPosition(Window, &OldX, nullptr);
    SDL_SetWindowPosition(Window, OldX, NewY);
}

int Renderer::GetX() {
    int CurrentX;
    SDL_GetWindowPosition(Window, &CurrentX, nullptr);
    return CurrentX;
}

int Renderer::GetY() {
    int CurrentY;
    SDL_GetWindowPosition(Window, nullptr, &CurrentY);
    return CurrentY;
}

void Renderer::SetHeight(int NewHeight) {
    int OldWidth;
    SDL_GetWindowSize(Window, &OldWidth, nullptr);
    SDL_SetWindowSize(Window, OldWidth, NewHeight);
}

void Renderer::SetWidth(int NewWidth) {
    int OldHeight;
    SDL_GetWindowSize(Window, nullptr, &OldHeight);
    SDL_SetWindowSize(Window, NewWidth, OldHeight);
}

int Renderer::GetHeight() {
    int CurrentHeight;
    SDL_GetWindowSize(Window, nullptr, &CurrentHeight);
    return CurrentHeight;
}

int Renderer::GetWidth() {
    int CurrentWidth;
    SDL_GetWindowSize(Window, &CurrentWidth, nullptr);
    return CurrentWidth;
}
