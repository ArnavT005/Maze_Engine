#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include "window.hpp"

enum BUTTON {
	IDLE, 
	HOVER, 
	PRESSED
};

class Menu {
	public:

	Menu();
	Menu(Window* window);
	void free();
	void loadTexture(Window* window);
	void render(Window* window);
	void handleEvent(SDL_Event &e);

	bool isAtMenuStart;
	bool isAtMenuMode;
	bool isAtEnd;
	bool isRunning;
	bool isOver;
	bool onlinePossible;
	int button1State;
	int button2State;
	int button3State;
	int winner;
	int mode;
	SDL_Texture* background;
	SDL_Texture* gameOver;
	SDL_Texture* connecting;

	SDL_Texture* rematchButton1;
	SDL_Texture* rematchButton2;
	SDL_Texture* backToMenuButton1;
	SDL_Texture* backToMenuButton2;
	SDL_Texture* newGameButton1;
	SDL_Texture* newGameButton2;
	SDL_Texture* localButton1;
	SDL_Texture* localButton2;
	SDL_Texture* onlineButton1;
	SDL_Texture* onlineButton2;
	SDL_Texture* backButton1;
	SDL_Texture* backButton2;
	SDL_Texture* disco;
	SDL_Texture* youWin;
	SDL_Texture* offline;


	SDL_Texture* P1;
	SDL_Texture* P2;
	SDL_Texture* draw;

	Mix_Chunk* click;

};