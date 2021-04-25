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

    static const int BOX_WIDTH = 45;
    static const int BOX_HEIGHT = 45;
    static const int GHOST_WIDTH = 35;
    static const int GHOST_HEIGHT = 45;
    static const int ANIMATION_FRAMES = 2;

    Ghost();
    Ghost(Maze* maze, int j, int k, Window* window);
    void loadTexture(Window* window);
    void checkAlignment();
    void update(Pacman* pac1);		// handle dynamics
    void move();						
    void render(Window* window);        // render GHOST
    bool parryPossible(Pacman* pac1);
    void handleEvent(SDL_Event event, Pacman* pac1);
    bool collisionDetectorRect(SDL_Rect* rect1, SDL_Rect* rect2);
    bool collisionDetectorCircle(Circle* circle1, Circle* circle2);
    int BFS(int i, int j, vector<vector<int>> &V);
    int moveTo();
    void checkPacmanCollision(Pacman* pac1);

    
    bool rowAligned, colAligned;        // To check if ghost is row/column aligned
    Maze* maze;                         // Maze
    std::vector<SDL_Rect> boundingRect; // bounding rectangles
    int screenX, screenY;				// screen coordinates of ghost
    int velX, velY;						// horizontal and vertical velocities in pixels per frame
    SDL_Rect colliderBox;
    Circle colliderSphere;				// circular collision detector box	

    int type;
    Pacman* pac1;				// pacman
    int direction;
    int destinationX;
    int destinationY;		
    int mode;					// set value default
    int prevMode;
    int state;
    int GHOST_VEL;     			// 1 pixel per frame
    int prevVel;
    bool success;				// error reporting flag
    int frameCount;
    Uint32 scareStart;
    SDL_Texture* up;
    SDL_Texture* right;
    SDL_Texture* down;
    SDL_Texture* left;			// rendering textures
    SDL_Texture* rightAngry;
    SDL_Texture* downAngry;
    SDL_Texture* leftAngry;		// rendering angry textures
    SDL_Texture* upScared;
    SDL_Texture* rightScared;
    SDL_Texture* downScared;
    SDL_Texture* leftScared;    // rendering scared textures
    SDL_Texture* rightDead;
    SDL_Texture* upDead;
    SDL_Texture* downDead;
    SDL_Texture* leftDead;
    bool randomOn;				
    bool isDead;
    bool isScared;

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
	colliderBox.w = BOX_WIDTH;
	colliderBox.h = BOX_HEIGHT;
	colliderSphere.radius = (GHOST_WIDTH - 1) / 2;
	colliderSphere.center.x = screenX + BOX_WIDTH / 2;
	colliderSphere.center.y = screenY + BOX_HEIGHT / 2;
	type = 1;
	mode = 1;
	GHOST_VEL = 1;
	destinationY = destinationX = 0;
	direction = INITIAL;
	state = STILL_UP;
	success = true;
	frameCount = 0;
	randomOn = false;
	isDead = false;
	isScared = false;
}

