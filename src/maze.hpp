#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include "block.hpp"
#include "window.hpp"


class Maze {
    
    public:
    Maze();
    Maze(int dimension, int blockSize, int dotSize, int padding = 25);
    void setMazeBlock(int i, int j, int degree);
    Block getMazeBlock(int i, int j);
    SDL_Point getBlockScreenCoordinate(int i, int j);
    SDL_Point getDotScreenCoordinate(int i, int j);
    SDL_Point screenToDotCoordinate(int x, int y);
    void createBase(Window* window, int x, int y, SDL_Color boundaryColor);
    void createBasicStructure(Window* window);
    void updateBlock(int i, int j, int degree, int up, int right, int down, int left);
    void createEdge(Window* window, int i, int j, int side);
    //void generateMazeRandom(Window* window);

    int dimension, blockSize, dotSize, padding;
    std::vector<std::vector<Block>> maze;
};

Maze::Maze() {
    dimension = 25;
    blockSize = 31;
    dotSize = 7;
    padding = 25;
    maze.resize(dimension);
    int i, j;
    for(i = 0; i < dimension; i ++) {
        maze[i].resize(dimension);
    }
    for(i = 0; i < dimension; i ++) {
        for(j = 0; j < dimension; j ++) {
            maze[i][j].degree = 4;
            maze[i][j].up = ACCESS_ALLOWED;
            maze[i][j].right = ACCESS_ALLOWED;
            maze[i][j].down = ACCESS_ALLOWED;
            maze[i][j].left = ACCESS_ALLOWED;
        }
    }
}

Maze::Maze(int dimension, int blockSize, int dotSize, int padding) {
    this->dimension = dimension;
    this->blockSize = blockSize;
    this->dotSize = dotSize;
    this->padding = padding;
    maze.resize(dimension);
    int i, j;
    for(i = 0; i < dimension; i ++) {
        maze[i].resize(dimension);
    }
    for(i = 0; i < dimension; i ++) {
        for(j = 0; j < dimension; j ++) {
            maze[i][j].degree = 4;
            maze[i][j].up = ACCESS_ALLOWED;
            maze[i][j].right = ACCESS_ALLOWED;
            maze[i][j].down = ACCESS_ALLOWED;
            maze[i][j].left = ACCESS_ALLOWED;
            if(i == 0) {
                maze[i][j].up = ALL_DENIED;
                maze[i][j].degree --;
            }
            else if(i == dimension - 1) {
                maze[i][j].down = ALL_DENIED;
                maze[i][j].degree --;
            }
            if(j == 0) {
                maze[i][j].left = ALL_DENIED;
                maze[i][j].degree --;
            }
            else if(j == dimension - 1) {
                maze[i][j].right = ALL_DENIED;
                maze[i][j].degree --;
            }
        }
    }
}


SDL_Point Maze::getBlockScreenCoordinate(int i, int j) {
    SDL_Point point = {-1, -1};
    if(i < dimension && j < dimension && i >= 0 && j >= 0) {
        point.x = padding + j * dotSize + (j - 1) * blockSize;
        point.y = padding + i * dotSize + (i - 1) * blockSize;
        return point;
    }
    else 
        return point;
}

SDL_Point Maze::getDotScreenCoordinate(int i, int j) {
    SDL_Point point = {-1, -1};
    if(i < dimension + 1 && j < dimension + 1 && i >= 0 && j >= 0) {
        point.x = padding + (j - 1) * (blockSize + dotSize);
        point.y = padding + (i - 1) * (blockSize + dotSize);
        return point;
    }
    else 
        return point;
}

SDL_Point Maze::screenToDotCoordinate(int x, int y) {
    SDL_Point point = {(y - padding) / (blockSize + dotSize) + 1, (x - padding) / (blockSize + dotSize) + 1};
    return point;
}

void Maze::updateBlock(int i, int j, int degree, int up, int right, int down, int left) {
    maze[i][j].degree = degree;
    maze[i][j].up = up;
    maze[i][j].right = right;
    maze[i][j].down = down;
    maze[i][j].left = left;
}

