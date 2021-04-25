#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
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
	void setPacman(Pacman* p1);
	void loadTexture(Window* window);
	void render(Window* window);
	void checkIfEaten();

	SDL_Texture* texture;
	SDL_Rect location;
	int type;
	bool isEaten;
	Pacman* p1;

	bool success;
};


Eatable::Eatable() {
	
	texture = NULL;
	
	location.x = 0;
	location.y = 0;
	location.w = 0;
	location.h = 0;

	type = NONE;
	isEaten = false;
	success = true;
}

Eatable::Eatable(int x, int y, int w, int h, int type, Window* window) {

	texture = NULL;

	location.x = x;
	location.y = y;
	location.w = w;
	location.h = h;

	this->type = type;
	isEaten = false;
	success = true;

	loadTexture(window);
}

void Eatable::setValues(int x, int y, int w, int h, int type, Window* window) {

	texture = NULL;

	location.x = x;
	location.y = y;
	location.w = w;
	location.h = h;

	this->type = type;
	isEaten = false;

	success = true;

	loadTexture(window);
}

void Eatable::setPacman(Pacman* p1) {
	this->p1 = p1;
}

void Eatable::loadTexture(Window* window) {
	if(type == TEN_POINT) {
		SDL_Surface* surf1 = IMG_Load("../img/Eatables/10Points.png");
		if(surf1 == NULL) {
			std::cout << "Unable to load image for small eatable! SDL_Image Error: " << IMG_GetError() << "\n";
			success = false;
		}
		else {
			texture = SDL_CreateTextureFromSurface(window->getRenderer(), surf1);
			if(texture == NULL) {
				std::cout << "Unable to create texture from surface! SDL Error: " << SDL_GetError() << "\n";
				success = false;
			}
		}
		SDL_FreeSurface(surf1);
	}
	else if(type == FIFTY_POINT) {
		SDL_Surface* surf2 = IMG_Load("../img/Eatables/50Points.png");
		if(surf2 == NULL) {
			std::cout << "Unable to load image for small eatable! SDL_Image Error: " << IMG_GetError() << "\n";
			success = false;
		}
		else {
			texture = SDL_CreateTextureFromSurface(window->getRenderer(), surf2);
			if(texture == NULL) {
				std::cout << "Unable to create texture from surface! SDL Error: " << SDL_GetError() << "\n";
				success = false;
			}
		}
		SDL_FreeSurface(surf2);
	}	
}

void Eatable::render(Window* window) {
	if(!isEaten) {
		window->renderTexture(texture, NULL, &location);
	}
}

void Eatable::checkIfEaten() {

	bool ifCollision1 = false, ifCollision2 = false;
	if(p1 != NULL) { 
		ifCollision1 = p1->collisionDetectorCircle(&(p1->colliderSphere), &location);
	}
	// if(p2 != NULL) {
	// 	ifCollision2 = p2->collisionDetectorCircle(&(p2->colliderSphere), &location);		
	// }
	if(ifCollision1 || ifCollision2) {
		isEaten = true;
		if(type == FIFTY_POINT) {
			p1->isBuffed = true;
		}
	}
}