Ghost::Ghost(Maze* maze, int ghostType, int mode, Window* window) {

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
	colliderBox.w = BOX_WIDTH;
	colliderBox.h = BOX_HEIGHT;
	colliderSphere.radius = (GHOST_WIDTH - 1) / 2;
	colliderSphere.center.x = screenX + BOX_WIDTH / 2;
	colliderSphere.center.y = screenY + BOX_HEIGHT / 2;
	type = ghostType;
	direction = INITIAL;
	state = STILL_UP;
	success = true;
	frameCount = 0;
	randomOn = false;
	isDead = false;
	isScared = false;
	loadTexture(window);
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
	SDL_Surface* scaredUp = IMG_Load("../img/Scared/upScared.png");
	SDL_Surface* scaredRight = IMG_Load("../img/Scared/rightScared.png");
	SDL_Surface* scaredDown = IMG_Load("../img/Scared/downScared.png");
	SDL_Surface* scaredLeft = IMG_Load("../img/Scared/leftScared.png");

	if(scaredUp == NULL || scaredRight == NULL || scaredDown == NULL || scaredLeft == NULL) {
		std::cout << "Unable to load scared images! SDL_Image Error: " << IMG_GetError() << "\n";
		success = false;
	}
	upScared = SDL_CreateTextureFromSurface(window->getRenderer(), scaredUp);
	rightScared = SDL_CreateTextureFromSurface(window->getRenderer(), scaredRight);
	downScared = SDL_CreateTextureFromSurface(window->getRenderer(), scaredDown);
	leftScared = SDL_CreateTextureFromSurface(window->getRenderer(), scaredLeft);

	if(upScared == NULL || rightScared == NULL || downScared == NULL || leftScared == NULL) {
		std::cout <<  "Unable to create texture from image! SDL_Image Error: " << IMG_GetError() << "\n";
		success = false;
	}
	if(scaredUp != NULL)
		SDL_FreeSurface(scaredUp);
	if(scaredRight != NULL)
		SDL_FreeSurface(scaredRight);
	if(scaredDown != NULL)
		SDL_FreeSurface(scaredDown);
	if(scaredLeft != NULL)
		SDL_FreeSurface(scaredLeft);

	SDL_Surface* deadUp = IMG_Load("../img/Dead/upDead.png");
	SDL_Surface* deadRight = IMG_Load("../img/Dead/rightDead.png");
	SDL_Surface* deadDown = IMG_Load("../img/Dead/downDead.png");
	SDL_Surface* deadLeft = IMG_Load("../img/Dead/leftDead.png");

	if(deadUp == NULL || deadRight == NULL || deadDown == NULL || deadLeft == NULL) {
		std::cout << "Unable to load dead images! SDL_Image Error: " << IMG_GetError() << "\n";
		success = false;
	}
	upDead = SDL_CreateTextureFromSurface(window->getRenderer(), deadUp);
	rightDead = SDL_CreateTextureFromSurface(window->getRenderer(), deadRight);
	downDead = SDL_CreateTextureFromSurface(window->getRenderer(), deadDown);
	leftDead = SDL_CreateTextureFromSurface(window->getRenderer(), deadLeft);

	if(upDead == NULL || rightDead == NULL || downDead == NULL || leftDead == NULL) {
		std::cout <<  "Unable to create texture from image! SDL_Image Error: " << IMG_GetError() << "\n";
		success = false;
	}
	if(deadUp != NULL)
		SDL_FreeSurface(deadUp);
	if(deadRight != NULL)
		SDL_FreeSurface(deadRight);
	if(deadDown != NULL)
		SDL_FreeSurface(deadDown);
	if(deadLeft != NULL)
		SDL_FreeSurface(deadLeft);
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
        if(maze->maze[pt[0]][pt[1]].up != ALL_DENIED && maze->maze[pt[0]][pt[1]].up != GHOST_DENIED && pt[0] > 0 && m[pt[0]-1][pt[1]] == -1){
            m[pt[0]-1][pt[1]] = m[pt[0]][pt[1]] + 1;
            que.push_back(vector<int> {pt[0]-1, pt[1]});
        }
        if(maze->maze[pt[0]][pt[1]].down != ALL_DENIED && maze->maze[pt[0]][pt[1]].down != GHOST_DENIED && pt[0] < size - 1 && m[pt[0]+1][pt[1]] == -1){
            m[pt[0]+1][pt[1]] = m[pt[0]][pt[1]] + 1;
            que.push_back(vector<int> {pt[0]+1, pt[1]});
        }
        if(maze->maze[pt[0]][pt[1]].right != ALL_DENIED && maze->maze[pt[0]][pt[1]].right != GHOST_DENIED && pt[1] < size - 1 && m[pt[0]][pt[1]+1] == -1){
            m[pt[0]][pt[1]+1] = m[pt[0]][pt[1]] + 1;
            que.push_back(vector<int> {pt[0], pt[1]+1});
        }
        if(maze->maze[pt[0]][pt[1]].left != ALL_DENIED && maze->maze[pt[0]][pt[1]].left != GHOST_DENIED && pt[1] > 0 && m[pt[0]][pt[1]-1] == -1){
            m[pt[0]][pt[1]-1] = m[pt[0]][pt[1]] + 1;
            que.push_back(vector<int> {pt[0], pt[1]-1});
        }
    }
    int blkX = maze->screenToBlockCoordinate(screenX, screenY).x, blkY = maze->screenToBlockCoordinate(screenX, screenY).y;
    int score = m[blkX][blkY], move = 0;

    // Add randomness in case multiple directions have same distance
    if(maze->maze[blkX][blkY].up != ALL_DENIED && maze->maze[blkX][blkY].up != GHOST_DENIED && blkX > 0 && score>m[blkX-1][blkY] ){score = m[blkX-1][blkY]; move = UP;}
    if(maze->maze[blkX][blkY].down != ALL_DENIED && maze->maze[blkX][blkY].down != GHOST_DENIED && blkX < size - 1 && score>m[blkX+1][blkY] ){score = m[blkX+1][blkY]; move = DOWN;}
    if(maze->maze[blkX][blkY].left != ALL_DENIED && maze->maze[blkX][blkY].left != GHOST_DENIED &&blkY > 0 && score>m[blkX][blkY-1] ){score = m[blkX][blkY-1]; move = LEFT;}
    if(maze->maze[blkX][blkY].right != ALL_DENIED && maze->maze[blkX][blkY].right != GHOST_DENIED && blkY < size - 1 && score>m[blkX][blkY+1] ){score = m[blkX][blkY+1]; move = RIGHT;}
    
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
    else if(mode == 2){
		if(type == TYPE_BLINKY){
	        destY = pacLoc.y; destX = pacLoc.x;
	        moveDir = BFS(destX, destY, distance);
	        if(moveDir != 0)
	        	return moveDir;
	        else {
	        	moveDir = BFS(destinationX, destinationY, distance);
	        	while(moveDir == 0) {
	        		destinationX = rand() % dimension;
	        		destinationY = rand() % dimension;
	        		moveDir = BFS(destinationX, destinationY, distance);
	        	}
	        	return moveDir;	
	        }
	        
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
	else if(mode == 3){
		// mode = 3 (scared)
		// move away from pacman if possible, otherwise accept your fate
		int vert = pacLoc.x - blkX >= 0 ? pacLoc.x - blkX : blkX - pacLoc.x;
		int horiz  = pacLoc.y - blkY >= 0 ? pacLoc.y - blkY : blkY - pacLoc.y;
		BFS(pacLoc.x, pacLoc.y, distance);
		if(distance[blkX][blkY] <= 3) {
			if(pacLoc.x >= blkX) {
				if(pacLoc.y >= blkY) {
					if(maze->maze[blkX][blkY].left == ALL_DENIED || maze->maze[blkX][blkY].left == GHOST_DENIED) {
						if(maze->maze[blkX][blkY].up == ALL_DENIED || maze->maze[blkX][blkY].up == GHOST_DENIED) {
							if(horiz <= vert) {
								if(maze->maze[blkX][blkY].right != ALL_DENIED && maze->maze[blkX][blkY].right != GHOST_DENIED)
									return RIGHT;
								else
									return DOWN;
							}	
							else {
								if(maze->maze[blkX][blkY].down != ALL_DENIED && maze->maze[blkX][blkY].down != GHOST_DENIED)
									return DOWN;
								else
									return RIGHT;
							}
						}
						else
							return UP;
					}
					else {
						if(maze->maze[blkX][blkY].up == ALL_DENIED || maze->maze[blkX][blkY].up == GHOST_DENIED)
							return LEFT;
						else {	
							if(horiz <= vert)
								return LEFT;
							else
								return UP;
						}
					}
				}
				else {
					// pacLoc.y < blkY
					if(maze->maze[blkX][blkY].right == ALL_DENIED || maze->maze[blkX][blkY].right == GHOST_DENIED) {
						if(maze->maze[blkX][blkY].up == ALL_DENIED || maze->maze[blkX][blkY].up == GHOST_DENIED) {
							if(horiz <= vert) {
								if(maze->maze[blkX][blkY].left != ALL_DENIED && maze->maze[blkX][blkY].left != GHOST_DENIED)
									return LEFT;
								else
									return DOWN;
							}
							else {
								if(maze->maze[blkX][blkY].down != ALL_DENIED && maze->maze[blkX][blkY].down != GHOST_DENIED)
									return DOWN;
								else 
									return LEFT;
							}
						}
						else
							return UP;
					}
					else {
						if(maze->maze[blkX][blkY].up == ALL_DENIED || maze->maze[blkX][blkY].up == GHOST_DENIED)
							return RIGHT;
						else {	
							if(horiz <= vert)
								return RIGHT;
							else
								return UP;
						}
					}
				}
			}
			else {
				// pacLoc.x < blkX
				if(pacLoc.y >= blkY) {
					if(maze->maze[blkX][blkY].left == ALL_DENIED || maze->maze[blkX][blkY].left == GHOST_DENIED) {
						if(maze->maze[blkX][blkY].down == ALL_DENIED || maze->maze[blkX][blkY].down == GHOST_DENIED) {
							if(horiz <= vert) {
								if(maze->maze[blkX][blkY].right != ALL_DENIED && maze->maze[blkX][blkY].right != GHOST_DENIED)
									return RIGHT;
								else
									return UP;
							}
							else {
								if(maze->maze[blkX][blkY].up != ALL_DENIED && maze->maze[blkX][blkY].up!= GHOST_DENIED)
									return UP;
								else 
									return RIGHT;
							}
						}
						else
							return DOWN;
					}
					else {
						if(maze->maze[blkX][blkY].down == ALL_DENIED || maze->maze[blkX][blkY].down == GHOST_DENIED)
							return LEFT;
						else {	
							if(horiz <= vert)
								return LEFT;
							else
								return DOWN;
						}
					}
				}
				else {
					// pacLoc.y < blkY
					if(maze->maze[blkX][blkY].right == ALL_DENIED || maze->maze[blkX][blkY].right == GHOST_DENIED) {
						if(maze->maze[blkX][blkY].down == ALL_DENIED || maze->maze[blkX][blkY].down == GHOST_DENIED) {
							if(horiz <= vert) {
								if(maze->maze[blkX][blkY].left != ALL_DENIED && maze->maze[blkX][blkY].left != GHOST_DENIED)
									return LEFT;
								else
									return UP;
							}
							else {
								if(maze->maze[blkX][blkY].up != ALL_DENIED && maze->maze[blkX][blkY].up != GHOST_DENIED)
									return UP;
								else
									return LEFT;
							}
						}
						else
							return DOWN;
					}
					else {
						if(maze->maze[blkX][blkY].down == ALL_DENIED || maze->maze[blkX][blkY].down == GHOST_DENIED)
							return RIGHT;
						else {	
							if(horiz <= vert)
								return RIGHT;
							else
								return DOWN;
						}
					}
				}
			}
		}	
		else {
			// distance > 3, move randomly
			moveDir = BFS(destinationX, destinationY, distance);
			while(moveDir == 0) {
				destinationX = rand() % dimension;
				destinationY = rand() % dimension;
				moveDir = BFS(destinationX, destinationY, distance);
			}
			return moveDir;
		}
	}
	else {
		// mode = 4
		if(blkX == destinationX && blkY == destinationY) {
			isDead = false;
			isScared = false;
			mode = prevMode;
			GHOST_VEL = prevVel;
			return 0;
		}
		else {
			moveDir = BFS(destinationX, destinationY, distance);
			return moveDir;
		}
	}
}

void Ghost::update(Pacman* pac1) {
	this->pac1 = pac1;
    checkAlignment();
    if(pac1->isBuffed && mode != 3 && mode != 4) {
    	prevMode = mode;
    	GHOST_VEL = 1;
    	mode = 3;
    	isScared = true;
    	isDead = false;
    	scareStart = SDL_GetTicks();
    }
    else{
    	if(pac1->isBuffed) {
    		if(mode == 3) {
    			scareStart = SDL_GetTicks();
    		}
    	}
    	else {
    		if(mode == 3) {
    			if(SDL_GetTicks() - scareStart >= 10000) {
    				// it has been 10 seconds
    				mode = prevMode;
    				isScared = false;
    				isDead = false;
    			}
    		}
    	}
    }
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
		colliderSphere.center.x += velX;		
		for(int i = 0; i < size; i ++) {
			if(collisionDetectorRect(&colliderBox, &boundingRect[i])) {
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
			if(collisionDetectorRect(&colliderBox, &boundingRect[i])) {
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

void Ghost::render(Window* window) {
	SDL_Color color = {0x00, 0x00, 0xFF, 0xFF};
	SDL_Rect stillPosition = {0, 0, 39, 45};
	SDL_Rect movingPosition = {(frameCount / (2 * ANIMATION_FRAMES)) * 39, 0, 39, 45};
	SDL_Rect onScreenRect = {colliderBox.x + 3, colliderBox.y, 39, 45};

	switch(state) {
		case STILL_UP: 
			if(mode == 1 || mode == 2)
				window->renderTexture(up, &stillPosition, &onScreenRect);
			else if(mode == 3)
				window->renderTexture(upScared, &stillPosition, &onScreenRect);	
			else 
				window->renderTexture(upDead, &stillPosition, &onScreenRect);
			break;
		case STILL_RIGHT: 
			if(mode == 1)
				window->renderTexture(right, &stillPosition, &onScreenRect);
			else if(mode == 2)
				window->renderTexture(rightAngry, &stillPosition, &onScreenRect);	
			else if(mode == 3)
				window->renderTexture(rightScared, &stillPosition, &onScreenRect);	
			else 
				window->renderTexture(rightDead, &stillPosition, &onScreenRect);				
			break;
		case STILL_DOWN: 
			if(mode == 1)
				window->renderTexture(down, &stillPosition, &onScreenRect);
			else if(mode == 2)
				window->renderTexture(downAngry, &stillPosition, &onScreenRect);
			else if(mode == 3)
				window->renderTexture(downScared, &stillPosition, &onScreenRect);
			else 
				window->renderTexture(downDead, &stillPosition, &onScreenRect);		
			break;
		case STILL_LEFT: 
			if(mode == 1)
				window->renderTexture(left, &stillPosition, &onScreenRect);
			else if(mode == 2)
				window->renderTexture(leftAngry, &stillPosition, &onScreenRect);
			else if(mode == 3)
				window->renderTexture(leftScared, &stillPosition, &onScreenRect);	
			else 
				window->renderTexture(leftDead, &stillPosition, &onScreenRect);
			break;			
		case MOVE_UP:
			if(mode == 1 || mode == 2) {
				window->renderTexture(up, &movingPosition, &onScreenRect);
				frameCount++;
			}
			else if(mode == 3) {
				window->renderTexture(upScared, &movingPosition, &onScreenRect);
				frameCount++;
			}
			else {
				window->renderTexture(upDead, &stillPosition, &onScreenRect);		
				frameCount = 0;
			}
			break;
		case MOVE_RIGHT:
			if(mode == 1) {
				window->renderTexture(right, &movingPosition, &onScreenRect);
				frameCount++;
			}
			else if(mode == 2){
				window->renderTexture(rightAngry, &movingPosition, &onScreenRect);
				frameCount++;
			}
			else if(mode == 3) {
				window->renderTexture(rightScared, &movingPosition, &onScreenRect);
				frameCount++;
			}
			else  {
				window->renderTexture(rightDead, &stillPosition, &onScreenRect);
				frameCount = 0;
			}
			break;
		case MOVE_DOWN:
			if(mode == 1) {
				window->renderTexture(down, &movingPosition, &onScreenRect);
				frameCount++;
			}
			else if(mode == 2){
				window->renderTexture(downAngry, &movingPosition, &onScreenRect);
				frameCount++;
			}
			else if(mode == 3) {
				window->renderTexture(downScared, &movingPosition, &onScreenRect);
				frameCount++;
			}
			else {
				window->renderTexture(downDead, &stillPosition, &onScreenRect);
				frameCount = 0;
			}
			break;
		case MOVE_LEFT:
			if(mode == 1) {
				window->renderTexture(left, &movingPosition, &onScreenRect);
				frameCount++;
			}
			else if(mode == 2){
				window->renderTexture(leftAngry, &movingPosition, &onScreenRect);
				frameCount++;

			}
			else if(mode == 3) {
				window->renderTexture(leftScared, &movingPosition, &onScreenRect);
				frameCount++;

			}
			else {
				window->renderTexture(leftDead, &stillPosition, &onScreenRect);
				frameCount = 0;
			}
			break;			
	}
	if(frameCount == 8) {
		frameCount = 0;
	}

}

bool Ghost::parryPossible(Pacman* pac1) {
	switch(state) {
		case MOVE_UP:
			if(pac1->state == STILL_DOWN || pac1->state == MOVE_DOWN) {
				if(pac1->colliderSphere.center.y < colliderSphere.center.y)
					return true;
				else
					return false;
			}
			else 
				return false;
			break;
		case MOVE_RIGHT:
			if(pac1->state == STILL_LEFT || pac1->state == MOVE_LEFT) {
				if(pac1->colliderSphere.center.x > colliderSphere.center.x)
					return true;
				else
					return false;
			}
			else 
				return false;
			break;
		case MOVE_DOWN: 
			if(pac1->state == STILL_UP || pac1->state == MOVE_UP) {
				if(pac1->colliderSphere.center.y > colliderSphere.center.y)
					return true;
				else
					return false;
			}
			else 
				return false;
			break;	
		case MOVE_LEFT:
			if(pac1->state == STILL_RIGHT || pac1->state == MOVE_RIGHT) {
				if(pac1->colliderSphere.center.x < colliderSphere.center.x)
					return true;
				else
					return false;
			}
			else 
				return false;
			break;	
		default:
			return false;
			break;	
	}
}

void Ghost::handleEvent(SDL_Event event, Pacman *pac1) {
	if(pac1->isDead || pac1->parry)
		return;
	if(event.type == SDL_KEYDOWN && event.key.repeat == 0 && event.key.keysym.sym == SDLK_x) {
		if(!collisionDetectorCircle(&colliderSphere, &pac1->colliderSphere)) {
			if(collisionDetectorCircle(&colliderSphere, &pac1->parryCircle) && parryPossible(pac1)) {
				if(mode != 3)
					prevMode = mode;
				mode = 4;
				prevVel = GHOST_VEL;
				GHOST_VEL = 5;
				isScared = false;
				isDead = true;
				SDL_Point point = maze->screenToBlockCoordinate(screenX, screenY);
				SDL_Point screenPoint = maze->getBlockScreenCoordinate(point.x, point.y);
				screenX = screenPoint.x;
				screenY = screenPoint.y;
				colliderBox.x = screenX;
				colliderBox.y = screenY;
				colliderSphere.center.x = screenX + BOX_WIDTH / 2;
				colliderSphere.center.y = screenY + BOX_HEIGHT / 2;
				destinationX = maze->dimension / 2;
				destinationY = maze->dimension / 2;
				pac1->parry = true;   // parry successful
				pac1->parryStart = SDL_GetTicks();
				pac1->parryCount = 0;
			}
		}
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

bool Ghost::collisionDetectorCircle(Circle* circle1, Circle* circle2) {
	int r1 = circle1->radius, r2 = circle2->radius;
	SDL_Point point1 = circle1->center, point2 = circle2->center;

	// check collision
	int distanceSq = (point1.x - point2.x) * (point1.x - point2.x) + (point1.y - point2.y) * (point1.y - point2.y);
	if(distanceSq < (r1 + r2) * (r1 + r2)) {
		return true;
	}
	else {
		return false;
	}
}


void Ghost::checkPacmanCollision(Pacman* pac1) {
	
	bool checkCollision = collisionDetectorCircle(&colliderSphere, &(pac1->colliderSphere));

	if(checkCollision && !isScared && !isDead) {
		if(!(pac1->isDead)) {
			pac1->isDead = true;
			pac1->parry = false;
			pac1->frameCount = 0;
			pac1->parryCount = 0;
		}
	}
	else if(checkCollision && !isDead){
		//std::cout << "Hello\n";
		mode = 4;
		prevVel = GHOST_VEL;
		GHOST_VEL = 5;
		isScared = false;
		isDead = true;
		SDL_Point point = maze->screenToBlockCoordinate(screenX, screenY);
		SDL_Point screenPoint = maze->getBlockScreenCoordinate(point.x, point.y);
		screenX = screenPoint.x;
		screenY = screenPoint.y;
		colliderBox.x = screenX;
		colliderBox.y = screenY;
		colliderSphere.center.x = screenX + BOX_WIDTH / 2;
		colliderSphere.center.y = screenY + BOX_HEIGHT / 2;
		destinationX = maze->dimension / 2;
		destinationY = maze->dimension / 2;
	}
}