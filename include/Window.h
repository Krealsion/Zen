//
// Created by jdemoss on 11/22/19.
//

#ifndef WINDOW_H
#define WINDOW_H

#include <SDL_video.h>

class Window {
public:
    Window();
    Window(const std::string &name, Rectangle WindowRectangle);
    ~Window();

    SDL_Window* GetWindow();

    void SetWindowX(int NewX);
    void SetWindowY(int NewY);
    int GetWindowX();
    int GetWindowY();

    void SetWidth(int NewWidth);
    void SetHeight(int NewHeight);
    int GetHeight();
    int GetWidth();

private:
    SDL_Window* SdlWindow;
};

#endif //WINDOW_H
