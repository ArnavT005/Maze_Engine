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


	SDL_Texture* P1;
	SDL_Texture* P2;
	SDL_Texture* draw;

};


Menu::Menu() {
	
	isAtMenuStart = true;
	isAtMenuMode = false;
	isAtEnd = false;
	isRunning = false;
	isOver = false;
	onlinePossible = true;
	button1State = IDLE;
	button2State = IDLE;
	button3State = IDLE;
	winner = 0;
	mode = 1;

	background = NULL;
	gameOver = NULL;
	connecting = NULL;
	rematchButton1 = NULL;
	rematchButton2 = NULL;
	backToMenuButton1 = NULL;
	backToMenuButton2 = NULL;
	newGameButton1 = NULL;
	newGameButton2 = NULL;
	localButton1 = NULL;
	localButton2 = NULL;
	onlineButton1 = NULL;
	onlineButton2 = NULL;
	backButton1 = NULL;
	backButton2 = NULL;
	P1 = NULL;
	P2 = NULL;
	draw = NULL;
}

Menu::Menu(Window* window) {
	
	isAtMenuStart = true;
	isAtMenuMode = false;
	isAtEnd = false;
	isRunning = false;
	isOver = false;
	onlinePossible = true;
	button1State = IDLE;
	button2State = IDLE;
	button3State = IDLE;
	winner = 0;
	mode = 1;

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
	if(backToMenuButton1 != NULL) {
		SDL_DestroyTexture(backToMenuButton1);
		backToMenuButton1 = NULL;
	}
	if(backToMenuButton2 != NULL) {
		SDL_DestroyTexture(backToMenuButton2);
		backToMenuButton2 = NULL;
	}
	if(gameOver != NULL) {
		SDL_DestroyTexture(gameOver);
		gameOver = NULL;
	}
	if(connecting != NULL) {
		SDL_DestroyTexture(connecting);
		connecting = NULL;
	}
	if(rematchButton1 != NULL) {
		SDL_DestroyTexture(rematchButton1);
		rematchButton1 = NULL;
	}
	if(rematchButton2 != NULL) {
		SDL_DestroyTexture(rematchButton2);
		rematchButton2 = NULL;
	}
	if(P1 != NULL) {
		SDL_DestroyTexture(P1);
		P1 = NULL;
	}
	if(P2 != NULL) {
		SDL_DestroyTexture(P2);
		P2 = NULL;
	}
	if(draw != NULL) {
		SDL_DestroyTexture(draw);
		draw = NULL;
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
	SDL_Surface* connectSurf = IMG_Load("../img/connecting.png");
	if(connectSurf == NULL) {
		std::cout << "Unable to load Up motion sprite! SDL_Image Error: " << IMG_GetError() << "\n";
	}
	else {
		connecting = SDL_CreateTextureFromSurface(window->getRenderer(), connectSurf);
		if(connecting == NULL) {
			std::cout << "Unable to create texture from sprite! SDL Error: " << SDL_GetError() << "\n";
		}
		SDL_FreeSurface(connectSurf);
	}
	SDL_Surface* newGameSurf1 = IMG_Load("../img/Button/newGameUnPressed.png");
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
	SDL_Surface* newGameSurf2 = IMG_Load("../img/Button/newGameHover.png");
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
	SDL_Surface* localSurf1 = IMG_Load("../img/Button/localUnPressed.png");
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
	SDL_Surface* onlineSurf1 = IMG_Load("../img/Button/onlineUnPressed.png");
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
	SDL_Surface* localSurf2 = IMG_Load("../img/Button/localHover.png");
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
	SDL_Surface* onlineSurf2 = IMG_Load("../img/Button/onlineHover.png");
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
	SDL_Surface* backSurf1 = IMG_Load("../img/Button/backUnPressed.png");
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
	SDL_Surface* backSurf2 = IMG_Load("../img/Button/backHover.png");
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
	SDL_Surface* gameOverSurf = IMG_Load("../img/gameOver.png");
	if(gameOverSurf == NULL) {
		std::cout << "Unable to load Up motion sprite! SDL_Image Error: " << IMG_GetError() << "\n";
	}
	else {
		gameOver = SDL_CreateTextureFromSurface(window->getRenderer(), gameOverSurf);
		if(gameOver == NULL) {
			std::cout << "Unable to create texture from sprite! SDL Error: " << SDL_GetError() << "\n";
		}
		SDL_FreeSurface(gameOverSurf);
	}
	SDL_Surface* backMenuSurf1 = IMG_Load("../img/Button/backMenuUnPressed.png");
	if(backMenuSurf1 == NULL) {
		std::cout << "Unable to load Up motion sprite! SDL_Image Error: " << IMG_GetError() << "\n";
	}
	else {
		backToMenuButton1 = SDL_CreateTextureFromSurface(window->getRenderer(), backMenuSurf1);
		if(backToMenuButton1 == NULL) {
			std::cout << "Unable to create texture from sprite! SDL Error: " << SDL_GetError() << "\n";
		}
		SDL_FreeSurface(backMenuSurf1);
	}
	SDL_Surface* backMenuSurf2 = IMG_Load("../img/Button/backMenuHover.png");
	if(backMenuSurf2 == NULL) {
		std::cout << "Unable to load Up motion sprite! SDL_Image Error: " << IMG_GetError() << "\n";
	}
	else {
		backToMenuButton2 = SDL_CreateTextureFromSurface(window->getRenderer(), backMenuSurf2);
		if(backToMenuButton2 == NULL) {
			std::cout << "Unable to create texture from sprite! SDL Error: " << SDL_GetError() << "\n";
		}
		SDL_FreeSurface(backMenuSurf2);
	}
	SDL_Surface* rematchSurf1 = IMG_Load("../img/Button/rematchUnPressed.png");
	if(rematchSurf1 == NULL) {
		std::cout << "Unable to load Up motion sprite! SDL_Image Error: " << IMG_GetError() << "\n";
	}
	else {
		rematchButton1 = SDL_CreateTextureFromSurface(window->getRenderer(), rematchSurf1);
		if(rematchButton1 == NULL) {
			std::cout << "Unable to create texture from sprite! SDL Error: " << SDL_GetError() << "\n";
		}
		SDL_FreeSurface(rematchSurf1);
	}
	SDL_Surface* rematchSurf2 = IMG_Load("../img/Button/rematchHover.png");
	if(rematchSurf2 == NULL) {
		std::cout << "Unable to load Up motion sprite! SDL_Image Error: " << IMG_GetError() << "\n";
	}
	else {
		rematchButton2 = SDL_CreateTextureFromSurface(window->getRenderer(), rematchSurf2);
		if(rematchButton2 == NULL) {
			std::cout << "Unable to create texture from sprite! SDL Error: " << SDL_GetError() << "\n";
		}
		SDL_FreeSurface(rematchSurf2);
	}
	SDL_Surface* P1Surf = IMG_Load("../img/P1.png");
	if(P1Surf == NULL) {
		std::cout << "Unable to load Up motion sprite! SDL_Image Error: " << IMG_GetError() << "\n";
	}
	else {
		P1 = SDL_CreateTextureFromSurface(window->getRenderer(), P1Surf);
		if(P1 == NULL) {
			std::cout << "Unable to create texture from sprite! SDL Error: " << SDL_GetError() << "\n";
		}
		SDL_FreeSurface(P1Surf);
	}
	SDL_Surface* P2Surf = IMG_Load("../img/P2.png");
	if(P2Surf == NULL) {
		std::cout << "Unable to load Up motion sprite! SDL_Image Error: " << IMG_GetError() << "\n";
	}
	else {
		P2 = SDL_CreateTextureFromSurface(window->getRenderer(), P2Surf);
		if(P2 == NULL) {
			std::cout << "Unable to create texture from sprite! SDL Error: " << SDL_GetError() << "\n";
		}
		SDL_FreeSurface(P2Surf);
	}
	SDL_Surface* drawSurf = IMG_Load("../img/draw.png");
	if(drawSurf == NULL) {
		std::cout << "Unable to load Up motion sprite! SDL_Image Error: " << IMG_GetError() << "\n";
	}
	else {
		draw = SDL_CreateTextureFromSurface(window->getRenderer(), drawSurf);
		if(draw == NULL) {
			std::cout << "Unable to create texture from sprite! SDL Error: " << SDL_GetError() << "\n";
		}
		SDL_FreeSurface(drawSurf);
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
	else if(isRunning && mode == 2 && onlinePossible) {
		SDL_Rect backButton = {50, 940, 105, 60};
		window->renderTexture(connecting, NULL, &bgRect);
	}
	else if(isRunning && mode == 2 && !onlinePossible) {
		SDL_Rect backButton = {50, 940, 105, 60};
		window->renderTexture(connecting, NULL, &bgRect);
		if(button1State == HOVER || button1State == PRESSED) {
			window->renderTexture(backButton2, NULL, &backButton);
		}
		else {
			window->renderTexture(backButton1, NULL, &backButton);
		}
	}
	else if(isAtEnd) {
		SDL_Rect rematchButton = {593, 780, 200, 65};
		SDL_Rect backButton = {50, 940, 200, 65};
		SDL_Rect playerWin = {543, 580, 300, 150};
		window->renderTexture(gameOver, NULL, &bgRect);
		if(button1State == HOVER || button1State == PRESSED) {
			window->renderTexture(rematchButton2, NULL, &rematchButton);
		}
		else {
			window->renderTexture(rematchButton1, NULL, &rematchButton);
		}
		if(button2State == HOVER || button2State == PRESSED) {
			window->renderTexture(backToMenuButton2, NULL, &backButton);
		}
		else {
			window->renderTexture(backToMenuButton1, NULL, &backButton);
		}
		if(winner == 1)
			window->renderTexture(P1, NULL, &playerWin);
		else if(winner == 2)
			window->renderTexture(P2, NULL, &playerWin);
		else
			window->renderTexture(draw, NULL, &playerWin);
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
						Mix_PlayChannel(20, click, 0);
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
						Mix_PlayChannel(20, click, 0);
						button1State = IDLE;
						isRunning = true;
						mode = 1;
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
						Mix_PlayChannel(20, click, 0);
						button2State = IDLE;
						isRunning = true;
						mode = 2;
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
						Mix_PlayChannel(20, click, 0);
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
	else if(isRunning && mode == 2 && !onlinePossible) {
		if(e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
			int x, y;
			SDL_GetMouseState(&x, &y);
			if(x >= 50 && x <= 155 && y >= 940 && y <= 1000) {
				if(e.type == SDL_MOUSEMOTION) {
					button1State = HOVER;
				}
				else if(e.type == SDL_MOUSEBUTTONDOWN) {
					button1State = PRESSED;
				}
				else if(e.type == SDL_MOUSEBUTTONUP) {
					if(button1State == PRESSED) {
						Mix_PlayChannel(20, click, 0);
						button1State = IDLE;
						isRunning = false;
						isAtEnd = false;
						isOver = false;
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
	else if(isAtEnd) {
		if(e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
			int x, y;
			SDL_GetMouseState(&x, &y);
			if(x >= 593 && x <= 793 && y >= 780 && y <= 845) {
				if(e.type == SDL_MOUSEMOTION) {
					button1State = HOVER;
				}
				else if(e.type == SDL_MOUSEBUTTONDOWN) {
					button1State = PRESSED;
				}
				else if(e.type == SDL_MOUSEBUTTONUP) {
					if(button1State == PRESSED) {
						Mix_PlayChannel(20, click, 0);
						button1State = IDLE;
						isRunning = true;
						isAtEnd = false;
						isOver = false;
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
			if(x >= 50 && x <= 250 && y >= 940 && y <= 1000) {
				if(e.type == SDL_MOUSEMOTION) {
					button2State = HOVER;
				}
				else if(e.type == SDL_MOUSEBUTTONDOWN) {
					button2State = PRESSED;
				}
				else if(e.type == SDL_MOUSEBUTTONUP) {
					if(button2State == PRESSED) {
						Mix_PlayChannel(20, click, 0);
						button2State = IDLE;
						isRunning = false;
						isAtEnd = false;
						isOver = false;
						isAtMenuStart = true;
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
		}
		else {
			button1State = IDLE;
			button2State = IDLE;
		}
	}
}