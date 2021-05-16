#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "window.hpp"



class Portal {

	public:

	void free();
	Portal();
	Portal(int screenX, int screenY, Window* window);
	void loadTexture(Window* window);
	void render(Window* window);
	void closePortal();
	void updatePortal();
	int screenX, screenY;
	bool isOpening;
	bool isClosing;
	bool isActive;
	bool isIdle;
	int frameCount;
	Uint32 start;
	SDL_Texture* opening;
	SDL_Texture* active;
	SDL_Texture* closing;

	Mix_Chunk* teleport;
};