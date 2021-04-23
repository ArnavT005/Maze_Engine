#pragma once

#include <vector>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include "maze.hpp"
#include "window.hpp"
#include "pacman.hpp"

using namespace std;


enum {
	INITIAL,
	UP, 
	DOWN,
	LEFT, 
	RIGHT
};

enum GHOST {
	TYPE_BLINKY, 
	TYPE_PINKY, 
	TYPE_INKY, 
	TYPE_CLYDE
};

class Ghost {

    public:

    static const int GHOST_WIDTH = 45;
    static const int GHOST_HEIGHT = 45;
    static const int ANIMATION_FRAMES = 2;

    Ghost();
    Ghost(Maze* maze, int j, int k);
    void loadTexture(Window* window);
    void checkAlignment();
    void handleEvent(Pacman* pac1);		// handle dynamics
    void move();						
    void render(Window* window);        // render GHOST
    bool collisionDetectorRect(SDL_Rect* rect1, SDL_Rect* rect2);
    int BFS(int i, int j, vector<vector<int>> &V);
    int moveTo();
    
    bool rowAligned, colAligned;        // To check if ghost is row/column aligned
    Maze* maze;                         // Maze
    std::vector<SDL_Rect> boundingRect; // bounding rectangles
    int screenX, screenY;				// screen coordinates of ghost
    int velX, velY;						// horizontal and vertical velocities in pixels per frame
    SDL_Rect colliderBox;
    int type;
    Pacman* pac1;				// pacman
    int direction;
    int destinationX;
    int destinationY;		
    int mode;					// set value default
    int state;
    int GHOST_VEL;     			// 1 pixel per frame
    bool success;				// error reporting flag
    int frameCount;
    SDL_Texture* up;
    SDL_Texture* right;
    SDL_Texture* down;
    SDL_Texture* left;			// rendering textures
    SDL_Texture* rightAngry;
    SDL_Texture* downAngry;
    SDL_Texture* leftAngry;		// rendering angry textures
    bool randomOn;

   // int random_seed;

};



Ghost::Ghost() {
	rowAligned = colAligned = false;
	screenX = screenY = 0;
	velX = velY = 0;
	maze = NULL;
	boundingRect.clear();
	colliderBox.x = screenX;
	colliderBox.y = screenY;
	colliderBox.w = GHOST_WIDTH;
	colliderBox.h = GHOST_HEIGHT;
	type = 1;
	mode = 1;
	GHOST_VEL = 1;
	destinationY = destinationX = 0;
	direction = INITIAL;
	state = STILL_UP;
	success = true;
	frameCount = 0;
	randomOn = false;
}

Ghost::Ghost(Maze* maze, int ghostType, int mode) {

    this->mode = mode;
	rowAligned = colAligned = true;
    int mid = maze->dimension/2 - 1;
    GHOST_VEL = 1;
    if(ghostType == TYPE_BLINKY) {
    	screenX = screenY = maze->getBlockScreenCoordinate(mid, mid).x;
    	destinationX = 0; destinationY = 0;
    	GHOST_VEL = 1;
    }
    else if(ghostType == TYPE_PINKY) {
    	screenX = maze->getBlockScreenCoordinate(mid, mid + 1).x; 
    	screenY = maze->getBlockScreenCoordinate(mid, mid + 1).y;
    	destinationX = maze->dimension - 1; destinationY = 0;
    	GHOST_VEL = 1;
    }
    else if(ghostType == TYPE_INKY) {
    	screenX = maze->getBlockScreenCoordinate(mid + 1, mid).x; 
    	screenY = maze->getBlockScreenCoordinate(mid + 1, mid).y;
    	destinationX = 0; destinationY = maze->dimension - 1;
    	if(mode == 2) {
  			GHOST_VEL = 5;
  		}	
    }
    else if(ghostType == TYPE_CLYDE) {
    	screenX = screenY = maze->getBlockScreenCoordinate(mid + 1, mid + 1).x;
    	destinationX = maze->dimension; destinationY = maze->dimension - 1;
    }
	velX = velY = 0;
	this->maze = maze;
	boundingRect = maze->boundaryRectGhost;
	colliderBox.x = screenX;
	colliderBox.y = screenY;
	colliderBox.w = GHOST_WIDTH;
	colliderBox.h = GHOST_HEIGHT;
	type = ghostType;
	direction = INITIAL;
	state = STILL_UP;
	success = true;
	frameCount = 0;
	randomOn = false;
}


