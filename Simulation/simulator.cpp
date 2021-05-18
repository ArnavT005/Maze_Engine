#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <vector>
#include "window.hpp"
#include "maze.hpp"
#include "ghost.hpp"
#include "eatable.hpp"

bool SDL_init() {
    bool success = true;
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cout << "SDL unable to initialize! SDL Error: " << SDL_GetError() << "\n";
        success = false;
    }
    else {
        if(IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
            std::cout << "IMG unable to initialize! SDL_Image Error: " << IMG_GetError() << "\n";
            success = false;
        }
        if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            std::cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << "\n";
            success = false;
        }
    }
    return success;
}

void close() {
    IMG_Quit();
    SDL_Quit();
}

bool isSame(int startX, int startY, int endX, int endY, std::vector<std::pair<int, int>> locations, int i) {
	if(locations[i].first == startX && locations[i].second == startY)
		return true;
	if(locations[i].first == endX && locations[i].second == endY)
		return true;
	int size = locations.size();
	for(int j = 0; j < size; j ++) {
		if(i != j) {
			if(locations[i].first == locations[j].first && locations[i].second == locations[j].second)
				return true;
		}
	}
	return false;
}

int main(int argc, char** argv) {
    
    if(!SDL_init()) {
        return 0;
    }
  
    Window window("PLIGHT OF PINKY", 1025, 1025);
    if(!window.getSuccess()) {
        close();
        return 0;
    }

    srand(time(0));

    std::string bgType = std::to_string((rand() % 6 + 1));
    Maze maze(16, 45, 15, 25);
    maze.bgType = bgType;
    SDL_Color boundaryColor = {0xFF, 0x00, 0x00, 0xFF};
    SDL_Color startPointColor = {0x00, 0xFF, 0x00, 0xFF};
    SDL_Color endPointColor = {0xFF, 0x00, 0x00, 0xFF};
    SDL_Color eatableColor = {0x00, 0x00, 0xFF, 0xFF};
    SDL_Texture* background = SDL_CreateTexture(window.getRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1025, 1025);
   
    SDL_Rect bg = {0, 0, 1025, 1025};
    SDL_Rect scr = {1025, 0, 380, 1025};
    SDL_SetTextureBlendMode(background, SDL_BLENDMODE_BLEND);
    maze.loadTexture(&window);

    window.setRenderTarget(background);
   
    maze.createBase(&window, 0, 0, boundaryColor);
    maze.generateMazeRandom(&window);

    int i = 0;
    while(!maze.BFS() && i < 16) {
        maze.refreshMaze(&window);
        maze.createBase(&window, 0, 0, boundaryColor);
        maze.generateMazeRandom(&window);
        i ++;
    }

    // generate starting and ending positions
    int startX = rand() % 16;
    int startY = rand() % 16;
    int endX = rand() % 16;
    int endY = rand() % 16;

    maze.colourBlock(startX, startY, startPointColor, &window, false);
    if(endX == startX && endY == startY) {
    	maze.colourBlock(endX, endY, endPointColor, &window, true);
	}
	else {
    	maze.colourBlock(endX, endY, endPointColor, &window, false);		
	}

    // choose 6 locations for eatables
    std::vector<std::pair<int, int>> locations(6, std::make_pair(-1, -1));
    for(int i = 0; i < 6; i ++) {
    	locations[i].first = rand() % 16;
    	locations[i].second = rand() % 16;
    	while(isSame(startX, startY, endX, endY, locations, i)) {
    		locations[i].first = rand() % 16;
    		locations[i].second = rand() % 16;
    	}
    	maze.colourBlock(locations[i].first, locations[i].second, eatableColor, &window, false);
    }

    window.setRenderTarget(NULL);

    // create ghost
    Ghost ghost(&maze, &window, startX, startY);

    Eatable item;
    // tell locations to ghost
    for(int i = 0; i < 6; i ++) {
    	ghost.dest.push_back(locations[i]);
    	SDL_Point point = maze.getBlockScreenCoordinate(locations[i].first, locations[i].second);
    	item.setValues(point.x + 7, point.y + 7, 31, 31, FIFTY_POINT, &window);
    	ghost.large.push_back(item);
    }
    ghost.dest.push_back(std::make_pair(endX, endY));

    SDL_Event event;
    Mix_Chunk* startGame = Mix_LoadWAV("../music/intro.wav");
    if(startGame == NULL) {
        std::cout << "Failed to load music! SDL_Mixer Error: " << Mix_GetError() << "\n";
    }
    else {
        if(Mix_PlayChannel(0, startGame, 0) == -1) {
            std::cout << "Error in playing music! SDL_Mixer Error: " << Mix_GetError() << "\n";
        }    
    }

    bool quit = false;
    Uint32 start = SDL_GetTicks();

    while(!quit) {
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                quit = true;
                break;
            }
        } 
        if(SDL_GetTicks() - start >= 4000) {
            ghost.update(&window);
            ghost.move();
        }    
        window.clearWindow();
        window.renderTexture(background, NULL, &bg);
	    if(ghost.targetAcquired) {
	        for(int i = 0; i < ghost.small.size(); i ++) {
	        	ghost.small[i].render(&window);
	        }
	    }
        for(int i = 0; i < ghost.large.size(); i ++) {
        	ghost.large[i].render(&window);
        }
        ghost.render(&window);
        window.updateWindow(); 
        SDL_Delay(15);
    }

    window.free();
    close();
}