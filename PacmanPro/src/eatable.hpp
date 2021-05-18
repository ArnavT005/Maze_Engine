#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "window.hpp"
#include "pacman.hpp"


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
	void setPacman(Pacman* p1, Pacman* p2);
	void loadTexture(Window* window);
	void free();
	void render(Window* window);
	void checkIfEaten(bool& isBuffed);

	SDL_Texture* texture;
	SDL_Rect location;
	Mix_Chunk *powerupLarge;
	Mix_Chunk * powerupSmall;
	int type;
	bool isEaten;
	Pacman* p1;
	Pacman* p2;

	bool success;
};






