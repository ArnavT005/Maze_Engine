#pragma once

#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "maze.hpp"
#include "window.hpp"
#include "texture.hpp"


const int PACMAN_WIDTH = 45;
const int PACMAN_HEIGHT = 45;
const int PACMAN_VEL = 5;    	 		// 5 pixel per frame

const int ANIMATION_FRAMES = 4;


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
    void checkAlignment();
    void boundingAlignedAll();
    void boundingAlignedRow();
    void boundingAlignedCol(int i);
    void handleEvent(SDL_Event &e);		// handle dynamics
    void move();						
    void render(Window* window);        // render PACMAN
    bool collisionDetectorRect(SDL_Rect* rect1, SDL_Rect* rect2);

    bool rowAligned, colAligned;        // To check if pacman is row/column aligned
    Maze* maze;                         // Maze
    std::vector<SDL_Rect> boundingRect; // bounding rectangles
    int screenX, screenY;				// screen coordinates of pacman
    int velX, velY;						// horizontal and vertical velocities in pixels per frame
    SDL_Rect colliderBox;				// rectangular collision detection
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
	colliderBox.w = PACMAN_WIDTH;
	colliderBox.h = PACMAN_HEIGHT;
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
	boundingRect.clear();
	colliderBox.x = screenX;
	colliderBox.y = screenY;
	colliderBox.w = PACMAN_WIDTH;
	colliderBox.h = PACMAN_HEIGHT;
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


void Pacman::checkAlignment() {
	
	int blockSize = maze->blockSize,
		dotSize = maze->dotSize,
		padding = maze->padding,
		offset = dotSize + blockSize,
		residue = (padding - blockSize + 100 * offset) % offset; // positive quantity
	if(screenX % offset == residue) {
		colAligned = true;
	}
	else {
		colAligned = false;
	}
	if(screenY % offset == residue) {
		rowAligned = true;
	}
	else {
		rowAligned = false;
	}
}

void Pacman::boundingAlignedAll() {
	int blockSize = maze->blockSize,
		dotSize = maze->dotSize,
		padding = maze->padding,
		offset = dotSize + blockSize;
	boundingRect.clear();
	std::vector<SDL_Rect> rectangles = { {screenX, screenY - dotSize, blockSize, dotSize},
										 {screenX + blockSize, screenY, dotSize, blockSize},
										 {screenX, screenY + blockSize, blockSize, dotSize},
										 {screenX - dotSize, screenY, dotSize, blockSize}};	
	SDL_Point point = maze->screenToBlockCoordinate(screenX, screenY);
	if(maze->maze[point.x][point.y].up == ALL_DENIED || maze->maze[point.x][point.y].up == PACMAN_DENIED) {
		boundingRect.push_back(rectangles[0]);
	}
	if(maze->maze[point.x][point.y].right == ALL_DENIED || maze->maze[point.x][point.y].right == PACMAN_DENIED) {
		boundingRect.push_back(rectangles[1]);
	}
	if(maze->maze[point.x][point.y].down == ALL_DENIED || maze->maze[point.x][point.y].down == PACMAN_DENIED) {
		boundingRect.push_back(rectangles[2]);
	}
	if(maze->maze[point.x][point.y].left == ALL_DENIED || maze->maze[point.x][point.y].left == PACMAN_DENIED) {
		boundingRect.push_back(rectangles[3]);
	}
}

void Pacman::boundingAlignedRow() {
	int blockSize = maze->blockSize,
		dotSize = maze->dotSize,
		padding = maze->padding,
		offset = dotSize + blockSize;
	boundingRect.clear();
	bool dot1 = false, dot2 = false;

	SDL_Point point = maze->screenToBlockCoordinate(screenX, screenY);
	SDL_Point screen = maze->getBlockScreenCoordinate(point.x, point.y);

	std::vector<SDL_Rect> rectangles = { {screen.x, screen.y - dotSize, blockSize, dotSize},
										 {screen.x + blockSize, screen.y - dotSize, dotSize, dotSize},
										 {screen.x + offset, screen.y - dotSize, blockSize, dotSize},
										 {screen.x, screen.y + blockSize, blockSize, dotSize},
										 {screen.x + blockSize, screen.y + blockSize, dotSize, dotSize},
										 {screen.x + offset, screen.y + blockSize, blockSize, dotSize} };	
	if(maze->maze[point.x][point.y].up == ALL_DENIED || maze->maze[point.x][point.y].up == PACMAN_DENIED) {
		boundingRect.push_back(rectangles[0]);
		boundingRect.push_back(rectangles[1]);
		dot1 = true;
	}
	if(maze->maze[point.x][point.y + 1].up == ALL_DENIED || maze->maze[point.x][point.y + 1].up == PACMAN_DENIED) {
		boundingRect.push_back(rectangles[2]);
		if(!dot1) {
			boundingRect.push_back(rectangles[1]);
			dot1 = true;
		}
	}
	if(maze->maze[point.x][point.y].down == ALL_DENIED || maze->maze[point.x][point.y].down == PACMAN_DENIED) {
		boundingRect.push_back(rectangles[3]);
		boundingRect.push_back(rectangles[4]);
		dot2 = true;
	}
	if(maze->maze[point.x][point.y + 1].down == ALL_DENIED || maze->maze[point.x][point.y + 1].down == PACMAN_DENIED) {
		boundingRect.push_back(rectangles[5]);
		if(!dot2) {
			boundingRect.push_back(rectangles[4]);
			dot2 = true;
		}
	}
	if(!dot1) {
		if(maze->maze[point.x - 1][point.y].right == ALL_DENIED || maze->maze[point.x - 1][point.y].right == PACMAN_DENIED) {
			boundingRect.push_back(rectangles[1]);
			dot1 = true;	
		}
	}
	if(!dot2) {
		if(maze->maze[point.x + 1][point.y].right == ALL_DENIED || maze->maze[point.x + 1][point.y].right == PACMAN_DENIED) {
			boundingRect.push_back(rectangles[4]);
			dot2 = true;	
		}
	}
}

