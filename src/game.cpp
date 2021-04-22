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
    return success;
}

void close() {
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
    
    Pacman pac(&maze);
    Ghost g1(&maze, 1, 1);
    Ghost g2(&maze, 2, 1);
    Ghost g3(&maze, 3, 1);
    Ghost g4(&maze, 4, 2);
    
    window.setRenderTarget(NULL);
    window.clearWindow();

    bool quit = false;
    SDL_Event event;
    
    while(!quit) {
        int i = 0;
    	bool flagCheck = true;
        while(SDL_PollEvent(& event)) {
            if(event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) ) {
                quit = true;
            }
            pac.handleEvent(event);
            break;
        }
        g1.handleEvent(maze.screenToBlockCoordinate(pac.screenX, pac.screenY), pac.velX, pac.velY);
        g2.handleEvent(maze.screenToBlockCoordinate(pac.screenX, pac.screenY), pac.velX, pac.velY);
        g3.handleEvent(maze.screenToBlockCoordinate(pac.screenX, pac.screenY), pac.velX, pac.velY);
        g4.handleEvent(maze.screenToBlockCoordinate(pac.screenX, pac.screenY), pac.velX, pac.velY);
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
        if (false){
            g1.handleEvent(maze.screenToBlockCoordinate(pac.screenX, pac.screenY), pac.velX, pac.velY);
        	g1.move();
        	window.renderTexture(background);
            pac.render(&window);
            g1.render(&window);
            window.updateWindow();
            //break;
            }
    }
    
    window.free();
    close();
}
