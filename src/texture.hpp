#pragma once

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Texture
{
    public:
    Texture();
    ~Texture();
    void free();
    bool loadImgTexture(std::string path, SDL_Renderer* renderer);

    SDL_Texture* texture;
    int width;
    int height;
};

Texture::Texture() {
    texture = NULL;
    width = 0;
    height = 0;
}

Texture::~Texture() {
    free();
}

void Texture::free() {
    if(texture != NULL) {
        SDL_DestroyTexture(texture);
        texture = NULL;
        width = 0;
        height = 0;
    }
}

bool Texture::loadImgTexture(std::string path, SDL_Renderer* renderer) {
    free();
    bool success = true;
    SDL_Surface* image = IMG_Load(path.c_str());
    if(image == NULL) {
        std::cout << "Unable to load image from path " << path.c_str() <<"! SDL_Image Error: " << IMG_GetError() << "\n";
        success = false; 
    }
    else {
        texture = SDL_CreateTextureFromSurface(renderer, image);
        if(texture == NULL) {
            std::cout << "Unable to create texture! SDL Error: " << SDL_GetError() << "\n";
            success = false;
        }
        else {
            width = image->w;
            height = image->h;
        }
        SDL_FreeSurface(image);
    }
    return success;

}