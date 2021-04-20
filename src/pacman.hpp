#pragma once

#include <vector>
#include <SDL2/SDL.h>
#include "maze.hpp"
#include "window.hpp"
#include "texture.hpp"



Texture PACMAN_SPRITE[9];
// PACMAN_SPRITE[0].loadImgTexture("../img/pacman/still.png");
// PACMAN_SPRITE[1].loadImgTexture("../img/pacman/upHalf.png");
// PACMAN_SPRITE[2].loadImgTexture("../img/pacman/upFull.png");
// PACMAN_SPRITE[3].loadImgTexture("../img/pacman/rightHalf.png");
// PACMAN_SPRITE[4].loadImgTexture("../img/pacman/rightFull.png");
// PACMAN_SPRITE[5].loadImgTexture("../img/pacman/downHalf.png");
// PACMAN_SPRITE[6].loadImgTexture("../img/pacman/downFull.png");
// PACMAN_SPRITE[7].loadImgTexture("../img/pacman/leftHalf.png");
// PACMAN_SPRITE[8].loadImgTexture("../img/pacman/leftFull.png");


const int PACMAN_WIDTH = 35;
const int PACMAN_HEIGHT = 35;
const int PACMAN_VEL = 5;     // 5 pixel per frame

enum SIDE {
	STILL,
	SIDE_UP,
	SIDE_RIGHT,
	SIDE_DOWN,
	SIDE_LEFT
};

void loadSprite(SDL_Renderer* renderer) {
	if(!PACMAN_SPRITE[0].loadImgTexture("../img/pacman/still.png", renderer)) std::cout << "Hello\n";
	PACMAN_SPRITE[1].loadImgTexture("../img/pacman/upHalf.png", renderer);
	PACMAN_SPRITE[2].loadImgTexture("../img/pacman/upFull.png", renderer);
	PACMAN_SPRITE[3].loadImgTexture("../img/pacman/rightHalf.png", renderer);
	PACMAN_SPRITE[4].loadImgTexture("../img/pacman/rightFull.png", renderer);
	PACMAN_SPRITE[5].loadImgTexture("../img/pacman/downHalf.png", renderer);
	PACMAN_SPRITE[6].loadImgTexture("../img/pacman/downFull.png", renderer);
	PACMAN_SPRITE[7].loadImgTexture("../img/pacman/leftHalf.png", renderer);
	PACMAN_SPRITE[8].loadImgTexture("../img/pacman/leftFull.png", renderer);
}

class Pacman {

    public:

    Pacman();
    Pacman(Maze* maze, Window* window);
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
    int side;
    int frameCount;
    int spriteIndex;
    std::vector<Texture> up;
    std::vector<Texture> right;
    std::vector<Texture> down;
    std::vector<Texture> left;

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
	side = STILL;
	frameCount = 0;
	spriteIndex = 0;
	up.clear();
	right.clear();
	down.clear();
	left.clear();
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
	frameCount = 0;
	spriteIndex = 0;
	side = STILL;
	up.clear();
	right.clear();
	down.clear();
	left.clear();
	loadSprite(window->getRenderer());
	up.push_back(PACMAN_SPRITE[0]); up.push_back(PACMAN_SPRITE[1]); 
	up.push_back(PACMAN_SPRITE[2]); up.push_back(PACMAN_SPRITE[1]);
	right.push_back(PACMAN_SPRITE[0]); right.push_back(PACMAN_SPRITE[3]); 
	right.push_back(PACMAN_SPRITE[4]); right.push_back(PACMAN_SPRITE[3]);
	down.push_back(PACMAN_SPRITE[0]); down.push_back(PACMAN_SPRITE[5]); 
	down.push_back(PACMAN_SPRITE[6]); down.push_back(PACMAN_SPRITE[5]);
	left.push_back(PACMAN_SPRITE[0]); left.push_back(PACMAN_SPRITE[7]); 
	left.push_back(PACMAN_SPRITE[8]); left.push_back(PACMAN_SPRITE[7]);
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
		boundingAlignedCol(1);
	}
	else {
		boundingRect.clear();
	}

	colliderBox.x = screenX;
	colliderBox.y = screenY;
	SDL_Rect temp;
	bool collision = false;
	int size = boundingRect.size();
	for(int i = 0; i < size; i ++) {
		if(collisionDetectorRect(&colliderBox, &boundingRect[i])) {
			screenX -= velX;
			screenY -= velY;
			colliderBox.x = screenX;
			colliderBox.y = screenY;
			collision = true;
			break;
		}
	}
	if(velX != 0 && !collision) {
		screenX += velX;
		side = velX > 0 ? SIDE_RIGHT : SIDE_LEFT;
	}
	else if(velY != 0 && !collision) {
		screenY += velY;
		side = velY > 0 ? SIDE_DOWN : SIDE_UP;
	}
	else {
		side = STILL;
		frameCount = 0;
		spriteIndex = 0;
	}
}

void Pacman::render(Window* window) {
	SDL_Color color = {0x00, 0x00, 0xFF, 0xFF};
	// window->renderRect(&colliderBox, color);
	if(side != STILL) {
		switch(side) {
			case SIDE_UP:
				if(frameCount == 4) {
					spriteIndex ++;
					if(spriteIndex == 5) {
						spriteIndex = 0;
					}
					frameCount = 0;
				}
				window->renderTexture(up[spriteIndex].texture, NULL, &colliderBox);
				frameCount ++; break;
			case SIDE_RIGHT:
				if(frameCount == 4) {
					spriteIndex ++;
					if(spriteIndex == 5) {
						spriteIndex = 0;
					}
					frameCount = 0;
				}
				window->renderTexture(right[spriteIndex].texture, NULL, &colliderBox);
				frameCount ++; break;
			case SIDE_DOWN:
				if(frameCount == 4) {
					spriteIndex ++;
					if(spriteIndex == 5) {
						spriteIndex = 0;
					}
					frameCount = 0;
				}
				window->renderTexture(down[spriteIndex].texture, NULL, &colliderBox);
				frameCount ++; break;
			case SIDE_LEFT:
				if(frameCount == 4) {
					spriteIndex ++;
					if(spriteIndex == 5) {
						spriteIndex = 0;
					}
					frameCount = 0;
				}
				window->renderTexture(left[spriteIndex].texture, NULL, &colliderBox);
				frameCount ++; break;			
		}	
		
	}
	else {
		frameCount = 0;
		spriteIndex = 0;
		window->renderTexture(PACMAN_SPRITE[0].texture, NULL, &colliderBox);
		window->renderRect(&colliderBox, color);
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