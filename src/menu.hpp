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
	int buttonState;
	SDL_Texture* background;
	SDL_Texture* newGameButton1;
	SDL_Texture* newGameButton2;
	SDL_Texture* localButton;
	SDL_Texture* onlineButton;

	SDL_Texture* backToMenu;

};


Menu::Menu() {
	
	isAtMenuStart = true;
	isAtMenuMode = false;
	isAtEnd = false;
	isRunning = false;
	isOver = false;
	buttonState = IDLE;

	background = NULL;
	newGameButton1 = NULL;
	newGameButton2 = NULL;
	localButton = NULL;
	onlineButton = NULL;
	backToMenu = NULL;
}

Menu::Menu(Window* window) {
	
	isAtMenuStart = true;
	isAtMenuMode = false;
	isAtEnd = false;
	isRunning = false;
	isOver = false;
	buttonState = IDLE;

	loadTexture(window);
}

void Menu::free() {
	if(background != NULL) {
		SDL_DestroyTexture(background);
		background = NULL;
	}
	if(newGameButton1 != NULL) {
		SDL_DestroyTexture(newGameButton1);
		newGameButton1 = NULL;
	}
	if(newGameButton2 != NULL) {
		SDL_DestroyTexture(newGameButton2);
		newGameButton2 = NULL;
	}
	if(localButton != NULL) {
		SDL_DestroyTexture(localButton);
		localButton = NULL;
	}
	if(onlineButton != NULL) {
		SDL_DestroyTexture(onlineButton);
		onlineButton = NULL;
	}
	if(backToMenu != NULL) {
		SDL_DestroyTexture(backToMenu);
		backToMenu = NULL;
	}
}

void Menu::loadTexture(Window* window) {
	SDL_Surface* bgSurf = IMG_Load("../img/Menu.png");
	if(bgSurf == NULL) {
		std::cout << "Unable to load Up motion sprite! SDL_Image Error: " << IMG_GetError() << "\n";
	}
	else {
		background = SDL_CreateTextureFromSurface(window->getRenderer(), bgSurf);
		if(background == NULL) {
			std::cout << "Unable to create texture from sprite! SDL Error: " << SDL_GetError() << "\n";
		}
		SDL_FreeSurface(bgSurf);
	}
	SDL_Surface* newGameSurf1 = IMG_Load("../img/newGameUnPressed.png");
	if(newGameSurf1 == NULL) {
		std::cout << "Unable to load Up motion sprite! SDL_Image Error: " << IMG_GetError() << "\n";
	}
	else {
		newGameButton1 = SDL_CreateTextureFromSurface(window->getRenderer(), newGameSurf1);
		if(newGameButton1 == NULL) {
			std::cout << "Unable to create texture from sprite! SDL Error: " << SDL_GetError() << "\n";
		}
		SDL_FreeSurface(newGameSurf1);
	}
	SDL_Surface* newGameSurf2 = IMG_Load("../img/newGameHover.png");
	if(newGameSurf2 == NULL) {
		std::cout << "Unable to load Up motion sprite! SDL_Image Error: " << IMG_GetError() << "\n";
	}
	else {
		newGameButton2 = SDL_CreateTextureFromSurface(window->getRenderer(), newGameSurf2);
		if(newGameButton2 == NULL) {
			std::cout << "Unable to create texture from sprite! SDL Error: " << SDL_GetError() << "\n";
		}
		SDL_FreeSurface(newGameSurf2);
	}
	SDL_Surface* localSurf = IMG_Load("../img/pacman/upMotion.png");
	if(localSurf == NULL) {
		std::cout << "Unable to load Up motion sprite! SDL_Image Error: " << IMG_GetError() << "\n";
	}
	else {
		localButton = SDL_CreateTextureFromSurface(window->getRenderer(), localSurf);
		if(localButton == NULL) {
			std::cout << "Unable to create texture from sprite! SDL Error: " << SDL_GetError() << "\n";
		}
		SDL_FreeSurface(localSurf);
	}
	SDL_Surface* onlineSurf = IMG_Load("../img/pacman/upMotion.png");
	if(onlineSurf == NULL) {
		std::cout << "Unable to load Up motion sprite! SDL_Image Error: " << IMG_GetError() << "\n";
	}
	else {
		onlineButton = SDL_CreateTextureFromSurface(window->getRenderer(), onlineSurf);
		if(onlineButton == NULL) {
			std::cout << "Unable to create texture from sprite! SDL Error: " << SDL_GetError() << "\n";
		}
		SDL_FreeSurface(onlineSurf);
	}
	SDL_Surface* backSurf = IMG_Load("../img/pacman/upMotion.png");
	if(backSurf == NULL) {
		std::cout << "Unable to load Up motion sprite! SDL_Image Error: " << IMG_GetError() << "\n";
	}
	else {
		backToMenu = SDL_CreateTextureFromSurface(window->getRenderer(), backSurf);
		if(backToMenu == NULL) {
			std::cout << "Unable to create texture from sprite! SDL Error: " << SDL_GetError() << "\n";
		}
		SDL_FreeSurface(backSurf);
	}
}

void Menu::render(Window* window) {
	if(isAtMenuStart) {
		SDL_Rect bgRect = {0, 0, 1405, 1025};
		SDL_Rect buttonRect = {593, 830, 221, 71};
		window->renderTexture(background, NULL, &bgRect);
		if(buttonState == HOVER || buttonState == PRESSED) {
			window->renderTexture(newGameButton2, NULL, &buttonRect);
		}
		else {
			window->renderTexture(newGameButton1, NULL, &buttonRect);
		}
	}
	else if(isAtMenuMode) {

	}
	else if(isAtEnd) {

	}
}

void Menu::handleEvent(SDL_Event &e) {
	if(isAtMenuStart) {
		if(e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
			int x, y;
			SDL_GetMouseState(&x, &y);
			if(x >= 593 && x <= 809 && y >= 830 && y <= 896) {
				if(e.type == SDL_MOUSEMOTION) {
					buttonState = HOVER;
				}
				else if(e.type == SDL_MOUSEBUTTONDOWN) {
					buttonState = PRESSED;
				}
				else if(e.type == SDL_MOUSEBUTTONUP) {
					if(buttonState == PRESSED) {
						buttonState = IDLE;
						isAtMenuStart = false;
						isAtMenuMode = true;
					}
					else {
						buttonState = HOVER;
					}
				}
			}
			else {
				buttonState = IDLE;
			}
		}
	}
	else if(isAtMenuMode) {

	}
	else if(isAtEnd) {

	}
}