#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "window.hpp"
#include "maze.hpp"
#include "pacman.hpp"
#include "ghost.hpp"
#include "manager.hpp"
#include "scoreboard.hpp"

int timeToChangeMode = 10000;
int timeToRandomize = 20000;
bool changedMode1 = false, changedMode2 = false; 

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
        if(TTF_Init() < 0) {
            std::cout << "TTF unable to initialize! SDL_ttf Error: " << TTF_GetError() << "\n";
            success = false;
        }
    }
    return success;
}

void close() {
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}



void ScoreUpdate(Scoreboard* board, Pacman* p1, Pacman* p2, Window* window) {
    int timeSeconds = (SDL_GetTicks() - board->start) / 1000;
    int timeMinutes = timeSeconds / 60;
    timeSeconds -= 60 * timeMinutes;
    board->timeText.str("");
    if(timeMinutes < 10) {
        if(timeSeconds < 10) {
            board->timeText << "Time- 0" << timeMinutes << ":0" << timeSeconds;
        }
        else {
            board->timeText << "Time- 0" << timeMinutes << ":" << timeSeconds;
        }
    }
    else {
        if(timeSeconds < 10) {
            board->timeText << "Time- " << timeMinutes << ":0" << timeSeconds;
        }
        else {
            board->timeText << "Time- " << timeMinutes << ":" << timeSeconds;
        }
    }
    board->scoreP1Text.str("");
    board->scoreP1Text << "Score: " << p1->score;
    board->scoreP2Text.str("");
    board->scoreP2Text << "Score: " << p2->score;
    board->livesP1Text.str("");
    board->livesP1Text << "Lives: " << p1->lives;
    board->livesP2Text.str("");
    board->livesP2Text << "Lives: " << p2->lives;

    board->loadRenderedText(window);
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
    g7->update(p2, p1);
    g7->move();
    g7->checkPacmanCollision(p1);
    g7->checkPacmanCollision(p2);

	if(time > timeToChangeMode){
		if(changedMode1) {
            g5->update(p2, p1);
		    g5->move();
            g5->checkPacmanCollision(p1);
            g5->checkPacmanCollision(p2);
        }
        if(changedMode2) {    
          g6->update(p2, p1);
		  g6->move();
		  g6->checkPacmanCollision(p1);	
		  g6->checkPacmanCollision(p2);
        }
	}
}   

void RenderElements(Pacman* p1, Pacman* p2, Ghost* g1, Ghost* g2, Ghost* g3, Ghost* g4, Ghost* g5, Ghost* g6, Ghost* g7, Uint32 time, Window* window) {
    p1->render(window);
    p2->render(window);
    g1->render(window);
    g2->render(window);
    g3->render(window);
    g4->render(window);
    g7->render(window);
    if(time >= timeToChangeMode){
        if(changedMode1) {
            g5->render(window);
        }
        if(changedMode2) {
            g6->render(window);
        }    
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

void renderBlackScreen(Pacman* p1, Pacman* p2, Window* window) {
    SDL_SetRenderDrawColor(window->getRenderer(), 0x00, 0x00, 0x00, 0xFF);
    SDL_Point points[952576];
    int n = 0;
    for(int i = 25; i <= 1000; i ++) {
        for(int j = 25; j <= 1000; j ++) {
            SDL_Point center1 = p1->colliderSphere.center, center2 = p2->colliderSphere.center;
            int distanceSq1 = (center1.x - j) * (center1.x - j) + (center1.y - i) * (center1.y - i);
            int distanceSq2 = (center2.x - j) * (center2.x - j) + (center2.y - i) * (center2.y - i);
            if(distanceSq1 >= 18225 && distanceSq2 >= 18225) {
                SDL_Point sample;
                sample.x = j;
                sample.y = i;
                points[n] = sample;  
                n ++;
            }
        }
    }
    SDL_RenderDrawPoints(window->getRenderer(), points, n);
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
    SDL_Rect scr = {1025, 0, 380, 1025};
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
    

    Scoreboard scoreBoard(&scr, &window);
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

    scoreBoard.start = startTime;
    bool randomized = false;

    
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
            if(changedMode1){
                g5.handleEvent(event, &p1, &p2);
            }
            if(changedMode2) {
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
        if( timeNow >= timeToChangeMode && (!changedMode1 || !changedMode2)) {
            switchGhostMode(&g1, &g2, &g3, &g4);
			if(g1.mode != 3 && g1.mode != 4) {
                g5 = g1;
                changedMode1 = true;
            }
            if(g2.mode != 3 && g2.mode != 4) {
			    g6 = g2;
                changedMode2 = true;
            }    
        }
        if(timeNow >= timeToRandomize && !randomized){
        	if(g1.mode != 3 && g1.mode != 4)
                g1.mode = 1;
            else 
                g1.prevMode = 1;
        	if(g2.mode != 3 && g2.mode != 4)
                g2.mode = 1;
            else 
                g2.prevMode = 1;
            if(g3.mode != 3 && g3.mode != 4)
                g3.mode = 1;
            else 
                g3.prevMode = 1;
            if(g4.mode != 3 && g4.mode != 4)
                g4.mode = 1;
            else 
                g4.prevMode = 1;
            if(g5.mode != 3 && g5.mode != 4)
                g5.mode = 1;
            else 
                g5.prevMode = 1;
            if(g6.mode != 3 && g6.mode != 4)
                g6.mode = 1;
            else 
                g6.prevMode = 1;
            if(g7.mode != 3 && g7.mode != 4)
                g7.mode = 1;
            else 
                g7.prevMode = 1;
        	randomized = true;
        }
        GhostUpdate(&p1, &p2, &g1, &g2, &g3, &g4, &g5, &g6, &g7, timeNow);
        ScoreUpdate(&scoreBoard, &p1, &p2, &window);
        scoreBoard.render(&window);

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
        p1.pacpacCollision(&p2);
        manager.renderPortals(&window);
        RenderElements(&p1, &p2, &g1, &g2, &g3, &g4, &g5, &g6, &g7, timeNow, &window);
        renderBlackScreen(&p1, &p2, &window);
        window.updateWindow();
        //SDL_Delay(10);
    }
    
    window.free();
    close();
}

