//
// Created by jdemoss on 11/22/19.
//

#include "Rectangle.h"
#include "Window.h"

Window::Window() {}
Window::Window(const std::string &name, Rectangle WindowRectangle) {
    SdlWindow = SDL_CreateWindow(name.c_str(),
                              (int) WindowRectangle.GetX(), (int) WindowRectangle.GetY(),
                              (int) WindowRectangle.GetWidth(), (int) WindowRectangle.GetHeight(),
                              SDL_WINDOW_SHOWN);
}

Window::~Window() {
    SDL_DestroyWindow(SdlWindow);
}

SDL_Window* Window::GetWindow() {
    return SdlWindow;
}

void Window::SetWindowX(int NewX) {
    int OldY;
    SDL_GetWindowPosition(SdlWindow, nullptr, &OldY);
    SDL_SetWindowPosition(SdlWindow, NewX, OldY);
}

void Window::SetWindowY(int NewY) {
    int OldX;
    SDL_GetWindowPosition(SdlWindow, &OldX, nullptr);
    SDL_SetWindowPosition(SdlWindow, OldX, NewY);
}

int Window::GetWindowX() {
    int CurrentX;
    SDL_GetWindowPosition(SdlWindow, &CurrentX, nullptr);
    return CurrentX;
}

int Window::GetWindowY() {
    int CurrentY;
    SDL_GetWindowPosition(SdlWindow, nullptr, &CurrentY);
    return CurrentY;
}

void Window::SetHeight(int NewHeight) {
    int OldWidth;
    SDL_GetWindowSize(SdlWindow, &OldWidth, nullptr);
    SDL_SetWindowSize(SdlWindow, OldWidth, NewHeight);
}

void Window::SetWidth(int NewWidth) {
    int OldHeight;
    SDL_GetWindowSize(SdlWindow, nullptr, &OldHeight);
    SDL_SetWindowSize(SdlWindow, NewWidth, OldHeight);
}

int Window::GetHeight() {
    int CurrentHeight;
    SDL_GetWindowSize(SdlWindow, nullptr, &CurrentHeight);
    return CurrentHeight;
}

int Window::GetWidth() {
    int CurrentWidth;
    SDL_GetWindowSize(SdlWindow, &CurrentWidth, nullptr);
    return CurrentWidth;
}

