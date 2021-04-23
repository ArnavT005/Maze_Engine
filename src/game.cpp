#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "window.hpp"
#include "maze.hpp"
#include "pacman.hpp"
#include "ghost.hpp"


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
    SDL_Color boundaryColor = {0x00, 0x00, 0x00, 0xFF};
    SDL_Texture* background = SDL_CreateTexture(window.getRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1025, 1025);

    window.setRenderTarget(background);
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    std::cout << DM.w << " " << DM.h << "\n";
   
    maze.createBase(&window, 0, 0, boundaryColor);
    maze.createBasicStructure(&window);
    maze.generateMazeRandom(&window);

    window.setRenderTarget(NULL);
    
    Pacman pac(&maze, &window);
    Ghost g1(&maze, TYPE_BLINKY, 1);
    Ghost g2(&maze, TYPE_PINKY, 1);
    Ghost g3(&maze, TYPE_INKY, 1);
    Ghost g4(&maze, TYPE_CLYDE, 1);
    g1.loadTexture(&window);
    g2.loadTexture(&window);
    g3.loadTexture(&window);
    g4.loadTexture(&window);


    window.clearWindow();

    bool quit = false;
    SDL_Event event;
    
    while(!quit) {
        int i = 0;
        while(SDL_PollEvent(& event)) {
            if(event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) ) {
                quit = true;
            }
            pac.handleEvent(event);
            break;
        }
        g1.handleEvent(&pac);
        g2.handleEvent(&pac);
        g3.handleEvent(&pac);
        g4.handleEvent(&pac);
        pac.move();
        g1.move();
        g2.move();
        g3.move();
        g4.move();
        window.renderTexture(background);
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
