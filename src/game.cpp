#include "window.hpp"
#include "maze.hpp"


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
   
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    std::cout << DM.w << " " << DM.h << "\n";
   
   
    window.clearWindow();
    maze.createBase(&window, 0, 0, boundaryColor);
    maze.createBasicStructure(&window);
    maze.generateMazeRandom(&window);
    window.updateWindow();

    bool quit = false;
    SDL_Event event;
    
    while(!quit) {
        while(SDL_PollEvent(& event)) {
            if(event.type == SDL_KEYDOWN) {
                quit = true;
            }
        }
    }
    window.free();
    close();
}
