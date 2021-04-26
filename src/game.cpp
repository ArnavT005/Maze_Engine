#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "window.hpp"
#include "maze.hpp"
#include "pacman.hpp"
#include "ghost.hpp"
#include "manager.hpp"

int timeToChangeMode = 10000;
	
bool SDL_init() {
    bool success = true;
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL unable to initialize! SDL Error: " << SDL_GetError() << "\n";
        success = false;
    }
    else {
        if(IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
            std::cout << "IMG unable to initialize! SDL_Image Error: " << IMG_GetError() << "\n";
            success = false;
        }
    }
    return success;
}

void close() {
    IMG_Quit();
    SDL_Quit();
}


void GhostUpdate(Pacman* p1, Pacman* p2, Ghost* g1, Ghost* g2, Ghost*g3, Ghost* g4, Ghost* g5, Ghost* g6, Ghost* g7, Uint32 time) {
    g1->update(p1, p2);
    g2->update(p1, p2);
    g3->update(p1, p2);
    g4->update(p1, p2);
    g1->move();
    g2->move();
    g3->move();
    g4->move();
    g1->checkPacmanCollision(p1);
    g2->checkPacmanCollision(p1);
    g3->checkPacmanCollision(p1);
    g4->checkPacmanCollision(p1);
	g1->checkPacmanCollision(p2);
    g2->checkPacmanCollision(p2);
    g3->checkPacmanCollision(p2);
    g4->checkPacmanCollision(p2);
	if(time > timeToChangeMode){
		g5->update(p2, p1);
		g6->update(p2, p1);
		g7->update(p2, p1);
		g5->move();
		g6->move();
		g7->move();
		g5->checkPacmanCollision(p1);
		g6->checkPacmanCollision(p1);
		g7->checkPacmanCollision(p1);
		g5->checkPacmanCollision(p2);
		g6->checkPacmanCollision(p2);
		g7->checkPacmanCollision(p2);
	}
}   

void RenderElements(Pacman* p1, Pacman* p2, Ghost* g1, Ghost* g2, Ghost* g3, Ghost* g4, Ghost* g5, Ghost* g6, Ghost* g7, Uint32 time, Window* window) {
    p1->render(window);
    p2->render(window);
    g1->render(window);
    g2->render(window);
    g3->render(window);
    g4->render(window);
    if(time >= timeToChangeMode){
        g5->render(window);
        g6->render(window);
        g7->render(window);
    }
}

void switchGhostMode(Ghost* g1, Ghost* g2, Ghost* g3, Ghost* g4) {
    if(g1->mode != 3 && g1->mode != 4) {
        g1->mode = 2;
    }
    else {
        g1->prevMode = 2;
    }
    if(g2->mode != 3 && g2->mode != 4) {
        g2->mode = 2;
    }
    else {
        g2->prevMode = 2;
    }
    if(g3->mode != 3 && g3->mode != 4) {
        g3->mode = 2;
    }
    else {
        g3->prevMode = 2;
    }
    if(g4->mode != 3 && g4->mode != 4) {
        g4->mode = 2;
    }
    else {
        g4->prevMode = 2;
    }
}

int main(int argc, char** argv) {
    if(!SDL_init()) {
        return 0;
    }
    // 1023
    Window window("Maze", 1405, 1025);
    if(!window.getSuccess()) {
        return 0;
    }
    srand(time(0));
    // 20, 35, 13, 25
    Maze maze(16, 45, 15, 25);
    SDL_Color boundaryColor = {0xFF, 0x00, 0x00, 0xFF};
    SDL_Texture* background = SDL_CreateTexture(window.getRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1025, 1025);
    SDL_Rect bg = {0, 0, 1025, 1025};
    SDL_SetTextureBlendMode(background, SDL_BLENDMODE_BLEND);
    maze.loadTexture(&window);

    window.setRenderTarget(background);
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    std::cout << DM.w << " " << DM.h << "\n";
   
    maze.createBase(&window, 0, 0, boundaryColor);
    maze.createBasicStructure(&window);
    maze.generateMazeRandom(&window);


    window.setRenderTarget(NULL);
    
    Pacman p1(&maze, &window, 1);
	Pacman p2(&maze, &window, 2);
    Manager manager(&maze);
    manager.generateEatables(&window);
    manager.generatePortals(&window);
    int numEat = manager.eatables.size();

    for(int k = 0; k < numEat; k ++) {
        manager.eatables[k].setPacman(&p1, &p2);
    }

    Ghost g1(&maze, TYPE_BLINKY, 1, &window);
    Ghost g2(&maze, TYPE_PINKY, 1, &window);
    Ghost g3(&maze, TYPE_INKY, 1, &window);
    Ghost g4(&maze, TYPE_CLYDE, 1, &window);
	Ghost g5;
	Ghost g6;
	Ghost g7(&maze, TYPE_CLYDE, 2, &window);

    window.clearWindow();

    bool quit = false;
    SDL_Event event;
    bool temp;
    Uint32 startTime = SDL_GetTicks();
    bool changedMode = false;
    
    while(!quit) {
        int i = 0;
        temp = false;

        while(SDL_PollEvent(& event)) {
            if(event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) ) {
                quit = true;
            }
            p1.handleEvent(event, SDL_GetKeyboardState(NULL));
            p2.handleEvent(event, SDL_GetKeyboardState(NULL));
            g1.handleEvent(event, &p1, &p2);
            g2.handleEvent(event, &p1, &p2);
            g3.handleEvent(event, &p1, &p2);
            g4.handleEvent(event, &p1, &p2);
            g7.handleEvent(event, &p1, &p2);
            if(changedMode){
                g5.handleEvent(event, &p1, &p2);
                g6.handleEvent(event, &p1, &p2);
            }
        }
        window.clearWindow();
        window.renderTexture(background, NULL, &bg);
        p1.move();
        p2.move();
        for(i = 0; i < numEat; i ++) {
            manager.eatables[i].checkIfEaten(temp);
            manager.eatables[i].render(&window);
        }
        p1.isBuffed = temp;
        p2.isBuffed = temp;
        Uint32 timeNow = SDL_GetTicks() - startTime;
        if( timeNow >= timeToChangeMode && !changedMode) {
            switchGhostMode(&g1, &g2, &g3, &g4);
			g5 = g1;
			g6 = g2;
            changedMode = true;
        }
        GhostUpdate(&p1, &p2, &g1, &g2, &g3, &g4, &g5, &g6, &g7, timeNow);
        manager.updatePortals();
        int preference = rand()%2 + 1;
        if(preference == 1){
		    manager.checkIfTeleport(&p1);
			manager.checkIfTeleport(&p2);
		}
		else if(preference == 2){
			manager.checkIfTeleport(&p2);
			manager.checkIfTeleport(&p1);
		}
        manager.renderPortals(&window);
        RenderElements(&p1, &p2, &g1, &g2, &g3, &g4, &g5, &g6, &g7, timeNow, &window);
        window.updateWindow();
        SDL_Delay(17);
    }
    
    window.free();
    close();
}
