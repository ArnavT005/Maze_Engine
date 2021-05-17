#include "eatable.hpp"

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

void Eatable::free() {
	if(texture != NULL) {
		SDL_DestroyTexture(texture);
		texture = NULL;
	}
}

void Eatable::render(Window* window) {
	if(!isEaten) {
		window->renderTexture(texture, NULL, &location);
	}
}