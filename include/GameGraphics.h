//
// Created by jdemoss on 11/20/19.
//

#ifndef GAMEGRAPHICS_H
#define GAMEGRAPHICS_H

#include "Color.h"
#include "Rectangle.h"

#include <vector>
#include <SDL_rect.h>
#include <SDL_render.h>

struct PriorityDrawable;

class GameGraphics {
public:
    GameGraphics();

    void Draw(SDL_Renderer* renderer);

    void DrawRectangle(const Rectangle &rectangle, Color color, int layer = 1, float sublayer = 1, bool useCamera = true);

private:
    SDL_Rect* ToSDLRect(Rectangle rectangle, bool useCamera = false);
    static void SetRendererColor(SDL_Renderer* renderer, Color color);

    Vector2 Camera;
    std::vector<PriorityDrawable> DrawList;
};

#endif //GAMEGRAPHICS_H