void Maze::createBase(Window *window, int x, int y, SDL_Color boundaryColor) {
    
    
    int windowWidth = 0, windowHeight = 0;

    SDL_GetWindowSize(window->getWindow(), &windowWidth, &windowHeight);

    SDL_Rect horizontalLayer = {x, y, windowWidth, padding};
    SDL_Rect verticalLayer = {x, y, padding, windowHeight};

    window->renderRect(&horizontalLayer, boundaryColor);
    window->renderRect(&verticalLayer, boundaryColor);

    horizontalLayer.y += windowHeight - padding;
    verticalLayer.x += windowWidth - padding;

    window->renderRect(&horizontalLayer, boundaryColor);
    window->renderRect(&verticalLayer, boundaryColor);

    SDL_Point startPoint = getDotScreenCoordinate(1, 1);

    SDL_Rect dot = {startPoint.x, startPoint.y, dotSize, dotSize},
             vEdge = {startPoint.x, startPoint.y + dotSize, dotSize, blockSize},
             hEdge = {startPoint.x + dotSize, startPoint.y, blockSize, dotSize},
             block = {startPoint.x + dotSize, startPoint.y + dotSize, blockSize, blockSize};

    SDL_Color dotInnerColor = {0x02, 0xc9, 0x70, 0x40},   // blue
              dotOuterColor = {0xFF, 0x00, 0x00, 0xFF},   // red
              edgeColor = {0x00, 0x70, 0x00, 0xFF},       // dark green
              blockColor = {0x00, 0xFF, 0x00, 0xFF};      // green

    window->renderRect(&dot, dotOuterColor);
    int offset = dotSize + blockSize;

    for(int i = 0; i < dimension; i ++) {
        window->renderRect(&hEdge, edgeColor);
        dot.x += offset;
        window->renderRect(&dot, dotOuterColor);
        hEdge.x += offset;
    }

    for(int i = 0; i < dimension; i ++) {
        window->renderRect(&vEdge, edgeColor);
        block.x = vEdge.x + dotSize;
        block.y = vEdge.y;
        for(int j = 0; j < dimension; j ++) {
            window->renderRect(&block, blockColor);
            vEdge.x += offset;
            if(j < dimension - 1) {
                window->renderRect(&vEdge, blockColor);
            }
            else {
                window->renderRect(&vEdge, edgeColor);
            }
            block.x += offset;
        }
        dot.x = startPoint.x;
        dot.y += offset;
        window->renderRect(&dot, dotOuterColor);
        hEdge.x = dot.x + dot.w;
        hEdge.y = dot.y;
        if(i < dimension - 1) {
            for(int j = 0; j < dimension; j ++) {
                window->renderRect(&hEdge, blockColor);
                dot.x += offset;
                if(j < dimension - 1) {
                    window->renderRect(&dot, dotInnerColor);
                }
                else {
                    window->renderRect(&dot, dotOuterColor);
                }
                hEdge.x += offset;
            }
        }   
        else {
            for(int j = 0; j < dimension; j ++) {
                window->renderRect(&hEdge, edgeColor);
                dot.x += offset;
                window->renderRect(&dot, dotOuterColor);
                hEdge.x += offset;
            }

        }
        vEdge.x = startPoint.x;
        vEdge.y += offset;
    }
}


