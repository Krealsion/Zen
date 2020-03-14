
#include "types/rectangle.h"
#include "window.h"

namespace Zen {

Window::Window(const std::string& name, Rectangle window_rectangle) {
  sdl_window = SDL_CreateWindow(name.c_str(),
                                (int) window_rectangle.get_x(), (int) window_rectangle.get_y(),
                                (int) window_rectangle.get_width(), (int) window_rectangle.get_height(),
                                SDL_WINDOW_SHOWN);
}

Window::~Window() {
  SDL_DestroyWindow(sdl_window);
}

SDL_Window* Window::get_window() {
  return sdl_window;
}

void Window::set_x(int new_x) {
  int old_y;
  SDL_GetWindowPosition(sdl_window, nullptr, &old_y);
  SDL_SetWindowPosition(sdl_window, new_x, old_y);
}

void Window::set_y(int new_y) {
  int old_y;
  SDL_GetWindowPosition(sdl_window, &old_y, nullptr);
  SDL_SetWindowPosition(sdl_window, old_y, new_y);
}

int Window::get_x() {
  int CurrentX;
  SDL_GetWindowPosition(sdl_window, &CurrentX, nullptr);
  return CurrentX;
}

int Window::get_y() {
  int CurrentY;
  SDL_GetWindowPosition(sdl_window, nullptr, &CurrentY);
  return CurrentY;
}

void Window::set_height(int new_height) {
  int OldWidth;
  SDL_GetWindowSize(sdl_window, &OldWidth, nullptr);
  SDL_SetWindowSize(sdl_window, OldWidth, new_height);
}

void Window::set_width(int new_width) {
  int OldHeight;
  SDL_GetWindowSize(sdl_window, nullptr, &OldHeight);
  SDL_SetWindowSize(sdl_window, new_width, OldHeight);
}

int Window::get_height() {
  int CurrentHeight;
  SDL_GetWindowSize(sdl_window, nullptr, &CurrentHeight);
  return CurrentHeight;
}

int Window::get_width() {
  int CurrentWidth;
  SDL_GetWindowSize(sdl_window, &CurrentWidth, nullptr);
  return CurrentWidth;
}
}
