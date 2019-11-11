#ifndef RENDERER
#define RENDERER

#include <vector>
#include <string>
#include <SDL.h>

class Renderer{
	public:
		Renderer(std::string Name, int PosX, int PosY, int ScreenWidth, int ScreenHeight);
		~Renderer();

		SDL_Renderer* GetRenderer();

		void RenderObj(SDL_Texture *Texture, SDL_Rect *Destination, SDL_Rect *Cliping, double Angle, SDL_Point* Origin);
		void RenderObj(SDL_Texture *Texture, SDL_Rect *Destination);

        SDL_Texture* LoadTexture(const std::string &Path);
        SDL_Texture* GetTexture(const std::string &Path);
        SDL_Texture* LoadText(const std::string Text, const std::string FontName, const int Size, SDL_Color Color);

		void RenderStart();
		void RenderFinish();

		SDL_Rect* CameraShift(SDL_Rect* TranslatedObj);

		void SetX(int NewX);
		void SetY(int NewY);
		int GetX();
		int GetY();

		void SetWidth(int NewWidth);
		void SetHeight(int NewHeight);
		int GetHeight();
		int GetWidth();

		int GetFPS();
		int GetRenderTime();

	protected:
	    std::vector<SDL_Texture*> Textures;
	    std::vector<std::string> Paths;

		SDL_Window* Window;
		SDL_Renderer* Render;
		SDL_Point Camera;
};
#endif
