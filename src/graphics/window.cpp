
#include "types/rectangle.h"
#include "window.h"

namespace Zen {

Window::Window(const std::string& name, Rectangle window_rectangle) {
  sdl_window = SDL_CreateWindow(name.c_str(),
                            (int) window_rectangle.get_width(),
                             (int) window_rectangle.get_height(),
                          SDL_WINDOW_RESIZABLE);
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
  int current_x;
  SDL_GetWindowPosition(sdl_window, &current_x, nullptr);
  return current_x;
}

int Window::get_y() {
  int current_y;
  SDL_GetWindowPosition(sdl_window, nullptr, &current_y);
  return current_y;
}

void Window::set_height(int new_height) {
  int old_width;
  SDL_GetWindowSize(sdl_window, &old_width, nullptr);
  SDL_SetWindowSize(sdl_window, old_width, new_height);
}

void Window::set_width(int new_width) {
  int old_height;
  SDL_GetWindowSize(sdl_window, nullptr, &old_height);
  SDL_SetWindowSize(sdl_window, new_width, old_height);
}

int Window::get_height() {
  int current_height;
  SDL_GetWindowSize(sdl_window, nullptr, &current_height);
  return current_height;
}

int Window::get_width() {
  int current_width;
  SDL_GetWindowSize(sdl_window, &current_width, nullptr);
  return current_width;
}
}
