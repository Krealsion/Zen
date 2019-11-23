#ifndef RENDERER
#define RENDERER

#include <vector>
#include <string>
#include <SDL.h>

#include "Rectangle.h"
#include "Window.h"
#include "GameGraphics.h"

/**
 * The renderer is in charge of creating and displaying the window
 * as well as any rendering of objects to the window
 * TODO Will possibly be split into two classes Window and Renderer for simplicity
 * TODO split texture loading and creation into a separate class
 */
class Renderer {
public:
    Renderer();
    Renderer(const std::string& name, Rectangle windowRectangle);
    ~Renderer();

    void RenderObj(SDL_Texture* texture, SDL_Rect* destination, SDL_Rect* clipping, double angle, SDL_Point* origin);
    void RenderObj(SDL_Texture* texture, SDL_Rect* destination);

    SDL_Texture* LoadTexture(const std::string &path);
    SDL_Texture* GetTexture(const std::string &path);
    SDL_Texture* LoadText(const std::string &text, std::string fontName, int size, SDL_Color color);

    void RenderGameGraphics(GameGraphics gameGraphics);

protected:
    void Initialize(const std::string &name, Rectangle rectangle);
    std::vector<SDL_Texture*> Textures;
    std::vector<std::string> Paths;

    Window GameWindow;
    SDL_Renderer* Render;
};

#endif
