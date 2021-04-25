#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "window.hpp"
#include "maze.hpp"
#include "pacman.hpp"
#include "ghost.hpp"
#include "manager.hpp"


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


int main(int argc, char** argv) {
    if(!SDL_init()) {
        return 0;
    }
    // 1023
    Window window("Maze", 1025, 1025);
    if(!window.getSuccess()) {
        return 0;
    }
    srand(time(0));
    // 20, 35, 13, 25
    Maze maze(16, 45, 15, 25);
    SDL_Color boundaryColor = {0xFF, 0x00, 0x00, 0xFF};
    SDL_Texture* background = SDL_CreateTexture(window.getRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1025, 1025);
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
    
    Pacman pac(&maze, &window);
    Manager manager(&maze);
    manager.generateEatables(&window);
    int numEat = manager.eatables.size();

    for(int k = 0; k < numEat; k ++) {
        manager.eatables[k].setPacman(&pac);
    }

    Ghost g1(&maze, TYPE_BLINKY, 2, &window);
    Ghost g2(&maze, TYPE_PINKY, 1, &window);
    Ghost g3(&maze, TYPE_INKY, 1, &window);
    Ghost g4(&maze, TYPE_CLYDE, 1, &window);


    window.clearWindow();

    bool quit = false;
    SDL_Event event;
    bool temp;
    
    while(!quit) {
        int i = 0;
        temp = false;

        while(SDL_PollEvent(& event)) {
            if(event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) ) {
                quit = true;
            }
            pac.handleEvent(event, SDL_GetKeyboardState(NULL));
            g1.handleEvent(event, &pac);
            g2.handleEvent(event, &pac);
            g3.handleEvent(event, &pac);
            g4.handleEvent(event, &pac);

           // break;
        }
        window.renderTexture(background);
        pac.move();
        for(i = 0; i < numEat; i ++) {
            manager.eatables[i].checkIfEaten(temp);
            manager.eatables[i].render(&window);
        }
        pac.isBuffed = temp;
        g1.update(&pac);
        g2.update(&pac);
        g3.update(&pac);
        g4.update(&pac);
        g1.move();
        g2.move();
        g3.move();
        g4.move();
        g1.checkPacmanCollision(&pac);
        g2.checkPacmanCollision(&pac);
        g3.checkPacmanCollision(&pac);
        g4.checkPacmanCollision(&pac);
        pac.render(&window);
        g1.render(&window);
        g2.render(&window);
        g3.render(&window);
        g4.render(&window);
        window.updateWindow();
        SDL_Delay(20);
    }
    
    window.free();
    close();
}