void Ghost::loadTexture(Window* window) {
	std::string path;
	if(type == TYPE_BLINKY) {
		path = "Blinky";
	}
	else if(type == TYPE_PINKY) {
		path = "Pinky";		
	}
	else if(type == TYPE_INKY) {
		path = "Inky";		
	}
	else if(type == TYPE_CLYDE) {
		path = "Clyde";		
	}
	SDL_Surface* upSurf = IMG_Load(("../img/" + path + "/upMotion.png").c_str());
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
	SDL_Surface* rightSurf1 = IMG_Load(("../img/" + path + "/rightAngry.png").c_str());
	if(rightSurf1 == NULL) {
		std::cout << "Unable to load Right motion sprite! SDL_Image Error: " << IMG_GetError() << "\n";
		success = false;
	}
	else {
		rightAngry = SDL_CreateTextureFromSurface(window->getRenderer(), rightSurf1);
		if(rightAngry == NULL) {
			std::cout << "Unable to create texture from sprite! SDL Error: " << SDL_GetError() << "\n";
			success = false;
		}
		SDL_FreeSurface(rightSurf1);
	}

	SDL_Surface* downSurf1 = IMG_Load(("../img/" + path + "/downAngry.png").c_str());
	if(downSurf1 == NULL) {
		std::cout << "Unable to load Down motion sprite! SDL_Image Error: " << IMG_GetError() << "\n";
		success = false;
	}
	else {
		downAngry = SDL_CreateTextureFromSurface(window->getRenderer(), downSurf1);
		if(downAngry == NULL) {
			std::cout << "Unable to create texture from sprite! SDL Error: " << SDL_GetError() << "\n";
			success = false;
		}
		SDL_FreeSurface(downSurf1);
	}

	SDL_Surface* leftSurf1 = IMG_Load(("../img/" + path + "/leftAngry.png").c_str());
	if(leftSurf1 == NULL) {
		std::cout << "Unable to load Left motion sprite! SDL_Image Error: " << IMG_GetError() << "\n";
		success = false;
	}
	else {
		leftAngry = SDL_CreateTextureFromSurface(window->getRenderer(), leftSurf1);
		if(leftAngry == NULL) {
			std::cout << "Unable to create texture from sprite! SDL Error: " << SDL_GetError() << "\n";
			success = false;
		}
		SDL_FreeSurface(leftSurf1);
	}
	SDL_Surface* rightSurf2 = IMG_Load(("../img/" + path + "/rightMotion.png").c_str());
	if(rightSurf2 == NULL) {
		std::cout << "Unable to load Right motion sprite! SDL_Image Error: " << IMG_GetError() << "\n";
		success = false;
	}
	else {
		right = SDL_CreateTextureFromSurface(window->getRenderer(), rightSurf2);
		if(right == NULL) {
			std::cout << "Unable to create texture from sprite! SDL Error: " << SDL_GetError() << "\n";
			success = false;
		}
		SDL_FreeSurface(rightSurf2);
	}

	SDL_Surface* downSurf2 = IMG_Load(("../img/" + path + "/downMotion.png").c_str());
	if(downSurf2 == NULL) {
		std::cout << "Unable to load Down motion sprite! SDL_Image Error: " << IMG_GetError() << "\n";
		success = false;
	}
	else {
		down = SDL_CreateTextureFromSurface(window->getRenderer(), downSurf2);
		if(down == NULL) {
			std::cout << "Unable to create texture from sprite! SDL Error: " << SDL_GetError() << "\n";
			success = false;
		}
		SDL_FreeSurface(downSurf2);
	}

	SDL_Surface* leftSurf2 = IMG_Load(("../img/" + path + "/leftMotion.png").c_str());
	if(leftSurf2 == NULL) {
		std::cout << "Unable to load Left motion sprite! SDL_Image Error: " << IMG_GetError() << "\n";
		success = false;
	}
	else {
		left = SDL_CreateTextureFromSurface(window->getRenderer(), leftSurf2);
		if(left == NULL) {
			std::cout << "Unable to create texture from sprite! SDL Error: " << SDL_GetError() << "\n";
			success = false;
		}
		SDL_FreeSurface(leftSurf2);
	}
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

int Ghost::BFS(int destX, int destY, vector<vector<int>> &m){
    int size = maze->dimension;
    if(destX < 0 || destX >= size || destY < 0 || destY >= size){
        return 0;
    }
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            m[i][j] = -1;
        }
    }
    m[destX][destY] = 0;
    vector<vector<int>> que;
    que.push_back(vector<int> {destX, destY});
    while(!que.empty()){
        vector<int> pt = que[0];
        que.erase(que.begin());
        if(maze->maze[pt[0]][pt[1]].up != ALL_DENIED && pt[0] > 0 && m[pt[0]-1][pt[1]] == -1){
            m[pt[0]-1][pt[1]] = m[pt[0]][pt[1]] + 1;
            que.push_back(vector<int> {pt[0]-1, pt[1]});
        }
        if(maze->maze[pt[0]][pt[1]].down != ALL_DENIED && pt[0] < size - 1 && m[pt[0]+1][pt[1]] == -1){
            m[pt[0]+1][pt[1]] = m[pt[0]][pt[1]] + 1;
            que.push_back(vector<int> {pt[0]+1, pt[1]});
        }
        if(maze->maze[pt[0]][pt[1]].right != ALL_DENIED && pt[1] < size - 1 && m[pt[0]][pt[1]+1] == -1){
            m[pt[0]][pt[1]+1] = m[pt[0]][pt[1]] + 1;
            que.push_back(vector<int> {pt[0], pt[1]+1});
        }
        if(maze->maze[pt[0]][pt[1]].left != ALL_DENIED && pt[1] > 0 && m[pt[0]][pt[1]-1] == -1){
            m[pt[0]][pt[1]-1] = m[pt[0]][pt[1]] + 1;
            que.push_back(vector<int> {pt[0], pt[1]-1});
        }
    }
    int blkX = maze->screenToBlockCoordinate(screenX, screenY).x, blkY = maze->screenToBlockCoordinate(screenX, screenY).y;
    int score = m[blkX][blkY], move = 0;
    // Add randomness in case multiple directions have same distance
    if(maze->maze[blkX][blkY].up != ALL_DENIED && blkX > 0 && score>m[blkX-1][blkY] ){score = m[blkX-1][blkY]; move = UP;}
    if(maze->maze[blkX][blkY].down != ALL_DENIED && blkX < size - 1 && score>m[blkX+1][blkY] ){score = m[blkX+1][blkY]; move = DOWN;}
    if(maze->maze[blkX][blkY].left != ALL_DENIED && blkY > 0 && score>m[blkX][blkY-1] ){score = m[blkX][blkY-1]; move = LEFT;}
    if(maze->maze[blkX][blkY].right != ALL_DENIED && blkY < size - 1 && score>m[blkX][blkY+1] ){score = m[blkX][blkY+1]; move = RIGHT;}
    
    return move;   // returns 0 if the block is unreachable or if the object is at the destination
}

