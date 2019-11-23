//
// Created by jdemoss on 11/22/19.
//

#ifndef LEAVINGTERRA_WINDOW_H
#define LEAVINGTERRA_WINDOW_H

#include <SDL_video.h>

class Window {
public:
    Window(Rectangle WindowRectangle);
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

#endif //LEAVINGTERRA_WINDOW_H
