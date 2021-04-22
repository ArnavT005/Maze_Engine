#pragma once

#include <vector>
#include <stdlib.h>
#include <time.h>
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

class Ghost {

    public:

    static const int GHOST_WIDTH = 45;
    static const int GHOST_HEIGHT = 45;

    Ghost();
    Ghost(Maze* maze, int j, int k);
    void checkAlignment();
    void handleEvent(SDL_Point P, int vx, int vy);		// handle dynamics
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
    SDL_Point pacLoc;       // use object pointer
    int pacVelX;
    int pacVelY;
    int direction;
    int destinationX;
    int destinationY;		
    int mode;				// set value default
    int state;
    int GHOST_VEL;     		// 1 pixel per frame
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
	GHOST_VEL = 1;
	destinationY = destinationX = 0;
	direction = INITIAL;
	state = STILL_UP;
}

Ghost::Ghost(Maze* maze, int ghostType, int mode) {
	
	int blockSize = maze->blockSize,
		dotSize = maze->dotSize,
		padding = maze->padding;
    this->mode = mode;
	rowAligned = colAligned = true;
    int mid = maze->dimension/2 - 1;
    if(ghostType == 1) {screenX = screenY = maze->getBlockScreenCoordinate(mid, mid).x;destinationX = mid; destinationY = mid;}
    if(ghostType == 2) {screenX = maze->getBlockScreenCoordinate(mid, mid + 1).x; screenY = maze->getBlockScreenCoordinate(mid, mid + 1).y;destinationX = mid; destinationY = mid+1;}
    if(ghostType == 3) {screenX = maze->getBlockScreenCoordinate(mid + 1, mid).x; screenY = maze->getBlockScreenCoordinate(mid + 1, mid).y;destinationX = mid+1; destinationY = mid;}
    if(ghostType == 4) {screenX = screenY = maze->getBlockScreenCoordinate(mid + 1, mid + 1).x;destinationX = mid+1; destinationY = mid+1;}
	velX = velY = 0;
	this->maze = maze;
	boundingRect = maze->boundaryRectGhost;
	colliderBox.x = screenX;
	colliderBox.y = screenY;
	colliderBox.w = GHOST_WIDTH;
	colliderBox.h = GHOST_HEIGHT;
	type = ghostType;
	if(mode == 2 && type == 3) {
		GHOST_VEL = 5;
	}
	else {
		GHOST_VEL = 1;
	}
	direction = INITIAL;
	state = STILL_UP;
}

// check once
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
        m[i] = vector<int>(size);
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
    return move;
}

int Ghost::moveTo(){
    int destX, destY, dimension = maze->dimension;
    vector<vector<int>> dist(dimension);
    int blkX = 0, blkY = 0;
    
    if(mode == 1){
    	GHOST_VEL = 1;
        blkX = maze->screenToBlockCoordinate(screenX, screenY).x;
        blkY = maze->screenToBlockCoordinate(screenX, screenY).y;
        if(blkX == destinationX && blkY == destinationY){
        	destX = rand()%dimension;
        	destY = rand()%dimension;
        	destinationX = destX; destinationY = destY;}
        else{
        	destX = destinationX; destY =destinationY;
        }
        int move = BFS(destX, destY, dist);
        // destX = pacLoc.x; destY = pacLoc.y;
        // BFS(destX, destY, dist);
        // if(type == 4 && dist[blkX][blkY] > 5 && blkX == destinationX && blkY == destinationY){mode = 2;}
        
        return move;
    }
    else {
		if(type == 1){
	        destX = pacLoc.x; destY = pacLoc.y;
	        destinationY = destY; destinationX = destX;
	        return BFS(destX, destY, dist);
		}
		if(type == 2){
	        int nextX = pacVelX>0?1:-1, nextY = pacVelY>0?1:-1;
	        if(pacVelX == 0) {nextX = 0;}
	        if(pacVelY == 0) {nextY = 0;}
	        destX = (dimension + pacLoc.x + nextX) % dimension; destY = (dimension + pacLoc.y + nextY) % dimension;
	        destinationY = destY; destinationX = destX;
	        return BFS(destX, destY, dist);
	    }
		if(type == 3){
	        GHOST_VEL = 5;
	        srand(time(0));
        	blkX = maze->screenToBlockCoordinate(screenX, screenY).x;
        	blkY = maze->screenToBlockCoordinate(screenX, screenY).y;
        	if(blkX == destinationX && blkY == destinationY){
        		destX = rand()%dimension;
        		destY = rand()%dimension;
        		destinationX = destX; destinationY = destY;}
        	else{
        		destX = destinationX; destY =destinationY;
        	}
        	return BFS(destX, destY, dist);
	    }
		if(type == 4){
	        int move, blkX = maze->screenToBlockCoordinate(screenX, screenY).x, blkY = maze->screenToBlockCoordinate(screenX, screenY).y;
	        destX = pacLoc.x; destY = pacLoc.y;
	        move = BFS(destX, destY, dist);
	        if(dist[blkX][blkY] < 5){mode = 1;}
	        return move;
	    }
	}
}

void Ghost::handleEvent(SDL_Point p, int vx, int vy) {
	pacLoc = p;
	pacVelX = vx;
	pacVelY = vy;
	int i = 0;
    checkAlignment();
	if(rowAligned && colAligned){
		direction = moveTo();
		while(direction == 0 && i < 16) {
			direction = moveTo();
			i ++;
		}
	}
	switch(direction) {
		case UP: velX = 0; velY = -GHOST_VEL; state = MOVE_UP; break;
		case DOWN: velX = 0; velY = GHOST_VEL; state = MOVE_DOWN; break;
		case RIGHT: velY = 0; velX = GHOST_VEL; state = MOVE_RIGHT; break;
		case LEFT: velY = 0; velX = -GHOST_VEL; state = MOVE_LEFT; break;
		default: velX = 0; velY = 0; std::cout << direction << " " << type << "\n";break;
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
		//colliderSphere.center.x += velX;
		for(int i = 0; i < size; i ++) {
			if(collisionDetectorRect(&colliderBox, &boundingRect[i])) {
				screenX -= 3 * velX;
				colliderBox.x = screenX;
				colliderBox.y = screenY;
				//colliderSphere.center.x -= velX;
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
		//colliderSphere.center.y += velY;
		for(int i = 0; i < size; i ++) {
			if(collisionDetectorRect(&colliderBox, &boundingRect[i])) {
				screenY -= 3 * velY;
				colliderBox.x = screenX;
				colliderBox.y = screenY;
				//colliderSphere.center.y -= velY;
				state = velY > 0 ? STILL_DOWN : STILL_UP;
				collision = true;
				break;
			}
		}
	}
}

void Ghost::render(Window* window) {
	SDL_Color color;
	if(type == 1) {color = {0xFF, 0x25, 0x0C, 0xFF};}	// red
	if(type == 2) {color = {0x1A, 0xA9, 0xDB, 0xFF};}	// blue
	if(type == 3) {color = {0xF0, 0x85, 0xA0, 0xFF};}	// pink
	if(type == 4) {color = {0x5B, 0xB3, 0x0F, 0xFF};}	// green
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
