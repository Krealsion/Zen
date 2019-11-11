#ifndef RENDERER
#define RENDERER

#include <vector>
#include <string>
#include <SDL.h>

#include "Rectangle.h"

/**
 * The renderer is in charge of creating and displaying the window
 * as well as any rendering of objects to the window
 * TODO Will possibly be split into two classes Window and Renderer for simplicity
 * TODO split texture loading and creation into a separate class
 */
class Renderer {
public:
    Renderer();
    Renderer(const std::string& Name, Rectangle WindowRectangle);
    ~Renderer();

    void RenderObj(SDL_Texture* Texture, SDL_Rect* Destination, SDL_Rect* Clipping, double Angle, SDL_Point* Origin);
    void RenderObj(SDL_Texture* Texture, SDL_Rect* Destination);

    SDL_Texture* LoadTexture(const std::string &Path);
    SDL_Texture* GetTexture(const std::string &Path);
    SDL_Texture* LoadText(const std::string &Text, std::string FontName, int Size, SDL_Color Color);

    void RenderStart();
    void RenderFinish();

    SDL_Rect* CameraShift(SDL_Rect* TranslatedObj);

    void SetWindowX(int NewX);
    void SetWindowY(int NewY);
    int GetWindowX();
    int GetWindowY();

    void SetWidth(int NewWidth);
    void SetHeight(int NewHeight);
    int GetHeight();
    int GetWidth();

protected:
    std::vector<SDL_Texture*> Textures;
    std::vector<std::string> Paths;

    SDL_Window* Window;
    SDL_Renderer* Render;
    Vector2 Camera;
};

#endif
