#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string.h>
#include "pacman.hpp"



class Scoreboard {
	public:
	void free1();
	Scoreboard();
	Scoreboard(SDL_Rect* rect, Window* window, Pacman* p1, Pacman* p2, std::string bgType);
	void free();
	void loadTexture(Window* window);
	void loadRenderedText(Window* window);
	void render(Window* window);

	std::string bgType; 
	SDL_Rect location;
	SDL_Texture* bgTexture;
	SDL_Texture* P1;
	SDL_Texture* scoreP1;
	SDL_Texture* livesP1;
	SDL_Texture* P2;
	SDL_Texture* scoreP2;
	SDL_Texture* livesP2;
	SDL_Texture* timer;
	SDL_Texture* pacman;
	SDL_Texture* pacDie;

	TTF_Font*  scoreFont;
	TTF_Font* timerFont;
	std::stringstream timeText;
	std::stringstream scoreP1Text;
	std::stringstream livesP1Text;
	std::stringstream scoreP2Text;
	std::stringstream livesP2Text;

	Pacman* p1; 
	Pacman* p2;

	Uint32 start;
	int time;
	int lifeCount1;
	int lifeCount2;
	int frameCount1;
	int frameCount2;
};

