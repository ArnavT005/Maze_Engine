#pragma once

#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "maze.hpp"
#include "window.hpp"
#include "texture.hpp"


const int BOX_WIDTH = 45;
const int BOX_HEIGHT = 45;
const int PACMAN_WIDTH = 31;
const int PACMAN_HEIGHT = 41;
const int PACMAN_VEL = 5;    	 		// 5 pixel per frame

const int ANIMATION_FRAMES = 4;


struct Circle {
	int radius;
	SDL_Point center;
};

enum STATE {
	STILL_UP,
	STILL_RIGHT,
	STILL_DOWN,
	STILL_LEFT,
	MOVE_UP,
	MOVE_RIGHT,
	MOVE_DOWN,
	MOVE_LEFT
};

class Pacman {

    public:

    Pacman();
    Pacman(Maze* maze, Window* window);
    void loadTexture(Window* window);
    void handleEvent(SDL_Event &e);		// handle dynamics
    void move();						
    void render(Window* window);        // render PACMAN
    bool collisionDetectorRect(SDL_Rect* rect1, SDL_Rect* rect2);
    bool collisionDetectorCircle(Circle* circle, SDL_Rect* rect);

    bool rowAligned, colAligned;        // To check if pacman is row/column aligned
    Maze* maze;                         // Maze
    std::vector<SDL_Rect> boundingRect; // bounding rectangles
    int screenX, screenY;				// screen coordinates of pacman
    int velX, velY;						// horizontal and vertical velocities in pixels per frame
    SDL_Rect colliderBox;				// rectangular collision detector box
    Circle colliderSphere;				// circular collision detector box	
    int state;							// motion state of pacman							
    int frameCount;						// frame count number
    SDL_Texture* up;
    SDL_Texture* right;
    SDL_Texture* down;
    SDL_Texture* left;
    bool success;					    // error reporting flag
};



Pacman::Pacman() {
	rowAligned = colAligned = false;
	screenX = screenY = 0;
	velX = velY = 0;
	maze = NULL;
	boundingRect.clear();
	colliderBox.x = 0;
	colliderBox.y = 0;
	colliderBox.w = BOX_WIDTH;
	colliderBox.h = BOX_HEIGHT;
	colliderSphere.radius = (PACMAN_WIDTH - 1) / 2;
	colliderSphere.center.x = BOX_WIDTH / 2;
	colliderSphere.center.y = BOX_HEIGHT / 2;
	state = STILL_RIGHT;
	frameCount = 0;
	success = true;
}

Pacman::Pacman(Maze* maze, Window* window) {
	
	int blockSize = maze->blockSize,
		dotSize = maze->dotSize,
		padding = maze->padding;
	
	rowAligned = colAligned = true;
	screenX = screenY = padding + dotSize;
	velX = velY = 0;
	this->maze = maze;
	boundingRect = maze->boundaryRect;
	colliderBox.x = screenX;
	colliderBox.y = screenY;
	colliderBox.w = BOX_WIDTH;
	colliderBox.h = BOX_HEIGHT;
	colliderSphere.radius = (PACMAN_WIDTH - 1) / 2;
	colliderSphere.center.x = screenX + BOX_WIDTH / 2;
	colliderSphere.center.y = screenY + BOX_HEIGHT / 2;
	state = STILL_RIGHT;
	frameCount = 0;
	success = true;
	loadTexture(window);
}

void Pacman::loadTexture(Window* window) {

	SDL_Surface* upSurf = IMG_Load("../img/pacman/upMotion.png");
	if(upSurf == NULL) {
		std::cout << "Unable to load Up motion sprite! SDL_Image Error: " << IMG_GetError() << "\n";
		success = false;
	}
	else {
		up = SDL_CreateTextureFromSurface(window->getRenderer(), upSurf);
		if(up == NULL) {
			std::cout << "Unable to create texture from sprite! SDL Error: " << SDL_GetError() << "\n";
			success = false;
		}
		SDL_FreeSurface(upSurf);
	}

	SDL_Surface* rightSurf = IMG_Load("../img/pacman/rightMotion.png");
	if(rightSurf == NULL) {
		std::cout << "Unable to load Right motion sprite! SDL_Image Error: " << IMG_GetError() << "\n";
		success = false;
	}
	else {
		right = SDL_CreateTextureFromSurface(window->getRenderer(), rightSurf);
		if(right == NULL) {
			std::cout << "Unable to create texture from sprite! SDL Error: " << SDL_GetError() << "\n";
			success = false;
		}
		SDL_FreeSurface(rightSurf);
	}

	SDL_Surface* downSurf = IMG_Load("../img/pacman/downMotion.png");
	if(downSurf == NULL) {
		std::cout << "Unable to load Down motion sprite! SDL_Image Error: " << IMG_GetError() << "\n";
		success = false;
	}
	else {
		down = SDL_CreateTextureFromSurface(window->getRenderer(), downSurf);
		if(down == NULL) {
			std::cout << "Unable to create texture from sprite! SDL Error: " << SDL_GetError() << "\n";
			success = false;
		}
		SDL_FreeSurface(downSurf);
	}

	SDL_Surface* leftSurf = IMG_Load("../img/pacman/leftMotion.png");
	if(leftSurf == NULL) {
		std::cout << "Unable to load Left motion sprite! SDL_Image Error: " << IMG_GetError() << "\n";
		success = false;
	}
	else {
		left = SDL_CreateTextureFromSurface(window->getRenderer(), leftSurf);
		if(left == NULL) {
			std::cout << "Unable to create texture from sprite! SDL Error: " << SDL_GetError() << "\n";
			success = false;
		}
		SDL_FreeSurface(leftSurf);
	}
}




