#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "window.hpp"


enum EAT_TYPE {
	NONE,
	TEN_POINT,
	FIFTY_POINT
};

class Eatable {
	
	public:
	Eatable();
	Eatable(int x, int y, int w, int h, int type, Window* window);
	void setValues(int x, int y, int w, int h, int type, Window* window);
	void loadTexture(Window* window);
	void free();
	void render(Window* window);

	SDL_Texture* texture;
	SDL_Rect location;
	int type;
	bool isEaten;

	bool success;
};






