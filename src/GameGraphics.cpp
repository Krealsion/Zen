//
// Created by jdemoss on 11/20/19.
//

#include "GameGraphics.h"

#include <functional>
#include <algorithm>
#include <utility>
#include <math.h>

struct PriorityDrawable {
    int Layer;
    float Sublayer;
    std::function<void(SDL_Renderer*)> DrawFunction;

    PriorityDrawable(std::function<void(SDL_Renderer*)> drawFunction, int layer, float sublayer) {
        Layer = layer;
        Sublayer = sublayer;
        DrawFunction = std::move(drawFunction);
    }
};

GameGraphics::GameGraphics() {
    DrawList = std::vector<PriorityDrawable*>();
}

void GameGraphics::Draw(SDL_Renderer* renderer) {
    std::sort(DrawList.begin(), DrawList.end(), [&renderer](PriorityDrawable* a, PriorityDrawable* b) {
        if (a->Layer == b->Layer) {
            return a->Sublayer - b->Sublayer;
        }
        return (float) (a->Layer - b->Layer);
    });
    for (PriorityDrawable* priorityDrawable : DrawList) {
        priorityDrawable->DrawFunction(renderer);
        delete (priorityDrawable);
    }
    DrawList.clear();
}

void GameGraphics::FillRectangle(const Rectangle& rectangle, Color color, int layer, float sublayer, bool useCamera) {
    DrawList.emplace_back(new PriorityDrawable([=](SDL_Renderer* renderer) {
        SetRendererColor(renderer, color);
        SDL_Rect* sdlRect = ToSDLRect(rectangle, useCamera);
        SDL_RenderFillRect(renderer, sdlRect);
        delete (sdlRect);
    }, layer, sublayer));
}

void GameGraphics::DrawRectangle(const Rectangle& rectangle, Color color, int layer, float sublayer, bool useCamera) {
    DrawList.emplace_back(new PriorityDrawable([=](SDL_Renderer* renderer) {
        SetRendererColor(renderer, color);
        SDL_Rect* sdlRect = ToSDLRect(rectangle, useCamera);
        SDL_RenderDrawRect(renderer, sdlRect);
        delete (sdlRect);
    }, layer, sublayer));
}

void GameGraphics::SetRendererColor(SDL_Renderer* renderer, Color color) {
    SDL_SetRenderDrawColor(renderer, color.GetRed(), color.GetGreen(), color.GetGreen(), color.GetAlpha());
}

SDL_Rect* GameGraphics::ToSDLRect(Rectangle rectangle, bool useCamera) {
    SDL_Rect* rect = new SDL_Rect;
    rect->x = rectangle.GetPosition().GetXInt();
    rect->y = rectangle.GetPosition().GetYInt();
    rect->h = rectangle.GetSize().GetXInt();
    rect->w = rectangle.GetSize().GetYInt();
    if (useCamera) {
        rect->x += Camera.GetXInt();
        rect->y += Camera.GetYInt();
    }
    return rect;
}

void GameGraphics::DrawOval(const Rectangle& ovalBounds, Color color, int layer, int sublayer, bool useCamera) {
    DrawList.emplace_back(new PriorityDrawable([=](SDL_Renderer* renderer) {
        SetRendererColor(renderer, color);
        Vector2 center = ovalBounds.GetPosition().Add(ovalBounds.GetSize().Scale(.5));
        double d = 2 * M_PI * sqrt((pow(ovalBounds.GetWidth() / 2, 2) + pow(ovalBounds.GetHeight() / 2, 2)) / 2);
        double step = M_PI / 4 / d;
        for (int i = 0; i < d; i++) {
            double s = 0;
            double c = 1;
            if (i != 0) {
                s = sin(i * step);
                c = cos(i * step);
            }
            double relativex = c * ovalBounds.GetWidth() / 2;
            double relativey = s * ovalBounds.GetHeight() / 2;
            double relativexb = s * ovalBounds.GetWidth() / 2;
            double relativeyb = c * ovalBounds.GetHeight() / 2;
            SDL_RenderDrawPoint(renderer, round(center.GetX() + relativex), round(center.GetY() + relativey));
            SDL_RenderDrawPoint(renderer, round(center.GetX() + relativex), round(center.GetY() - relativey));
            SDL_RenderDrawPoint(renderer, round(center.GetX() - relativex), round(center.GetY() + relativey));
            SDL_RenderDrawPoint(renderer, round(center.GetX() - relativex), round(center.GetY() - relativey));
            SDL_RenderDrawPoint(renderer, round(center.GetX() + relativexb), round(center.GetY() + relativeyb));
            SDL_RenderDrawPoint(renderer, round(center.GetX() + relativexb), round(center.GetY() - relativeyb));
            SDL_RenderDrawPoint(renderer, round(center.GetX() - relativexb), round(center.GetY() + relativeyb));
            SDL_RenderDrawPoint(renderer, round(center.GetX() - relativexb), round(center.GetY() - relativeyb));
        }
    }, layer, sublayer));
}

void GameGraphics::FillOval(const Rectangle& ovalBounds, Color color, int layer, int sublayer, bool useCamera) {
    DrawList.emplace_back(new PriorityDrawable([=](SDL_Renderer* renderer) {
        SetRendererColor(renderer, color);
        Vector2 center = ovalBounds.GetPosition().Add(ovalBounds.GetSize().Scale(.5));
        if (useCamera) {
            center.Add(Camera);
        }
        double d = 2 * M_PI * sqrt((pow(ovalBounds.GetWidth() / 2, 2) + pow(ovalBounds.GetHeight() / 2, 2)) / 2);
        double step = M_PI / 4 / d;
        for (int i = 0; i < d; i++) {
            double s = 0;
            double c = 1;
            if (i != 0) {
                s = sin(i * step);
                c = cos(i * step);
            }
            double relativex = c * ovalBounds.GetWidth() / 2;
            double relativey = s * ovalBounds.GetHeight() / 2;
            double relativexb = s * ovalBounds.GetWidth() / 2;
            double relativeyb = c * ovalBounds.GetHeight() / 2;
            for (int j = (int) round(center.GetY() - relativey); j < (int) round(center.GetY() + relativey); j++) {
                SDL_RenderDrawPoint(renderer, round(center.GetX() + relativex), j);
                SDL_RenderDrawPoint(renderer, round(center.GetX() - relativex), j);
            }
            for (int j = (int) round(center.GetY() - relativeyb); j < (int) round(center.GetY() + relativeyb); j++) {
                SDL_RenderDrawPoint(renderer, round(center.GetX() + relativexb), j);
                SDL_RenderDrawPoint(renderer, round(center.GetX() - relativexb), j);
            }
        }
    }, layer, sublayer));
}