void Pacman::boundingAlignedCol(int i = 0) {
	int blockSize = maze->blockSize,
		dotSize = maze->dotSize,
		padding = maze->padding,
		offset = dotSize + blockSize;
	boundingRect.clear();
	bool dot1 = false, dot2 = false;
	SDL_Point point = maze->screenToBlockCoordinate(screenX, screenY);
	//if(i != 0) std::cout << point.x << " " << point.y << "\n";
	SDL_Point screen = maze->getBlockScreenCoordinate(point.x, point.y);
	std::vector<SDL_Rect> rectangles = { {screen.x + blockSize, screen.y, dotSize, blockSize},
										 {screen.x + blockSize, screen.y + blockSize, dotSize, dotSize},
										 {screen.x + blockSize, screen.y + offset, dotSize, blockSize},
										 {screen.x - dotSize, screen.y + offset, dotSize, blockSize},
										 {screen.x - dotSize, screen.y + blockSize, dotSize, dotSize},
										 {screen.x - dotSize, screen.y, dotSize, blockSize} };	
	if(maze->maze[point.x][point.y].right == ALL_DENIED || maze->maze[point.x][point.y].right == PACMAN_DENIED) {
		boundingRect.push_back(rectangles[0]);
		boundingRect.push_back(rectangles[1]);
		dot1 = true;
	}
	if(maze->maze[point.x + 1][point.y].right == ALL_DENIED || maze->maze[point.x + 1][point.y].right == PACMAN_DENIED) {
		boundingRect.push_back(rectangles[2]);
		if(!dot1) {
			boundingRect.push_back(rectangles[1]);
			dot1 = true;
		}
	}
	if(maze->maze[point.x][point.y].left == ALL_DENIED || maze->maze[point.x][point.y].left == PACMAN_DENIED) {
		boundingRect.push_back(rectangles[3]);
		boundingRect.push_back(rectangles[4]);
		dot2 = true;
	}
	if(maze->maze[point.x + 1][point.y].left == ALL_DENIED || maze->maze[point.x + 1][point.y].left == PACMAN_DENIED) {
		boundingRect.push_back(rectangles[5]);
		if(!dot2) {
			boundingRect.push_back(rectangles[4]);
			dot2 = true;
		}
	}
	if(!dot1) {
		if(maze->maze[point.x][point.y + 1].down == ALL_DENIED || maze->maze[point.x][point.y + 1].down == PACMAN_DENIED) {
			boundingRect.push_back(rectangles[1]);
			dot1 = true;	
		}
	}
	if(!dot2) {
		if(maze->maze[point.x][point.y - 1].down == ALL_DENIED || maze->maze[point.x][point.y - 1].down == PACMAN_DENIED) {
			boundingRect.push_back(rectangles[4]);
			dot2 = true;	
		}
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
	checkAlignment();
	if(rowAligned && colAligned) {
		boundingAlignedAll();
	}
	else if(rowAligned) {
		boundingAlignedRow();
	}
	else if(colAligned) {
		boundingAlignedCol(1);
	}
	else {
		boundingRect.clear();
	}
	SDL_Rect temp;
	bool collision = false;
	int size = boundingRect.size();
	if(velX != 0) {
		screenX += velX;
		state = velX > 0 ? MOVE_RIGHT : MOVE_LEFT;
		colliderBox.x = screenX;
		colliderBox.y = screenY;
		for(int i = 0; i < size; i ++) {
			if(collisionDetectorRect(&colliderBox, &boundingRect[i])) {
				screenX -= velX;
				colliderBox.x = screenX;
				colliderBox.y = screenY;
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
		for(int i = 0; i < size; i ++) {
			if(collisionDetectorRect(&colliderBox, &boundingRect[i])) {
				screenY -= velY;
				colliderBox.x = screenX;
				colliderBox.y = screenY;
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