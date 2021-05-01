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
	int button1State;
	int button2State;
	int button3State;
	SDL_Texture* background;
	SDL_Texture* newGameButton1;
	SDL_Texture* newGameButton2;
	SDL_Texture* localButton1;
	SDL_Texture* localButton2;
	SDL_Texture* onlineButton1;
	SDL_Texture* onlineButton2;

	SDL_Texture* backButton1;
	SDL_Texture* backButton2;

};


Menu::Menu() {
	
	isAtMenuStart = true;
	isAtMenuMode = false;
	isAtEnd = false;
	isRunning = false;
	isOver = false;
	button1State = IDLE;
	button2State = IDLE;
	button3State = IDLE;

	background = NULL;
	newGameButton1 = NULL;
	newGameButton2 = NULL;
	localButton1 = NULL;
	localButton2 = NULL;
	onlineButton1 = NULL;
	onlineButton2 = NULL;
	backButton1 = NULL;
	backButton2 = NULL;
}

Menu::Menu(Window* window) {
	
	isAtMenuStart = true;
	isAtMenuMode = false;
	isAtEnd = false;
	isRunning = false;
	isOver = false;
	button1State = IDLE;
	button2State = IDLE;
	button3State = IDLE;

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
	if(localButton1 != NULL) {
		SDL_DestroyTexture(localButton1);
		localButton1 = NULL;
	}
	if(onlineButton1 != NULL) {
		SDL_DestroyTexture(onlineButton1);
		onlineButton1 = NULL;
	}
	if(localButton2 != NULL) {
		SDL_DestroyTexture(localButton2);
		localButton2 = NULL;
	}
	if(onlineButton2 != NULL) {
		SDL_DestroyTexture(onlineButton2);
		onlineButton2 = NULL;
	}
	if(backButton1 != NULL) {
		SDL_DestroyTexture(backButton1);
		backButton1 = NULL;
	}
	if(backButton2 != NULL) {
		SDL_DestroyTexture(backButton2);
		backButton2 = NULL;
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
	SDL_Surface* localSurf1 = IMG_Load("../img/localUnPressed.png");
	if(localSurf1 == NULL) {
		std::cout << "Unable to load Up motion sprite! SDL_Image Error: " << IMG_GetError() << "\n";
	}
	else {
		localButton1 = SDL_CreateTextureFromSurface(window->getRenderer(), localSurf1);
		if(localButton1 == NULL) {
			std::cout << "Unable to create texture from sprite! SDL Error: " << SDL_GetError() << "\n";
		}
		SDL_FreeSurface(localSurf1);
	}
	SDL_Surface* onlineSurf1 = IMG_Load("../img/onlineUnPressed.png");
	if(onlineSurf1 == NULL) {
		std::cout << "Unable to load Up motion sprite! SDL_Image Error: " << IMG_GetError() << "\n";
	}
	else {
		onlineButton1 = SDL_CreateTextureFromSurface(window->getRenderer(), onlineSurf1);
		if(onlineButton1 == NULL) {
			std::cout << "Unable to create texture from sprite! SDL Error: " << SDL_GetError() << "\n";
		}
		SDL_FreeSurface(onlineSurf1);
	}
	SDL_Surface* localSurf2 = IMG_Load("../img/localHover.png");
	if(localSurf2 == NULL) {
		std::cout << "Unable to load Up motion sprite! SDL_Image Error: " << IMG_GetError() << "\n";
	}
	else {
		localButton2 = SDL_CreateTextureFromSurface(window->getRenderer(), localSurf2);
		if(localButton2 == NULL) {
			std::cout << "Unable to create texture from sprite! SDL Error: " << SDL_GetError() << "\n";
		}
		SDL_FreeSurface(localSurf2);
	}
	SDL_Surface* onlineSurf2 = IMG_Load("../img/onlineHover.png");
	if(onlineSurf2 == NULL) {
		std::cout << "Unable to load Up motion sprite! SDL_Image Error: " << IMG_GetError() << "\n";
	}
	else {
		onlineButton2 = SDL_CreateTextureFromSurface(window->getRenderer(), onlineSurf2);
		if(onlineButton2 == NULL) {
			std::cout << "Unable to create texture from sprite! SDL Error: " << SDL_GetError() << "\n";
		}
		SDL_FreeSurface(onlineSurf2);
	}
	SDL_Surface* backSurf1 = IMG_Load("../img/backUnPressed.png");
	if(backSurf1 == NULL) {
		std::cout << "Unable to load Up motion sprite! SDL_Image Error: " << IMG_GetError() << "\n";
	}
	else {
		backButton1 = SDL_CreateTextureFromSurface(window->getRenderer(), backSurf1);
		if(backButton1 == NULL) {
			std::cout << "Unable to create texture from sprite! SDL Error: " << SDL_GetError() << "\n";
		}
		SDL_FreeSurface(backSurf1);
	}
	SDL_Surface* backSurf2 = IMG_Load("../img/backHover.png");
	if(backSurf2 == NULL) {
		std::cout << "Unable to load Up motion sprite! SDL_Image Error: " << IMG_GetError() << "\n";
	}
	else {
		backButton2 = SDL_CreateTextureFromSurface(window->getRenderer(), backSurf2);
		if(backButton2 == NULL) {
			std::cout << "Unable to create texture from sprite! SDL Error: " << SDL_GetError() << "\n";
		}
		SDL_FreeSurface(backSurf2);
	}
}

void Menu::render(Window* window) {
	SDL_Rect bgRect = {0, 0, 1405, 1025};
	if(isAtMenuStart) {
		SDL_Rect buttonRect = {593, 830, 221, 71};
		window->renderTexture(background, NULL, &bgRect);
		if(button1State == HOVER || button1State == PRESSED) {
			window->renderTexture(newGameButton2, NULL, &buttonRect);
		}
		else {
			window->renderTexture(newGameButton1, NULL, &buttonRect);
		}
	}
	else if(isAtMenuMode) {
		SDL_Rect localButton = {515, 830, 130, 90};
		SDL_Rect onlineButton = {750, 830, 130, 90};
		SDL_Rect backButton = {50, 940, 105, 60};
		window->renderTexture(background, NULL, &bgRect);
		if(button1State == HOVER || button1State == PRESSED) {
			window->renderTexture(localButton2, NULL, &localButton);
		}
		else {
			window->renderTexture(localButton1, NULL, &localButton);
		}
		if(button2State == HOVER || button2State == PRESSED) {
			window->renderTexture(onlineButton2, NULL, &onlineButton);
		}
		else {
			window->renderTexture(onlineButton1, NULL, &onlineButton);
		}
		if(button3State == HOVER || button3State == PRESSED) {
			window->renderTexture(backButton2, NULL, &backButton);
		}
		else {
			window->renderTexture(backButton1, NULL, &backButton);
		}
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
					button1State = HOVER;
				}
				else if(e.type == SDL_MOUSEBUTTONDOWN) {
					button1State = PRESSED;
				}
				else if(e.type == SDL_MOUSEBUTTONUP) {
					if(button1State == PRESSED) {
						button1State = IDLE;
						isAtMenuStart = false;
						isAtMenuMode = true;
					}
					else {
						button1State = HOVER;
					}
				}
			}
			else {
				button1State = IDLE;
			}
		}
		else {
			button1State = IDLE;
		}
	}
	else if(isAtMenuMode) {
		if(e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
			int x, y;
			SDL_GetMouseState(&x, &y);
			if(x >= 515 && x <= 645 && y >= 830 && y <= 920) {
				if(e.type == SDL_MOUSEMOTION) {
					button1State = HOVER;
				}
				else if(e.type == SDL_MOUSEBUTTONDOWN) {
					button1State = PRESSED;
				}
				else if(e.type == SDL_MOUSEBUTTONUP) {
					if(button1State == PRESSED) {
						button1State = IDLE;
						isRunning = true;
						isAtMenuStart = false;
						isAtMenuMode = false;
					}
					else {
						button1State = HOVER;
					}
				}
			}
			else {
				button1State = IDLE;
			}
			if(x >= 750 && x <= 880 && y >= 830 && y <= 920) {
				if(e.type == SDL_MOUSEMOTION) {
					button2State = HOVER;
				}
				else if(e.type == SDL_MOUSEBUTTONDOWN) {
					button2State = PRESSED;
				}
				else if(e.type == SDL_MOUSEBUTTONUP) {
					if(button2State == PRESSED) {
						button2State = IDLE;
						isRunning = true;
						isAtMenuStart = false;
						isAtMenuMode = false;
					}
					else {
						button2State = HOVER;
					}
				}
			}
			else {
				button2State = IDLE;
			}
			if(x >= 50 && x <= 155 && y >= 940 && y <= 1000) {
				if(e.type == SDL_MOUSEMOTION) {
					button3State = HOVER;
				}
				else if(e.type == SDL_MOUSEBUTTONDOWN) {
					button3State = PRESSED;
				}
				else if(e.type == SDL_MOUSEBUTTONUP) {
					if(button3State == PRESSED) {
						button3State = IDLE;
						isAtMenuStart = true;
						isAtMenuMode = false;
					}
					else {
						button3State = HOVER;
					}
				}
			}
			else {
				button3State = IDLE;
			}
		}
		else {
			button1State = IDLE;
			button2State = IDLE;
			button3State = IDLE;
		}
	}
	else if(isAtEnd) {

	}
}