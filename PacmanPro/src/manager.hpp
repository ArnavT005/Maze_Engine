#pragma once


#include <vector>
#include <random>
#include <chrono>
#include "eatable.hpp"
#include "portal.hpp"
#include "window.hpp"


class Manager {

	public:
	Manager(Maze* maze, unsigned int SEED);
	void generateEatables(Window* window);
    void generatePortals(Window* window);
    void updatePortals();
    void renderPortals(Window* window);
    void checkIfTeleport(Pacman* pac);
    void freeEatables();
    void freePortals();
	
	Maze* maze;
	std::vector<Eatable> eatables;
    std::vector<Portal> portals;
    std::mt19937 generator;

};