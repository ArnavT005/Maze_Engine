#pragma once

#include <SDL2/SDL_mixer.h>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <random>
#include "window.hpp"
#include "maze.hpp"
#include "eatable.hpp"

using namespace std;


enum {
	INITIAL,
	UP, 
	DOWN,
	LEFT, 
	RIGHT
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

class Ghost {

    public:

    static const int BOX_WIDTH = 45;
    static const int BOX_HEIGHT = 45;
    static const int GHOST_WIDTH = 35;
    static const int GHOST_HEIGHT = 45;
    static const int ANIMATION_FRAMES = 2;
    static const int GHOST_VEL = 1;
    void free();
    Ghost();
    Ghost(Maze* maze, Window* window, int startX, int startY);
    void loadTexture(Window* window);
    void checkAlignment();
    void update(Window* window);		// handle dynamics
    void move();						
    void render(Window* window);        // render GHOST
    int BFS(int destX, int destY);
    bool collisionDetectorRect(SDL_Rect* rect1, SDL_Rect* rect2);
    bool collisionDetectorCircle(Circle* circle1, Circle* circle2);
    bool isIn(std::pair<int, int> location);
    std::pair<int, int> chooseLoc(Window* window);
    void renderRadar(Window* window);
    
    bool rowAligned, colAligned;        // To check if ghost is row/column aligned
    Maze* maze;                         // Maze
    std::vector<SDL_Rect> boundingRect; // bounding rectangles
    int screenX, screenY;				// screen coordinates of ghost
    int velX, velY;						// horizontal and vertical velocities in pixels per frame
    SDL_Rect colliderBox;
    Circle colliderSphere;				// circular collision detector box	

    int direction;
    int state;
    int destinationX;
    int destinationY;		
    bool success;				// error reporting flag
    int frameCount;
    int distance;               // distance travelled by the ghost
    int radarRadius;


    std::vector<std::pair<int, int>> dest; // set of locations to visit
    bool reached;
    bool targetAcquired;
    bool end;
    std::vector<Eatable> large;
    std::vector<Eatable> small;

    SDL_Point points[952576];


    Uint32 reachedStart;
    
    SDL_Texture* up;
    SDL_Texture* right;
    SDL_Texture* down;
    SDL_Texture* left;			// rendering textures

    Mix_Chunk* movement;
    Mix_Chunk* endSound;
    Mix_Chunk* found;
};