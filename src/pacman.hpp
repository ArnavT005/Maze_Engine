#pragma once

#include <vector>
#include "maze.hpp"
#include "window.hpp"



class Pacman {

    public:

    static const int PACMAN_WIDTH = 45;
    static const int PACMAN_HEIGHT = 45;
    static const int PACMAN_VEL = 5;     // 5 pixel per frame

    Pacman();
    Pacman(Maze* maze);
    void checkAlignment();
    void boundingAlignedAll();
    void boundingAlignedRow();
    void boundingAlignedCol();
    void handleEvent(SDL_Event &e);		// handle dynamics
    void move();						
    void render(Window* window);                      // render PACMAN

    bool rowAligned, colAligned;        // To check if pacman is row/column aligned
    Maze* maze;                         // Maze
    std::vector<SDL_Rect> boundingRect; // bounding rectangles
    int screenX, screenY;				// screen coordinates of pacman
    int velX, velY;						// horizontal and vertical velocities in pixels per frame
    SDL_Rect colliderBox;
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
}

Pacman::Pacman(Maze* maze) {
	
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
	colliderBox.w = blockSize;
	colliderBox.h = blockSize;
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
	SDL_Point screen = maze->getBlockScreenCoordinate(point.x + 1, point.y + 1);
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

void Pacman::boundingAlignedCol() {
	int blockSize = maze->blockSize,
		dotSize = maze->dotSize,
		padding = maze->padding,
		offset = dotSize + blockSize;
	boundingRect.clear();
	bool dot1 = false, dot2 = false;
	SDL_Point point = maze->screenToBlockCoordinate(screenX, screenY);
	SDL_Point screen = maze->getBlockScreenCoordinate(point.x + 1, point.y + 1);
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
			case SDLK_UP: velY -= PACMAN_VEL; break;
			case SDLK_DOWN: velY += PACMAN_VEL; break;
			case SDLK_RIGHT: velX += PACMAN_VEL; break;
			case SDLK_LEFT: velX -= PACMAN_VEL; break;
			default: break;
		}
	}
	else if(e.type == SDL_KEYUP && e.key.repeat == 0) {
		switch(e.key.keysym.sym) {
			case SDLK_UP: velY += PACMAN_VEL; break;
			case SDLK_DOWN: velY -= PACMAN_VEL; break;
			case SDLK_RIGHT: velX -= PACMAN_VEL; break;
			case SDLK_LEFT: velX += PACMAN_VEL; break;
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
		boundingAlignedCol();
	}
	else {
		boundingRect.clear();
	}
	screenX += velX;
	screenY += velY;
	colliderBox.x = screenX;
	colliderBox.y = screenY;
	SDL_Rect temp;
	bool collision = false;
	int size = boundingRect.size();
	for(int i = 0; i < size; i ++) {
		if(SDL_IntersectRect(&colliderBox, &boundingRect[i], &temp) == SDL_TRUE) {
			screenX -= velX;
			screenY -= velY;
			colliderBox.x = screenX;
			colliderBox.y = screenY;
			collision = true;
			break;
		}
	}
}

void Pacman::render(Window* window) {
	SDL_Color color = {0x00, 0x00, 0xFF, 0xFF};
	window->renderRect(&colliderBox, color);
}