void Maze::createBasicStructure(Window* window) {
    int offset = dotSize + blockSize;

    // create ghost house
    SDL_Point startPoint = getDotScreenCoordinate(dimension / 2 - 1, dimension / 2 -1);
    SDL_Rect dot = {startPoint.x, startPoint.y, dotSize, dotSize},
             vEdge = {startPoint.x, startPoint.y + dotSize, dotSize, blockSize},
             hEdge = {startPoint.x + dotSize, startPoint.y, blockSize, dotSize},
             block = {startPoint.x + dotSize, startPoint.y + dotSize, blockSize, blockSize};
    SDL_Color dotColor = {0xFF, 0x00, 0x00, 0xFF},   // blue 
              edgeColor = {0x00, 0x70, 0x00, 0xFF},  // dark green
              blockColor = {0x30, 0x70, 0x00, 0xFF},  // mix
              doorColor = {0xFF, 0x80, 0xFF, 0xFF};  // light magenta
    // create top left square
    window->renderRect(&dot, dotColor);
    window->renderRect(&hEdge, edgeColor);
    window->renderRect(&vEdge, edgeColor);
    window->renderRect(&block, blockColor);
    dot.y += offset;
    hEdge.y += offset;
    vEdge.x += offset;
    window->renderRect(&dot, dotColor);
    window->renderRect(&hEdge, edgeColor);
    window->renderRect(&vEdge, edgeColor);
    dot.x += offset;
    window->renderRect(&dot, dotColor);
    dot.y -= offset;
    window->renderRect(&dot, dotColor);

    hEdge.x += offset;
    hEdge.y -= offset;
    window->renderRect(&hEdge, doorColor);
    dot.x += offset;
    hEdge.x += offset;
    window->renderRect(&dot, doorColor);
    window->renderRect(&hEdge, doorColor);

    dot.x += offset;
    hEdge.x += offset;
    vEdge.x += 2 * offset;
    block.x += 3 * offset;
    // create top right square
    window->renderRect(&dot, dotColor);
    window->renderRect(&hEdge, edgeColor);
    window->renderRect(&vEdge, edgeColor);
    window->renderRect(&block, blockColor);
    dot.y += offset;
    hEdge.y += offset;
    vEdge.x += offset;
    window->renderRect(&dot, dotColor);
    window->renderRect(&hEdge, edgeColor);
    window->renderRect(&vEdge, edgeColor);
    dot.x += offset;
    window->renderRect(&dot, dotColor);
    dot.y -= offset;
    window->renderRect(&dot, dotColor);

    vEdge.y += offset;
    window->renderRect(&vEdge, doorColor);
    dot.y += 2 * offset;
    vEdge.y += offset;
    window->renderRect(&dot, doorColor);
    window->renderRect(&vEdge, doorColor);

    dot.x -= offset;
    dot.y += offset;
    hEdge.y += 2 * offset;
    vEdge.y += offset;
    vEdge.x -= offset;
    block.y += 3 * offset;
    // create bottom right square
    window->renderRect(&dot, dotColor);
    window->renderRect(&hEdge, edgeColor);
    window->renderRect(&vEdge, edgeColor);
    window->renderRect(&block, blockColor);
    dot.y += offset;
    hEdge.y += offset;
    vEdge.x += offset;
    window->renderRect(&dot, dotColor);
    window->renderRect(&hEdge, edgeColor);
    window->renderRect(&vEdge, edgeColor);
    dot.x += offset;
    window->renderRect(&dot, dotColor);
    dot.y -= offset;
    window->renderRect(&dot, dotColor);

    hEdge.x -= offset;
    window->renderRect(&hEdge, doorColor);
    dot.y += offset;
    dot.x -= 2 * offset;
    hEdge.x -= offset;
    window->renderRect(&dot, doorColor);
    window->renderRect(&hEdge, doorColor);

    dot.x -= 2 * offset;
    dot.y -= offset;
    hEdge.x -= offset;
    hEdge.y -= offset;
    block.x -= 3 * offset;
    vEdge.x -= 4 * offset;
    // create bottom left square
    window->renderRect(&dot, dotColor);
    window->renderRect(&hEdge, edgeColor);
    window->renderRect(&vEdge, edgeColor);
    window->renderRect(&block, blockColor);
    dot.y += offset;
    hEdge.y += offset;
    vEdge.x += offset;
    window->renderRect(&dot, dotColor);
    window->renderRect(&hEdge, edgeColor);
    window->renderRect(&vEdge, edgeColor);
    dot.x += offset;
    window->renderRect(&dot, dotColor);
    dot.y -= offset;
    window->renderRect(&dot, dotColor);

    vEdge.x -= offset;
    vEdge.y -= offset;
    window->renderRect(&vEdge, doorColor);
    dot.y -= offset;
    dot.x -= offset;
    vEdge.y -= offset;
    window->renderRect(&dot, doorColor);
    window->renderRect(&vEdge, doorColor);

    updateBlock(dimension / 2 - 2, dimension / 2 - 2, 0, 2, 2, 2, 2);
    updateBlock(dimension / 2 + 1, dimension / 2 - 2, 0, 2, 2, 2, 2);
    updateBlock(dimension / 2 - 2, dimension / 2 + 1, 0, 2, 2, 2, 2);
    updateBlock(dimension / 2 + 1, dimension / 2 + 1, 0, 2, 2, 2, 2);

    updateBlock(dimension / 2 - 3, dimension / 2 - 2, 3, 0, 0, 2, 0);
    updateBlock(dimension / 2 - 3, dimension / 2 - 1, 3, 0, 0, 1, 0);
    updateBlock(dimension / 2 - 3, dimension / 2, 3, 0, 0, 1, 0);
    updateBlock(dimension / 2 - 2, dimension / 2 - 1, 3, 1, 0, 0, 0);
    updateBlock(dimension / 2 - 2, dimension / 2, 3, 1, 0, 0, 0);
    updateBlock(dimension / 2 - 3, dimension / 2 + 1, 3, 0, 0, 2, 0);

    updateBlock(dimension / 2 + 2, dimension / 2 - 2, 3, 2, 0, 0, 0);
    updateBlock(dimension / 2 + 2, dimension / 2 - 1, 3, 1, 0, 0, 0);
    updateBlock(dimension / 2 + 2, dimension / 2, 3, 1, 0, 0, 0);
    updateBlock(dimension / 2 + 1, dimension / 2 - 1, 3, 0, 0, 1, 0);
    updateBlock(dimension / 2 + 1, dimension / 2, 3, 0, 0, 1, 0);
    updateBlock(dimension / 2 + 2, dimension / 2 + 1, 3, 2, 0, 0, 0);

    updateBlock(dimension / 2 - 2, dimension / 2 - 3, 3, 0, 2, 0, 0);
    updateBlock(dimension / 2 - 1, dimension / 2 - 3, 3, 0, 1, 0, 0);
    updateBlock(dimension / 2, dimension / 2 - 3, 3, 0, 1, 0, 0);
    updateBlock(dimension / 2 - 1, dimension / 2 - 2, 3, 0, 0, 0, 1);
    updateBlock(dimension / 2, dimension / 2 - 2, 3, 0, 0, 0, 1);
    updateBlock(dimension / 2 + 1, dimension / 2 - 3, 3, 0, 2, 0, 0);

    updateBlock(dimension / 2 - 2, dimension / 2 + 2, 3, 0, 0, 0, 2);
    updateBlock(dimension / 2 - 1, dimension / 2 + 2, 3, 0, 0, 0, 1);
    updateBlock(dimension / 2, dimension / 2 + 2, 3, 0, 0, 0, 1);
    updateBlock(dimension / 2 - 1, dimension / 2 + 1, 3, 0, 1, 0, 0);
    updateBlock(dimension / 2, dimension / 2 + 1, 3, 0, 1, 0, 0);
    updateBlock(dimension / 2 + 1, dimension / 2 + 2, 3, 0, 0, 0, 2);


    startPoint = getDotScreenCoordinate(1, dimension / 2 + 1);
    vEdge.x = startPoint.x;
    vEdge.y = startPoint.y + dotSize;
    dot.x = startPoint.x;
    dot.y = startPoint.y + offset;
    for(int i = 0; i < 5; i ++) {
        window->renderRect(&vEdge, edgeColor);
        window->renderRect(&dot, dotColor);
        if(i == 0) {
            updateBlock(i, dimension / 2 - 1, 2, 2, 2, 0, 0);
            updateBlock(i, dimension / 2, 2, 2, 0, 0, 2);
        }
        else {
            updateBlock(i, dimension / 2 - 1, 3, 0, 2, 0, 0);
            updateBlock(i, dimension / 2, 3, 0, 0, 0, 2);    
        }
        vEdge.y += offset;
        dot.y += offset;
    }

    startPoint = getDotScreenCoordinate(dimension - 4, dimension / 2 + 1);
    vEdge.y = startPoint.y + dotSize;
    dot.y = startPoint.y;
    for(int i = 0; i < 5; i ++) {
        window->renderRect(&vEdge, edgeColor);
        window->renderRect(&dot, dotColor);
        if(i == 4) {
            updateBlock(dimension - 1, dimension / 2 - 1, 2, 0, 2, 2, 0);
            updateBlock(dimension - 1, dimension / 2, 2, 0, 0, 2, 2);
        }
        else {
            updateBlock(dimension - 5 + i, dimension / 2 - 1, 3, 0, 2, 0, 0);
            updateBlock(dimension - 5 + i, dimension / 2, 3, 0, 0, 0, 2);    
        }
        vEdge.y += offset;
        dot.y += offset;
    }

    startPoint = getDotScreenCoordinate(dimension / 2 + 1, 1);
    hEdge.x = startPoint.x + dotSize;
    hEdge.y = startPoint.y;
    dot.x = startPoint.x + offset;
    dot.y = startPoint.y;
    for(int i = 0; i < 5; i ++) {
        window->renderRect(&hEdge, edgeColor);
        window->renderRect(&dot, dotColor);
        if(i == 0) {
            updateBlock(dimension / 2 - 1, i, 2, 0, 0, 2, 2);
            updateBlock(dimension / 2, i, 2, 2, 0, 0, 2);
        }
        else {
            updateBlock(dimension / 2 - 1, i, 3, 0, 0, 2, 0);
            updateBlock(dimension / 2, i, 3, 2, 0, 0, 0);    
        }
        hEdge.x += offset;
        dot.x += offset;
    }

    startPoint = getDotScreenCoordinate(dimension / 2 + 1, dimension - 4);
    hEdge.x = startPoint.x + dotSize;
    dot.x = startPoint.x;
    for(int i = 0; i < 5; i ++) {
        window->renderRect(&hEdge, edgeColor);
        window->renderRect(&dot, dotColor);
        if(i == 4) {
            updateBlock(dimension / 2 - 1, dimension - 1, 2, 0, 2, 2, 0);
            updateBlock(dimension / 2, dimension - 1, 2, 2, 2, 0, 0);
        }
        else {
            updateBlock(dimension / 2 - 1, dimension - 5 + i, 3, 0, 0, 2, 0);
            updateBlock(dimension / 2, dimension - 5 + i, 3, 2, 0, 0, 0);    
        }
        hEdge.x += offset;
        dot.x += offset;
    }

}


