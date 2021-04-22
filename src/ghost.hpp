#pragma once

#include <vector>
#include <stdlib.h>
#include <time.h>
#include "maze.hpp"
#include "window.hpp"

using namespace std;

class Ghost {

    public:

    static const int GHOST_WIDTH = 45;
    static const int GHOST_HEIGHT = 45;
    int GHOST_VEL = 1;     // 1 pixel per frame
	static const int UP = 1; 
	static const int DOWN = 2; 
	static const int LEFT = 3; 
	static const int RIGHT = 4; 

    Ghost();
    Ghost(Maze* maze, int j, int k);
    void checkAlignment();
    void boundingAlignedAll();
    void boundingAlignedRow();
    void boundingAlignedCol(int i);
    void handleEvent(SDL_Point P, int vx, int vy);		// handle dynamics
    void move();						
    void render(Window* window);        // render GHOST
    bool collisionDetectorRect(SDL_Rect* rect1, SDL_Rect* rect2);
    int BFS(int i, int j, vector<vector<int>> &V);
    
    bool rowAligned, colAligned;        // To check if ghost is row/column aligned
    Maze* maze;                         // Maze
    std::vector<SDL_Rect> boundingRect; // bounding rectangles
    int screenX, screenY;				// screen coordinates of ghost
    int velX, velY;						// horizontal and vertical velocities in pixels per frame
    SDL_Rect colliderBox;
    int moveTo();
    int type;
    SDL_Point pacLoc;
    int pacVelX;
    int pacVelY;
    int direction;
    int destinationX;
    int destinationY;
    int mode;
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
	if(maze->maze[point.x][point.y].up == ALL_DENIED) {
		boundingRect.push_back(rectangles[0]);
	}
	if(maze->maze[point.x][point.y].right == ALL_DENIED) {
		boundingRect.push_back(rectangles[1]);
	}
	if(maze->maze[point.x][point.y].down == ALL_DENIED) {
		boundingRect.push_back(rectangles[2]);
	}
	if(maze->maze[point.x][point.y].left == ALL_DENIED) {
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
	if(maze->maze[point.x][point.y].up == ALL_DENIED) {
		boundingRect.push_back(rectangles[0]);
		boundingRect.push_back(rectangles[1]);
		dot1 = true;
	}
	if(maze->maze[point.x][point.y + 1].up == ALL_DENIED) {
		boundingRect.push_back(rectangles[2]);
		if(!dot1) {
			boundingRect.push_back(rectangles[1]);
			dot1 = true;
		}
	}
	if(maze->maze[point.x][point.y].down == ALL_DENIED) {
		boundingRect.push_back(rectangles[3]);
		boundingRect.push_back(rectangles[4]);
		dot2 = true;
	}
	if(maze->maze[point.x][point.y + 1].down == ALL_DENIED) {
		boundingRect.push_back(rectangles[5]);
		if(!dot2) {
			boundingRect.push_back(rectangles[4]);
			dot2 = true;
		}
	}
	if(!dot1) {
		if(maze->maze[point.x - 1][point.y].right == ALL_DENIED) {
			boundingRect.push_back(rectangles[1]);
			dot1 = true;	
		}
	}
	if(!dot2) {
		if(maze->maze[point.x + 1][point.y].right == ALL_DENIED) {
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
	if(maze->maze[point.x][point.y].right == ALL_DENIED) {
		boundingRect.push_back(rectangles[0]);
		boundingRect.push_back(rectangles[1]);
		dot1 = true;
	}
	if(maze->maze[point.x + 1][point.y].right == ALL_DENIED) {
		boundingRect.push_back(rectangles[2]);
		if(!dot1) {
			boundingRect.push_back(rectangles[1]);
			dot1 = true;
		}
	}
	if(maze->maze[point.x][point.y].left == ALL_DENIED) {
		boundingRect.push_back(rectangles[3]);
		boundingRect.push_back(rectangles[4]);
		dot2 = true;
	}
	if(maze->maze[point.x + 1][point.y].left == ALL_DENIED) {
		boundingRect.push_back(rectangles[5]);
		if(!dot2) {
			boundingRect.push_back(rectangles[4]);
			dot2 = true;
		}
	}
	if(!dot1) {
		if(maze->maze[point.x][point.y + 1].down == ALL_DENIED) {
			boundingRect.push_back(rectangles[1]);
			dot1 = true;	
		}
	}
	if(!dot2) {
		if(maze->maze[point.x][point.y - 1].down == ALL_DENIED) {
			boundingRect.push_back(rectangles[4]);
			dot2 = true;	
		}
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
        if(maze->maze[pt[0]][pt[1]].up != ALL_DENIED && m[pt[0]-1][pt[1]] == -1){
            m[pt[0]-1][pt[1]] = m[pt[0]][pt[1]] + 1;
            que.push_back(vector<int> {pt[0]-1, pt[1]});
        }
        if(maze->maze[pt[0]][pt[1]].down != ALL_DENIED && m[pt[0]+1][pt[1]] == -1){
            m[pt[0]+1][pt[1]] = m[pt[0]][pt[1]] + 1;
            que.push_back(vector<int> {pt[0]+1, pt[1]});
        }
        if(maze->maze[pt[0]][pt[1]].right != ALL_DENIED && m[pt[0]][pt[1]+1] == -1){
            m[pt[0]][pt[1]+1] = m[pt[0]][pt[1]] + 1;
            que.push_back(vector<int> {pt[0], pt[1]+1});
        }
        if(maze->maze[pt[0]][pt[1]].left != ALL_DENIED && m[pt[0]][pt[1]-1] == -1){
            m[pt[0]][pt[1]-1] = m[pt[0]][pt[1]] + 1;
            que.push_back(vector<int> {pt[0], pt[1]-1});
        }
    }
    int blkX = maze->screenToBlockCoordinate(screenX, screenY).x, blkY = maze->screenToBlockCoordinate(screenX, screenY).y;
    int score = m[blkX][blkY], move = 0;
    if(maze->maze[blkX][blkY].up != ALL_DENIED && score>m[blkX-1][blkY] ){score = m[blkX-1][blkY]; move = UP;}
    if(maze->maze[blkX][blkY].down != ALL_DENIED && score>m[blkX+1][blkY] ){score = m[blkX+1][blkY]; move = DOWN;}
    if(maze->maze[blkX][blkY].left != ALL_DENIED && score>m[blkX][blkY-1] ){score = m[blkX][blkY-1]; move = LEFT;}
    if(maze->maze[blkX][blkY].right != ALL_DENIED && score>m[blkX][blkY+1] ){score = m[blkX][blkY+1]; move = RIGHT;}
    return move;
}

int Ghost::moveTo(){
    int destX, destY, dimension = maze->dimension;
    vector<vector<int>> dist(dimension);
    
    if(mode == 1){
        srand(time(0));
        int blkX = maze->screenToBlockCoordinate(screenX, screenY).x, blkY = maze->screenToBlockCoordinate(screenX, screenY).y;
        if(blkX == destinationX && blkY == destinationY){
        	destX = (dimension + (rand()%dimension))%dimension;
        	destY = (dimension + (rand()%dimension))%dimension;
        	destinationX = destX; destinationY = destY;}
        else{
        	destX = destinationX; destY =destinationY;
        }
        int move = BFS(destX, destY, dist);
        destX = pacLoc.x; destY = pacLoc.y;
        BFS(destX, destY, dist);
        if(type == 4 && dist[blkX][blkY] > 5 && blkX == destinationX && blkY == destinationY){mode = 2;}
        
        return move;
    }

	if(type == 1){
        destX = pacLoc.x; destY = pacLoc.y;
        return BFS(destX, destY, dist);
	}
	if(type == 2){
        int nextX = pacVelX>0?1:-1, nextY = pacVelY>0?1:-1;
        if(pacVelX == 0) {nextX = 0;}
        if(pacVelY == 0) {nextY = 0;}
        destX = pacLoc.x + nextX; destY = pacLoc.y + nextY;
        return BFS(destX, destY, dist);
    }
	if(type == 3){
        destX = pacLoc.x; destY = pacLoc.y;
        GHOST_VEL = 5;
        mode = 1;
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

void Ghost::handleEvent(SDL_Point p, int vx, int vy) {
	pacLoc = p;
	pacVelX = vx;
	pacVelY = vy;
    	checkAlignment();
    	if(rowAligned && colAligned){
    		direction = moveTo();
    	}
	switch(direction) {
		case UP: velX = 0; velY = -GHOST_VEL; break;
		case DOWN: velX = 0; velY = GHOST_VEL; break;
		case RIGHT: velY = 0; velX = GHOST_VEL; break;
		case LEFT: velY = 0; velX = -GHOST_VEL; break;
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
	SDL_Color color;
	if(type == 1) {color = {0xFF, 0x25, 0x0C, 0xFF};}
	if(type == 2) {color = {0x1A, 0xA9, 0xDB, 0xFF};}
	if(type == 3) {color = {0xF0, 0x85, 0xA0, 0xFF};}
	if(type == 4) {color = {0x5B, 0xB3, 0x0F, 0xFF};}
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