int Ghost::moveTo(){
    int destX = destinationX, destY = destinationY, dimension = maze->dimension;
    vector<vector<int>> distance(dimension, std::vector<int>(dimension, -1));
    int blkX = maze->screenToBlockCoordinate(screenX, screenY).x;
    int blkY = maze->screenToBlockCoordinate(screenX, screenY).y;
    int moveDir = INITIAL;
    SDL_Point center = pac1->colliderSphere.center;
    SDL_Point pacLoc = maze->screenToBlockCoordinate(center.x, center.y);
    int state = pac1->state;
   
    if(mode == 1){
    	moveDir = BFS(destX, destY, distance);
    	while(moveDir == INITIAL) {
    		destX = rand() % dimension;
    		destY = rand() % dimension;
    		moveDir = BFS(destX, destY, distance);
    	}

   		if(type == TYPE_CLYDE) {
   			BFS(pacLoc.x, pacLoc.y, distance);
   			if(distance[blkX][blkY] > 3 && blkX == destinationX && blkY == destinationY) {
   				mode = 2;
   				destinationX = pacLoc.x;
   				destinationY = pacLoc.y;
   			}
   			else if(distance[blkX][blkY] > 3) {
   				if(destX != destinationX || destY != destinationY) {
   					mode = 2;
   					destinationX = pacLoc.x;
   					destinationY = pacLoc.y;
   				}
   				else {
   					destinationX = destX;
   					destinationY = destY;
   				}
   			}
   			else {
   				destinationX = destX;
   				destinationY = destY;
   			}
   		}
   		else {
   			destinationX = destX;
   			destinationY = destY;
    	}
        return moveDir;
    }
    else {
		if(type == TYPE_BLINKY){
	        destinationY = pacLoc.y; destinationX = pacLoc.x;
	        return BFS(destinationX, destinationY, distance);
		}
		else if(type == TYPE_PINKY){
	        if(!randomOn) {	
		        int nextX = 0, nextY = 0;
		        if(state == MOVE_UP || state == STILL_UP) {
		        	nextX = -2;
		        }
		        else if(state == MOVE_DOWN || state == STILL_DOWN) {
		        	nextX = 2;
		        }
		        else if(state == MOVE_RIGHT || state == STILL_RIGHT) {
		        	nextY = 2;
		        }
		        else if(state == MOVE_LEFT || state == STILL_LEFT) {
		        	nextY = -2;
		        }
		        destinationX = (dimension + pacLoc.x + nextX) % dimension; 
		        destinationY = (dimension + pacLoc.y + nextY) % dimension;
		        moveDir = BFS(destinationX, destinationY, distance);
		        while(moveDir == INITIAL) {
		        	randomOn = true;
		        	destinationX = rand() % dimension;
		        	destinationY = rand() % dimension;
		        	moveDir = BFS(destinationX, destinationY, distance);
		        }
	        	return moveDir;
	        }
	        else {
	        	if(blkX == destinationX && blkY == destinationY) {
	        		randomOn = false;
	        	}
	        	else {
	        		return BFS(destinationX, destinationY, distance);
	        	}
	        }
	        
	    }
		else if(type == TYPE_INKY){
	        GHOST_VEL = 5;
	      	moveDir = BFS(destX, destY, distance);
	        while(moveDir == INITIAL) {
	        	destX = rand()%dimension;
	        	destY = rand()%dimension;
	        	moveDir = BFS(destX, destY, distance);
	        }
	        destinationX = destX;
	        destinationY = destY;
	        return moveDir;
	    }
		else if(type == TYPE_CLYDE){ 
	        destX = pacLoc.x; destY = pacLoc.y;
	        moveDir = BFS(destX, destY, distance);
	        if(distance[blkX][blkY] < 3){
	        	mode = 1; 
	        	destinationX = rand() % dimension; 
	        	destinationY = rand() % dimension;
	        }
	        else {
	        	destinationX = destX;
	        	destinationY = destY;
	        }
	        return moveDir;
	    }
	}
}

