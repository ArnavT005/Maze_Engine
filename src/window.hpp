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

Window::Window() {
    success = true;
    window = SDL_CreateWindow("Default", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1100, 1100, SDL_WINDOW_SHOWN);
    if(window == NULL) {
        std::cout << "Unable to create window! SDL Error: " << SDL_GetError() << "\n";
        success = false;
    }
    else {
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if(renderer == NULL) {
            std::cout << "Unable to create window renderer! SDL Error: " << SDL_GetError() << "\n";
            success = false;
        }
    }
}

Window::Window(const char* name, int w, int h, int x, int y, int flag1, int flag2, int flag3) {
    success = true;
    window = SDL_CreateWindow(name, x, y, w, h, flag1);
    if(window == NULL) {
        std::cout << "Unable to create window! SDL Error: " << SDL_GetError() << "\n";
        success = false;
    }
    else {
        renderer = SDL_CreateRenderer(window, flag2, flag3);
        if(renderer == NULL) {
            std::cout << "Unable to create window renderer! SDL Error: " << SDL_GetError() << "\n";
            success = false;
        }
    }
}

Window::~Window() {
    free();
}

void Window::free() {
    if(renderer != NULL) {
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
    }
    if(window != NULL) {
        SDL_DestroyWindow(window);
        window = NULL;
    }    
}

SDL_Window* Window::getWindow() {
    return window;
}

SDL_Renderer* Window::getRenderer() {
    return renderer;
}

bool Window::getSuccess() {
    return success;
}

void Window::renderTexture(SDL_Texture* texture, SDL_Rect* srcrect, SDL_Rect* dstrect, double angle, SDL_Point* center, SDL_RendererFlip flip) {
    SDL_RenderCopyEx(renderer, texture, srcrect, dstrect, angle, center, flip);
}

void Window::renderRect(SDL_Rect* rect, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, rect);
}

void Window::renderCircle(Circle* circle, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    int radius = circle->radius;
    int x0 = circle->center.x, y0 = circle->center.y;
    for(int x = x0 - radius; x < x0 + radius; x ++) {
        for(int y = y0 - radius; y < y0 + radius; y ++) {
            int distanceSq = (x0 - x) * (x0 - x) + (y0 - y) * (y0 - y);
            if(distanceSq <= radius * radius) {
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

void Window::setRenderTarget(SDL_Texture *texture) {
    SDL_SetRenderTarget(renderer, texture);
}

void Window::clearWindow() {
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);
}

void Window::updateWindow() {
    SDL_RenderPresent(renderer);
}
