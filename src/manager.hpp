#pragma once


#include <vector>
#include <random>
#include <chrono>
#include "eatable.hpp"
#include "portal.hpp"
#include "window.hpp"


class Manager {

	public:
	Manager(Maze* maze);
	void generateEatables(Window* window, unsigned int SEED);
    void generatePortals(Window* window);
    void updatePortals();
    void renderPortals(Window* window);
    void checkIfTeleport(Pacman* pac);
    void freePortals();
	
	Maze* maze;
	std::vector<Eatable> eatables;
    std::vector<Portal> portals;

};


Manager::Manager(Maze* maze) {
	this->maze = maze;
	eatables.clear();
    portals.clear();
}



void Manager::generateEatables(Window* window, unsigned int SEED) {

    std::mt19937 generator(SEED);
    Eatable item;
    int num = 0;
    for(int i = 0; i < maze->dimension; i ++) {
        for(int j = 0; j < maze->dimension / 2; j ++) {
            if(i == 0 && j == 0 || maze->dimension / 2 - 2 <= i && maze->dimension / 2 + 1 >= i && maze->dimension / 2 - 2 <= j) {
                continue;
            }
            else {
                SDL_Point point = maze->getBlockScreenCoordinate(i, j);
                SDL_Point dot = maze->getDotScreenCoordinate(maze->dimension + 1 - i, maze->dimension + 1 -j);
                int randomNumber = generator() % 100;
                if(randomNumber < 98 || num >= 8) {
                    // create 10 point item
                    item.setValues(point.x + 15, point.y + 15, 15, 15, TEN_POINT, window);
                    eatables.push_back(item);

                    // mirror eatable
                    item.setValues(dot.x - 30, dot.y - 30, 15, 15, TEN_POINT, window);
                    eatables.push_back(item);

                }
                else {
                    // create 50 point item
                    item.setValues(point.x + 7, point.y + 7, 31, 31, FIFTY_POINT, window);
                    eatables.push_back(item);

                    // mirror eatable
                    item.setValues(dot.x - 38, dot.y -38, 31, 31, FIFTY_POINT, window);
                    eatables.push_back(item);
                    num ++;
                }
            }   
        }
    }
}


void Manager::generatePortals(Window* window) {
    SDL_Point p1 = maze->getBlockScreenCoordinate(maze->dimension / 2, 0);
    SDL_Point p2 = maze->getBlockScreenCoordinate(maze->dimension - 1, maze->dimension / 2);
    SDL_Point p3 = maze->getBlockScreenCoordinate(maze->dimension / 2 - 1, maze->dimension - 1);
    SDL_Point p4 = maze->getBlockScreenCoordinate(0, maze->dimension / 2 - 1);

    portals.push_back(Portal(p1.x, p1.y, window));
    portals.push_back(Portal(p2.x, p2.y, window));
    portals.push_back(Portal(p3.x, p3.y, window));
    portals.push_back(Portal(p4.x, p4.y, window));
}

void Manager::updatePortals() {
    int size = portals.size();
    for(int i = 0; i < size; i ++) {
        portals[i].updatePortal();
    }
}

void Manager::renderPortals(Window* window) {
    int size = portals.size();
    for(int i = 0; i < size; i ++) {
        portals[i].render(window);
    }
}

void Manager::checkIfTeleport(Pacman* pac) {
    int size = portals.size();
    SDL_Rect rect;
    int num = 0;
    for(int i = 0; i < size; i ++) {
        if(portals[i].isActive)
            num ++;
    }
    if(num < 2)
        return;
    for(int i = 0; i < size; i ++) {
        if(portals[i].isActive) {
            rect.x = portals[i].screenX;
            rect.y = portals[i].screenY;
            rect.w = rect.h = 45;
            if(pac->collisionDetectorCircle(&pac->colliderSphere, &rect) && !pac->isDead) {
                int portalNum = rand() % size;
                while(portalNum == i || !portals[portalNum].isActive) {
                    portalNum = rand() % size;
                }
                pac->screenX = portals[portalNum].screenX;
                pac->screenY = portals[portalNum].screenY;
                pac->colliderBox.x = pac->screenX;
                pac->colliderBox.y = pac->screenY;
                pac->colliderSphere.center.x = pac->parryCircle.center.x = pac->screenX + BOX_WIDTH / 2;
                pac->colliderSphere.center.y = pac->parryCircle.center.y = pac->screenY + BOX_HEIGHT / 2;
                Mix_PlayChannel(16, teleport, 0);
                portals[portalNum].closePortal();
                portals[i].closePortal();
            }    
        }
        else {
            continue;
        }
    }
}

void Manager::freePortals() {
    int size = portals.size();
    for(int i = 0; i < size; i ++) {
        portals[i].free();
    }
}