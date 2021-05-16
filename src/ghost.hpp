#pragma once

#include <SDL2/SDL_mixer.h>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <random>
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
    void free();
    Ghost();
    void splitGhost(Ghost g);
    Ghost(Maze* maze, int j, int k, Window* window, int channel);
    void loadTexture(Window* window);
    void checkAlignment();
    void update(Pacman* pac1, Pacman* pac2);		// handle dynamics
    void move();						
    void render(Window* window);        // render GHOST
    bool parryPossible(Pacman* pac1);
    void handleEvent(SDL_Event event, Pacman* pac1, Pacman* pac2);
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
    std::mt19937 generator;
    
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
    SDL_Texture* final;

    Mix_Chunk* parry;
    Mix_Chunk* pacDeath;
    Mix_Chunk* ghostDeath;
    Mix_Chunk* retreat;

    bool randomOn;				
    bool isDead;
    bool isScared;
    Window* window;

    int channel;
};