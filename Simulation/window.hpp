#pragma once

#include <SDL2/SDL.h>
#include <iostream>

struct Circle {
    int radius;
    SDL_Point center;
};


class Window {
    
    public:
    Window();
    Window(const char* name, int w, int h, int x = SDL_WINDOWPOS_UNDEFINED, int y = SDL_WINDOWPOS_UNDEFINED, int flag1 = SDL_WINDOW_SHOWN, int flag2 = -1, int flag3 = SDL_RENDERER_ACCELERATED);
    ~Window();
    SDL_Window* getWindow();
    SDL_Renderer* getRenderer();
    bool getSuccess();
    void free();
    void renderTexture(SDL_Texture* texture, SDL_Rect* srcrect = NULL, SDL_Rect* dstrect = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
    void renderRect(SDL_Rect* rect, SDL_Color color);
    void renderCircle(Circle* circle, SDL_Color color);
    void setRenderTarget(SDL_Texture* texture);
    void clearWindow();
    void updateWindow();
    
    private:
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    bool success;
};
