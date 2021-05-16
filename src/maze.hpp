#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <time.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <random>
#include <chrono>
#include "block.hpp"
#include "window.hpp"

class Maze {
    
    public:
    void free();
    Maze();
    Maze(int dimension, int blockSize, int dotSize, int padding = 25);
    void setMazeBlock(int i, int j, int degree);
    Block getMazeBlock(int i, int j);
    SDL_Point getBlockScreenCoordinate(int i, int j);
    SDL_Point getDotScreenCoordinate(int i, int j);
    SDL_Point screenToDotCoordinate(int x, int y);
    SDL_Point screenToBlockCoordinate(int x, int y);
    void loadTexture(Window* window);
    void refreshMaze(Window* window);
    void createBase(Window* window, int x, int y, SDL_Color boundaryColor);
    void createBasicStructure(Window* window);
    void updateBlock(int i, int j, int degree, int up, int right, int down, int left);
    void createEdge(Window* window, int i, int j, int side);
    bool BFS();
    void generateMazeRandom(Window* window);

    std::string bgType;
    int dimension, blockSize, dotSize, padding;
    std::vector<std::vector<Block>> maze;
    std::vector<SDL_Rect> boundaryRectPacman;
    std::vector<SDL_Rect> boundaryRectGhost;
    SDL_Texture* texture;
    SDL_Texture* pacWallHoriz;
    SDL_Texture* ghostWallHoriz;
    SDL_Texture* pacWallVert;
    SDL_Texture* ghostWallVert;
};   



