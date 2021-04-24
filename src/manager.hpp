#pragma once


#include <vector>
#include <random>
#include <chrono>
#include "eatable.hpp"
#include "window.hpp"

class Manager {

	public:
	Manager(Maze* maze);
	void generateEatables(Window* window);
	
	Maze* maze;
	std::vector<Eatable> eatables;

};


Manager::Manager(Maze* maze) {
	this->maze = maze;
	eatables.clear();
}



void Manager::generateEatables(Window* window) {

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 generator(seed);
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