void Pacman::handleEvent(SDL_Event &e) {
	if(e.type == SDL_KEYDOWN && e.key.repeat == 0) {
		switch(e.key.keysym.sym) {
			case SDLK_UP: velY -= PACMAN_VEL; state = MOVE_UP; frameCount = 0; break;
			case SDLK_DOWN: velY += PACMAN_VEL; state = MOVE_DOWN; frameCount = 0; break;
			case SDLK_RIGHT: velX += PACMAN_VEL; state = MOVE_RIGHT; frameCount = 0; break;
			case SDLK_LEFT: velX -= PACMAN_VEL; state = MOVE_LEFT; frameCount = 0; break;
			default: break;
		}
	}
	else if(e.type == SDL_KEYUP && e.key.repeat == 0) {
		switch(e.key.keysym.sym) {
			case SDLK_UP: velY += PACMAN_VEL; state = STILL_UP; frameCount = 0; break;
			case SDLK_DOWN: velY -= PACMAN_VEL; state = STILL_DOWN; frameCount = 0; break;
			case SDLK_RIGHT: velX -= PACMAN_VEL; state = STILL_RIGHT; frameCount = 0; break;
			case SDLK_LEFT: velX += PACMAN_VEL; state = STILL_LEFT; frameCount = 0; break;
			default: break;
		}
	}
}

void Pacman::move() {
	
	bool collision = false;
	int size = boundingRect.size();
	if(velX != 0) {
		screenX += velX;
		state = velX > 0 ? MOVE_RIGHT : MOVE_LEFT;
		colliderBox.x = screenX;
		colliderBox.y = screenY;
		colliderSphere.center.x += velX;
		for(int i = 0; i < size; i ++) {
			if(collisionDetectorCircle(&colliderSphere, &boundingRect[i])) {
				screenX -= velX;
				colliderBox.x = screenX;
				colliderBox.y = screenY;
				colliderSphere.center.x -= velX;
				collision = true;
				state = velX > 0 ? STILL_RIGHT : STILL_LEFT;
				break;
			}
		}	
	}
	else if(velY != 0) {
		screenY += velY;
		state = velY > 0 ? MOVE_DOWN : MOVE_UP;
		colliderBox.x = screenX;
		colliderBox.y = screenY;
		colliderSphere.center.y += velY;
		for(int i = 0; i < size; i ++) {
			if(collisionDetectorCircle(&colliderSphere, &boundingRect[i])) {
				screenY -= velY;
				colliderBox.x = screenX;
				colliderBox.y = screenY;
				colliderSphere.center.y -= velY;
				state = velY > 0 ? STILL_DOWN : STILL_UP;
				collision = true;
				break;
			}
		}
	}
}

void Pacman::render(Window* window) {
	SDL_Color color = {0x00, 0x00, 0xFF, 0xFF};
	SDL_Rect stillPosition = {0, 0, 45, 45};
	SDL_Rect movingPosition = {(frameCount / ANIMATION_FRAMES) * 45, 0, 45, 45};

	switch(state) {
		case STILL_UP: 
			window->renderTexture(up, &stillPosition, &colliderBox);
			break;
		case STILL_RIGHT: 
			window->renderTexture(right, &stillPosition, &colliderBox);
			break;
		case STILL_DOWN: 
			window->renderTexture(down, &stillPosition, &colliderBox);
			break;
		case STILL_LEFT: 
			window->renderTexture(left, &stillPosition, &colliderBox);
			break;			
		case MOVE_UP:
			window->renderTexture(up, &movingPosition, &colliderBox);
			frameCount++;
			break;
		case MOVE_RIGHT:
			window->renderTexture(right, &movingPosition, &colliderBox);
			frameCount++;
			break;
		case MOVE_DOWN:
			window->renderTexture(down, &movingPosition, &colliderBox);
			frameCount++;
			break;
		case MOVE_LEFT:
			window->renderTexture(left, &movingPosition, &colliderBox);
			frameCount++;
			break;			
	}
	if(frameCount == 16) {
		frameCount = 0;
	}

}

bool Pacman::collisionDetectorRect(SDL_Rect* rect1, SDL_Rect* rect2) {
	int top1 = rect1->y, top2 = rect2->y, bottom1 = rect1->y + rect1->h, bottom2 = rect2->y + rect2->h,
		right1 = rect1->x + rect1->w, right2 = rect2->x + rect2->w, left1 = rect1->x, left2 = rect2->x;

	if(bottom1 <= top2 || top1 >= bottom2 || left1 >= right2 || left2 >= right1) {
		return false;
	}	
	else {
		return true;
	}
}

bool Pacman::collisionDetectorCircle(Circle* circle, SDL_Rect *rect) {
	SDL_Point closestPoint;
	if(circle->center.x < rect->x) {
		closestPoint.x = rect->x;
	}
	else if(circle->center.x > rect->x + rect->w) {
		closestPoint.x = rect->x + rect->w;
	}
	else {
		closestPoint.x = circle->center.x;
	}
	if(circle->center.y < rect->y) {
		closestPoint.y = rect->y;
	}
	else if(circle->center.y > rect->y + rect->h) {
		closestPoint.y = rect->y + rect->h;
	}
	else {
		closestPoint.y = circle->center.y;
	}
	// check collision
	int distanceSq = (closestPoint.x - circle->center.x) * (closestPoint.x - circle->center.x) + 
				     (closestPoint.y - circle->center.y) * (closestPoint.y - circle->center.y);
	if(distanceSq < circle->radius * circle->radius) {
		return true;
	} 
	else {
		return false;
	}
}