void Ghost::handleEvent(Pacman* pac1) {
	this->pac1 = pac1;
    checkAlignment();
	if(rowAligned && colAligned){
		direction = moveTo();
	}
	switch(direction) {
		case UP: velX = 0; velY = -GHOST_VEL; state = MOVE_UP; break;
		case DOWN: velX = 0; velY = GHOST_VEL; state = MOVE_DOWN; break;
		case RIGHT: velY = 0; velX = GHOST_VEL; state = MOVE_RIGHT; break;
		case LEFT: velY = 0; velX = -GHOST_VEL; state = MOVE_LEFT; break;
		default: ;break;
	}
}

void Ghost::move() {
	
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

void Ghost::render(Window* window) {
	SDL_Color color = {0x00, 0x00, 0xFF, 0xFF};
	SDL_Rect stillPosition = {0, 0, 39, 45};
	SDL_Rect movingPosition = {(frameCount / (2 * ANIMATION_FRAMES)) * 39, 0, 39, 45};
	SDL_Rect onScreenRect = {colliderBox.x + 3, colliderBox.y, 39, 45};

	switch(state) {
		case STILL_UP: 
			window->renderTexture(up, &stillPosition, &onScreenRect);
			break;
		case STILL_RIGHT: 
			if(mode == 1)
				window->renderTexture(right, &stillPosition, &onScreenRect);
			else
				window->renderTexture(rightAngry, &stillPosition, &onScreenRect);				
			break;
		case STILL_DOWN: 
			if(mode == 1)
				window->renderTexture(down, &stillPosition, &onScreenRect);
			else
				window->renderTexture(downAngry, &stillPosition, &onScreenRect);
			break;
		case STILL_LEFT: 
			if(mode == 1)
				window->renderTexture(left, &stillPosition, &onScreenRect);
			else
				window->renderTexture(leftAngry, &stillPosition, &onScreenRect);
			break;			
		case MOVE_UP:
			window->renderTexture(up, &movingPosition, &onScreenRect);
			frameCount++;
			break;
		case MOVE_RIGHT:
			if(mode == 1) {
				window->renderTexture(right, &movingPosition, &onScreenRect);
			}
			else {
				window->renderTexture(rightAngry, &movingPosition, &onScreenRect);
			}
			frameCount++;
			break;
		case MOVE_DOWN:
			if(mode == 1) {
				window->renderTexture(down, &movingPosition, &onScreenRect);
			}
			else {
				window->renderTexture(downAngry, &movingPosition, &onScreenRect);
			}
			frameCount++;
			break;
		case MOVE_LEFT:
			if(mode == 1) {
				window->renderTexture(left, &movingPosition, &onScreenRect);
			}
			else {
				window->renderTexture(leftAngry, &movingPosition, &onScreenRect);
			}
			frameCount++;
			break;			
	}
	if(frameCount == 8) {
		frameCount = 0;
	}

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
