#pragma once

#include <vector>
#include "maze.hpp"
#include "window.hpp"



class Ghost {

    public:

    static const int PACMAN_WIDTH = 45;
    static const int PACMAN_HEIGHT = 45;
    static const int PACMAN_VEL = 3;     // 3 pixel per frame
	static const int UP = 1; 
	static const int DOWN = 2; 
	static const int LEFT = 3; 
	static const int RIGHT = 4; 

    Ghost();
    Ghost(Maze* maze, int j);
    void checkAlignment();
    void boundingAlignedAll();
    void boundingAlignedRow();
    void boundingAlignedCol(int i);
    void handleEvent(SDL_Point P, int vx, int vy);		// handle dynamics
    void move();						
    void render(Window* window);        // render PACMAN
    bool collisionDetectorRect(SDL_Rect* rect1, SDL_Rect* rect2);

    bool rowAligned, colAligned;        // To check if pacman is row/column aligned
    Maze* maze;                         // Maze
    std::vector<SDL_Rect> boundingRect; // bounding rectangles
    int screenX, screenY;				// screen coordinates of pacman
    int velX, velY;						// horizontal and vertical velocities in pixels per frame
    SDL_Rect colliderBox;
    int moveTo();
    int type;
    SDL_Point pacLoc;
    int pacVelX;
    int pacVelY;
};



Ghost::Ghost() {
	rowAligned = colAligned = false;
	screenX = screenY = 0;
	velX = velY = 0;
	maze = NULL;
	boundingRect.clear();
	colliderBox.x = 0;
	colliderBox.y = 0;
	colliderBox.w = PACMAN_WIDTH;
	colliderBox.h = PACMAN_HEIGHT;
	type = 1;
}

Ghost::Ghost(Maze* maze, int ghostType) {
	
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
	type = ghostType;
}


void Ghost::checkAlignment() {
	
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

void Ghost::boundingAlignedAll() {
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

void Ghost::boundingAlignedRow() {
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

void Ghost::boundingAlignedCol(int i = 0) {
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

int Ghost::moveTo(){
	return RIGHT;
	if(type == 1){}
	else if(type == 2){}
	else if(type == 3){}
	else if(type == 4){}
	else{}
}

void Ghost::handleEvent(SDL_Point p, int vx, int vy) {
	pacLoc = p;
	pacVelX = vx;
	pacVelY = vy;
	int direction = moveTo();
	switch(direction) {
		case UP: velY = -PACMAN_VEL; break;
		case DOWN: velY = PACMAN_VEL; break;
		case RIGHT: velX = PACMAN_VEL; break;
		case LEFT: velX = -PACMAN_VEL; break;
		default: break;
	}
}

void Ghost::move() {
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
	screenX += velX;
	screenY += velY;
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
}

void Ghost::render(Window* window) {
	SDL_Color color = {0xFF, 0xA5, 0x00, 0xFF};
	window->renderRect(&colliderBox, color);
}

bool Ghost::collisionDetectorRect(SDL_Rect* rect1, SDL_Rect* rect2) {
	int top1 = rect1->y, top2 = rect2->y, bottom1 = rect1->y + rect1->h, bottom2 = rect2->y + rect2->h,
		right1 = rect1->x + rect1->w, right2 = rect2->x + rect2->w, left1 = rect1->x, left2 = rect2->x;

	if(bottom1 <= top2 || top1 >= bottom2 || left1 >= right2 || left2 >= right1) {
		return false;
	}	
	else {
		return true;
	}
}