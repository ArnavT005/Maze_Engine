#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
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
};


void Portal::free() {
	if(opening != NULL) {
		SDL_DestroyTexture(opening);
		opening = NULL;
	}
	if(closing != NULL) {
		SDL_DestroyTexture(closing);
		closing = NULL;
	}
	if(active != NULL) {
		SDL_DestroyTexture(active);
		active = NULL;
	}

}

Portal::Portal() {
	screenX = screenY = 0;
	isOpening = true;
	isActive = false;
	isClosing = false;
	frameCount = 0;
	opening = NULL;
	active = NULL;
	closing = NULL;
	start = 0;
}

Portal::Portal(int screenX, int screenY, Window* window) {
	this->screenX = screenX;
	this->screenY = screenY;
	isOpening = true;
	isActive = false;
	isClosing = false;
	frameCount = 0;
	opening = NULL;
	active = NULL;
	closing = NULL;
	start = 0;

	loadTexture(window);
}

void Portal::loadTexture(Window* window) {

	SDL_Surface* open = IMG_Load("../img/Portal/open.png");
	if(open == NULL) {
		std::cout << "Unable to load opening image! SDL_Image Error: " << IMG_GetError() << "\n";
	}
	else {
		opening = SDL_CreateTextureFromSurface(window->getRenderer(), open);
		if(opening == NULL) {
			std::cout << "Unable to create texture from surface! SDL_Image Error: " << IMG_GetError() << "\n";
		}
		SDL_FreeSurface(open);
	}
	SDL_Surface* act = IMG_Load("../img/Portal/active.png");
	if(act == NULL) {
		std::cout << "Unable to load opening image! SDL_Image Error: " << IMG_GetError() << "\n";
	}
	else {
		active = SDL_CreateTextureFromSurface(window->getRenderer(), act);
		if(active == NULL) {
			std::cout << "Unable to create texture from surface! SDL_Image Error: " << IMG_GetError() << "\n";
		}
		SDL_FreeSurface(act);
	}
	SDL_Surface* close = IMG_Load("../img/Portal/close.png");
	if(close == NULL) {
		std::cout << "Unable to load opening image! SDL_Image Error: " << IMG_GetError() << "\n";
	}
	else {
		closing = SDL_CreateTextureFromSurface(window->getRenderer(), close);
		if(closing == NULL) {
			std::cout << "Unable to create texture from surface! SDL_Image Error: " << IMG_GetError() << "\n";
		}
		SDL_FreeSurface(close);
	}
}

void Portal::render(Window* window) {
	SDL_Rect screenLocation = {screenX, screenY, 45, 45};
	SDL_Rect src = {(frameCount / 4) * 45, 0, 45, 45};
	if(isOpening) {
		src.x = (frameCount / 8) * 45;
		window->renderTexture(opening, &src, &screenLocation);
		frameCount ++;
		if(frameCount == 32) {
			frameCount = 0;
			isOpening = false;
			isActive = true;
			isClosing = false;
		}
	}
	else if(isActive) {
		window->renderTexture(active, &src, &screenLocation);
		frameCount ++;
		if(frameCount == 40) {
			frameCount = 0;
			isClosing = false;
			isActive = true;
			isIdle = false;
		}
	}
	else if(isClosing) {
		src.x = (frameCount / 8) * 45; 
		window->renderTexture(closing, &src, &screenLocation);
		frameCount ++;
		if(frameCount == 32) {
			frameCount = 0;
			isIdle = true;
			isClosing = false;
			isOpening = false;
			isActive = false;
		}
	}
}

void Portal::closePortal() {
	isClosing = true;
	isOpening = false;
	isActive = false;
	isIdle = false;
	frameCount = 0;
	start = SDL_GetTicks();
}

void Portal::updatePortal() {
	if(isIdle) {
		if(SDL_GetTicks() - start >= 5000) {
			isOpening = true;
			isIdle = false;
			isClosing = false;
			isActive = false;
			frameCount = 0;
			start = 0;
		}
	}
}