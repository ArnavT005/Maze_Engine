#pragma once

#include <map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <vector>
#include <fstream>
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


void BFS(int destX, int destY, int k, std::vector<std::pair<int, int>> &locations, int startX, int startY, int endX, int endY, std::map<std::pair<int, int>, int> &distance, Maze* maze){
    int size = maze->dimension;
	std::vector<std::vector<int>> m(size, std::vector<int>(size, -1));
    if(destX < 0 || destX >= size || destY < 0 || destY >= size){
        return;
    }
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            m[i][j] = -1;
        }
    }
    m[destX][destY] = 0;
    std::vector<std::vector<int>> que;
    que.push_back(std::vector<int> {destX, destY});
    while(!que.empty()){
        std::vector<int> pt = que[0];
        que.erase(que.begin());
        if(maze->maze[pt[0]][pt[1]].up != ALL_DENIED && maze->maze[pt[0]][pt[1]].up != GHOST_DENIED && pt[0] > 0 && m[pt[0]-1][pt[1]] == -1){
            m[pt[0]-1][pt[1]] = m[pt[0]][pt[1]] + 1;
            que.push_back(std::vector<int> {pt[0]-1, pt[1]});
        }
        if(maze->maze[pt[0]][pt[1]].down != ALL_DENIED && maze->maze[pt[0]][pt[1]].down != GHOST_DENIED && pt[0] < size - 1 && m[pt[0]+1][pt[1]] == -1){
            m[pt[0]+1][pt[1]] = m[pt[0]][pt[1]] + 1;
            que.push_back(std::vector<int> {pt[0]+1, pt[1]});
        }
        if(maze->maze[pt[0]][pt[1]].right != ALL_DENIED && maze->maze[pt[0]][pt[1]].right != GHOST_DENIED && pt[1] < size - 1 && m[pt[0]][pt[1]+1] == -1){
            m[pt[0]][pt[1]+1] = m[pt[0]][pt[1]] + 1;
            que.push_back(std::vector<int> {pt[0], pt[1]+1});
        }
        if(maze->maze[pt[0]][pt[1]].left != ALL_DENIED && maze->maze[pt[0]][pt[1]].left != GHOST_DENIED && pt[1] > 0 && m[pt[0]][pt[1]-1] == -1){
            m[pt[0]][pt[1]-1] = m[pt[0]][pt[1]] + 1;
            que.push_back(std::vector<int> {pt[0], pt[1]-1});
        }
    }
	for(int i = 0; i < 6; i++){
		distance.insert({make_pair(i,k), m[locations[i].first][locations[i].second]});
		distance.insert({make_pair(k,i), m[locations[i].first][locations[i].second]});
	}
	distance.insert({make_pair(k,-1), m[startX][startY]});
	distance.insert({make_pair(-1,k), m[startX][startY]});
	distance.insert({make_pair(k,6), m[endX][endY]});
	distance.insert({make_pair(6,k), m[endX][endY]});

}

std::vector<std::vector<int>> findPermutations(int n){
	std::vector<std::vector<int>> list = {{0}};
	for(int i = 1; i < n; i++){
		std::vector<std::vector<int>> temp ={};
		for(int j = 0; j < list.size(); j++){
			for(int k = 0; k < list[j].size() + 1; k++){
				int flag = 0; std::vector<int> permute = {};
				for(int m = 0; m < list[j].size() + 1; m++){
					if(m == k){ permute.push_back(i); flag = 1;}
					else{permute.push_back(list[j][m - flag]);}
				}
				temp.push_back(permute);
			}
		}
		list = temp;
	}
	return list;
}

int findDistance(std::vector<int> &v, map<std::pair<int, int>, int> &distance){
	int ans = 0;
    int i = 0;
	for(; i < v.size()-1; i++){
		auto iter =  distance.find(std::make_pair(v[i],v[i+1]));
		ans+=iter->second;
	}
	auto iter =  distance.find(std::make_pair(v[0],-1));
	ans+=iter->second;
	iter =  distance.find(std::make_pair(v[i],6));
	ans+=iter->second;
	return ans;
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

    ofstream fout("Analysis.csv");
    fout << "Optimal,Experimental\n";
    int counter = 0;

    while(counter < 100) {

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
    	//find all distances using BFS
    	map<pair<int, int>, int> distance;
    	for(int k = 0; k < 6; k++){
    		BFS(locations[k].first, locations[k].second, k, locations, startX, startY, endX, endY, distance, &maze);
    	}
    	vector<vector<int>> allPermutations = findPermutations(6);
    	//find optimal distance
    	int optimalDist = 9999;
        std::vector<int> optimalPerm;
    	for(int i = 0; i < allPermutations.size(); i++){
            int d = findDistance(allPermutations[i], distance);
            if(optimalDist >  d) {
                optimalDist = d;
                optimalPerm = allPermutations[i];
            }

    	}
    	fout << optimalDist << ",";
        // for(int i = 0; i < 6; i ++) {
        //     std::cout << locations[optimalPerm[i]].first << " " << locations[optimalPerm[i]].second << "\n";
        // }
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

        bool quit = false;

        while(!quit && !ghost.end) {
            while(SDL_PollEvent(&event)) {
                if(event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                    quit = true;
                    break;
                }
            } 
            ghost.update(&window);
            ghost.move();
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
        }
        fout << ghost.distance << "\n";
        counter ++;
    }  
    fout.close();  
    window.free();
    close();
}