void Maze::createEdge(Window* window, int i, int j, int side) {
    SDL_Point point = getDotScreenCoordinate(i, j);
    SDL_Rect dot = {point.x, point.y, dotSize, dotSize},
             vEdge = {point.x, point.y + dotSize, dotSize, blockSize},
             hEdge = {point.x + dotSize, point.y, blockSize, dotSize};
    SDL_Color dotColor = {0xFF, 0x00, 0x00, 0xFF},  
              edgeColor = {0x00, 0x70, 0x00, 0xFF};

    int offset = dotSize + blockSize;
    window->renderRect(&dot, dotColor);
    switch(side) {
        case 0:
            vEdge.y = dot.y - blockSize;
            window->renderRect(&vEdge, edgeColor);
            dot.y -= offset;
            window->renderRect(&dot, dotColor);
            break;
        case 1:
            hEdge.x = dot.x + dotSize;
            window->renderRect(&hEdge, edgeColor);
            dot.x += offset;
            window->renderRect(&dot, dotColor);
            break;
        case 2:
            vEdge.y = dot.y + dotSize;
            window->renderRect(&vEdge, edgeColor);
            dot.y += offset;
            window->renderRect(&dot, dotColor);
            break;
        case 3:
            hEdge.x = dot.x - blockSize;
            window->renderRect(&hEdge, edgeColor);
            dot.y -= offset;
            window->renderRect(&dot, dotColor);
            break;
    }
}


