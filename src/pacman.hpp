#pragma once

#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "window.hpp"
#include "maze.hpp"

const int BOX_WIDTH = 45;
const int BOX_HEIGHT = 45;
const int PACMAN_WIDTH = 31;
const int PACMAN_HEIGHT = 41;
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

   	void free();
    Pacman();
    Pacman(Maze* maze, Window* window, int i);
    void loadTexture(Window* window);
    void handleEvent(SDL_Event &e, const Uint8* keyStates, int online);		// handle dynamics
    void move();						
    void render(Window* window);        // render PACMAN
    bool collisionDetectorRect(SDL_Rect* rect1, SDL_Rect* rect2);
    bool collisionDetectorCircle(Circle* circle, SDL_Rect* rect);
    void pacpacCollision(Pacman* pac);
    bool collisionDetectorCirclesOnly(Circle* circle1, Circle* circle2);

    bool rowAligned, colAligned;        // To check if pacman is row/column aligned
    Maze* maze;                         // Maze
    std::vector<SDL_Rect> boundingRect; // bounding rectangles
    int screenX, screenY;				// screen coordinates of pacman
    int velX, velY;						// horizontal and vertical velocities in pixels per frame
    SDL_Rect colliderBox;				// rectangular collision detector box
    Circle colliderSphere;				// circular collision detector box	
    Circle parryCircle;
    int state;							// motion state of pacman							
    int frameCount;						// frame count number
    SDL_Point respawnPoint;
    Uint32 parryStart;
    bool success;					    // error reporting flag
    bool isDead;

    int score;							// game score

    bool isBuffed;
    bool parry;
    
    int parryCount;
    
    int type;
    int lives;
    int life;

    SDL_Texture* up;
    SDL_Texture* right;
    SDL_Texture* down;
    SDL_Texture* left;

    SDL_Texture* die;

    Mix_Chunk* pacPacCollision;
};
