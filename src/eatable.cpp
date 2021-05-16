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

void Eatable::setPacman(Pacman* p1, Pacman* p2) {
	this->p1 = p1;
	this->p2 = p2;
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
	powerupSmall = Mix_LoadWAV( "../music/eat10.wav");
	if( powerupSmall == NULL ){
		printf( "Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}
	powerupLarge = Mix_LoadWAV( "../music/eat50.wav");
	if( powerupLarge == NULL ){
		printf( "Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}	
}

void Eatable::free() {
	if(texture != NULL) {
		SDL_DestroyTexture(texture);
		texture = NULL;
	}
	if(powerupLarge != NULL) {
		Mix_FreeChunk(powerupLarge);
		powerupLarge = NULL;
	}
	if(powerupSmall != NULL) {
		Mix_FreeChunk(powerupSmall);
		powerupSmall = NULL;
	}
}

void Eatable::render(Window* window) {
	if(!isEaten) {
		window->renderTexture(texture, NULL, &location);
	}
}

void Eatable::checkIfEaten(bool &isBuffed) {

	bool ifCollision1 = false, ifCollision2 = false;
	if(p1 != NULL) { 
		ifCollision1 = p1->collisionDetectorCircle(&(p1->colliderSphere), &location);
	}
	if(p2 != NULL) {
		ifCollision2 = p2->collisionDetectorCircle(&(p2->colliderSphere), &location);		
	}
	if(ifCollision1 || ifCollision2) {
		if(!isEaten) {
			isEaten = true;
			if(type == FIFTY_POINT) {
				Mix_PlayChannel(8, powerupLarge, 0);
				isBuffed = true;
				if(ifCollision1 && ifCollision2){
					p1->score += 25;
					p2->score += 25;
				}
				else if(ifCollision1){p1->score += 50;}
				else if(ifCollision2){p2->score += 50;}
			}
			else if(type == TEN_POINT) {
				Mix_PlayChannel(9, powerupSmall, 0);
				if(ifCollision1 && ifCollision2){
					p1->score += 5;	
					p2->score += 5;
				}
				else if(ifCollision1){p1->score += 10;}
				else if(ifCollision2){p2->score += 10;}
			}
		}
	}
}