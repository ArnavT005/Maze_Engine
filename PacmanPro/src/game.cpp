#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_net.h>
#include <chrono>
#include <random>
#include "window.hpp"
#include "maze.hpp"
#include "pacman.hpp"
#include "ghost.hpp"
#include "manager.hpp"
#include "scoreboard.hpp"
#include "menu.hpp"

int timeToChangeMode = 34000;
int timeToRandomize = 64000;
int timeToFinale = 74000;
int finishTime = 104000;
bool changeMode = false, changedMode1 = false, changedMode2 = false; 
bool finalMode = false, createNew = false;
SDL_Point points[952576];
Mix_Music *bground = NULL;
Mix_Chunk* tenSecTimer = NULL;
Mix_Chunk* start = NULL;
Mix_Chunk* buzzer = NULL;

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
        if(TTF_Init() < 0) {
            std::cout << "TTF unable to initialize! SDL_ttf Error: " << TTF_GetError() << "\n";
            success = false;
        }
        if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            std::cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << "\n";
            success = false;
        }
        if(SDLNet_Init() < 0) {
            std::cout << "SDLNet could not initialize! SDLNet Error: " << SDLNet_GetError() << "\n";
            success = false;
        }

    }
    return success;
}

void close() {
    SDLNet_Quit();
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void LoadMusic() {
    Mix_AllocateChannels(32);
    tenSecTimer = Mix_LoadWAV( "../music/countdown.mp3");
    if( tenSecTimer == NULL ){
        printf( "Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
    }
    buzzer = Mix_LoadWAV( "../music/buzzer.wav");
    if(buzzer == NULL ){
        printf( "Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
    }
    bground = Mix_LoadMUS("../music/background.mp3");
    if(bground == NULL) 
        std::cout << "Failed to load music! SDL_mixer Error: " << Mix_GetError() << "\n";
    start = Mix_LoadWAV( "../music/intro.wav");
    if(start == NULL ){
        printf( "Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
    }
}

void ClearMusic() {
    if(bground != NULL) {
        Mix_FreeMusic(bground);
        bground = NULL;
    }
    if(start != NULL) {
        Mix_FreeChunk(start);
        start = NULL;
    }
    if(buzzer != NULL) {
        Mix_FreeChunk(buzzer);
        buzzer = NULL;
    }
    if(tenSecTimer != NULL) {
        Mix_FreeChunk(tenSecTimer);
        tenSecTimer = NULL;
    }

}

void ScoreUpdate(Scoreboard* board, Pacman* p1, Pacman* p2, Window* window) {
    int time = SDL_GetTicks() - board->start;
    int timeSeconds = (time - 4000) / 1000;
    int timeMinutes = timeSeconds / 60;
    timeSeconds -= 60 * timeMinutes;
    board->timeText.str("");
    if(time < 1050) {
        board->timeText << "3";
    }
    else if(time < 2100) {
        board->timeText << "2";
    }
    else if(time < 3150) {
        board->timeText << "1";
    }
    else if(time < 4000) {
        board->timeText << "GO!!";
    }
    else if(time > finishTime || p1->lives == 0 || p2->lives == 0) {
        board->timeText << "GAME OVER!!";
    }
    else {
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
    }
    board->scoreP1Text.str("");
    board->scoreP1Text << "Score: " << p1->score;
    board->scoreP2Text.str("");
    board->scoreP2Text << "Score: " << p2->score;
    board->livesP1Text.str("");
    board->livesP1Text << "Lives: ";
    board->livesP2Text.str("");
    board->livesP2Text << "Lives: ";
    board->loadRenderedText(window);
}

void GhostUpdate(Pacman* p1, Pacman* p2, Ghost* g1, Ghost* g2, Ghost*g3, Ghost* g4, Ghost* g5, Ghost* g6, Ghost* g7, Ghost* g8, Uint32 time) {
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
        if(time > timeToFinale) {
            if(createNew) {
                g8->update(p2, p1);
                g8->move();
                g8->checkPacmanCollision(p1); 
                g8->checkPacmanCollision(p2);
            }
        }
    }
}   

void RenderElements(Pacman* p1, Pacman* p2, Ghost* g1, Ghost* g2, Ghost* g3, Ghost* g4, Ghost* g5, Ghost* g6, Ghost* g7, Ghost* g8, Uint32 time, Window* window) {
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
        if(time >= timeToFinale) {
            if(createNew) {
                g8->render(window);
            }
        } 
    }

}

void switchGhostMode(int mode, Ghost* g1, Ghost* g2, Ghost* g3, Ghost* g4, Ghost* g5, Ghost* g6, Ghost* g7, Ghost* g8) {
    if(g1 != NULL) {
        if(g1->mode != 3 && g1->mode != 4) {
            g1->mode = mode;
        }
        else {
            g1->prevMode = mode;
        }
    }    
    if(g2 != NULL) {
        if(g2->mode != 3 && g2->mode != 4) {
            g2->mode = mode;
        }
        else {
            g2->prevMode = mode;
        }
    }  
    if(g3 != NULL) {
        if(g3->mode != 3 && g3->mode != 4) {
            g3->mode = mode;
        }
        else {
            g3->prevMode = mode;
        }
    }  
    if(g4 != NULL) {
        if(g4->mode != 3 && g4->mode != 4) {
            g4->mode = mode;
        }
        else {
            g4->prevMode = mode;
        }
    }
    if(g5 != NULL) {
        if(g5->mode != 3 && g5->mode != 4) {
            g5->mode = mode;
        }
        else {
            g5->prevMode = mode;
        }
    }
    if(g6 != NULL) {
        if(g6->mode != 3 && g6->mode != 4) {
            g6->mode = mode;
        }
        else {
            g6->prevMode = mode;
        }
    }
    if(g7 != NULL) {
        if(g7->mode != 3 && g7->mode != 4) {
            g7->mode = mode;
        }
        else {
            g7->prevMode = mode;
        }
    }
    if(g8 != NULL) {
        if(g8->mode != 3 && g8->mode != 4) {
            g8->mode = mode;
        }
        else {
            g8->prevMode = mode;
        }
    }          
}

void renderBlackScreen(Pacman* p1, Pacman* p2, Window* window, Uint32 timeNow) {
    if(timeNow >= timeToFinale)
        SDL_SetRenderDrawColor(window->getRenderer(), 0x00, 0x00, 0x00, 0xFF);
    else
        SDL_SetRenderDrawColor(window->getRenderer(), 0x00, 0x00, 0x00, 0x00);
    SDL_SetRenderDrawBlendMode(window->getRenderer(), SDL_BLENDMODE_BLEND);
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
    SDL_SetRenderDrawBlendMode(window->getRenderer(), SDL_BLENDMODE_NONE);
}

bool game(Menu* menu, Window* window, int id, TCPsocket* server, SDLNet_SocketSet* set, unsigned int SEED) {
    changeMode = false;
    changedMode1 = false;
    changedMode2 = false; 
    finalMode = false; 
    createNew = false;

    srand(SEED);

    std::string bgType = std::to_string((rand() % 6 + 1));

    // create maze
    Maze maze(16, 45, 15, 25);
    maze.bgType = bgType;
    SDL_Color boundaryColor = {0xFF, 0x00, 0x00, 0xFF};
    SDL_Texture* background = SDL_CreateTexture(window->getRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1025, 1025);
   
    SDL_Rect bg = {0, 0, 1025, 1025};
    SDL_Rect scr = {1025, 0, 380, 1025};
    SDL_SetTextureBlendMode(background, SDL_BLENDMODE_BLEND);
    maze.loadTexture(window);

    window->setRenderTarget(background);
   
    maze.createBase(window, 0, 0, boundaryColor);
    maze.createBasicStructure(window);
    maze.generateMazeRandom(window);

    int i = 0;
    while(!maze.BFS() && i < 16) {
        maze.refreshMaze(window);
        maze.createBase(window, 0, 0, boundaryColor);
        maze.createBasicStructure(window);
        maze.generateMazeRandom(window);
        i ++;
    }

    window->setRenderTarget(NULL);

    // create pacman
    Pacman p1(&maze, window, 1);
    Pacman p2(&maze, window, 2);

    // create scoreboard
    Scoreboard scoreBoard(&scr, window, &p1, &p2, bgType);
    
    // create manager
    Manager manager(&maze, SEED);
    manager.generateEatables(window);
    manager.generatePortals(window);
    int numEat = manager.eatables.size();

    for(int k = 0; k < numEat; k ++) {
        manager.eatables[k].setPacman(&p1, &p2);
    }

    // create ghosts
    Ghost g1(&maze, TYPE_BLINKY, 1, window, 0);
    Ghost g2(&maze, TYPE_PINKY, 1, window, 1);
    Ghost g3(&maze, TYPE_INKY, 1, window, 2);
    Ghost g4(&maze, TYPE_CLYDE, 1, window, 3);
    Ghost g5(&maze, TYPE_BLINKY, 1, window, 5);   // second blinky
    Ghost g6(&maze, TYPE_PINKY, 1, window, 6);   // second pinky
    Ghost g7(&maze, TYPE_CLYDE, 1, window, 4);
    Ghost g8(&maze, TYPE_INKY, 1, window, 7);   // second inky
    g1.generator.seed(SEED % 65536 + 10);
    g2.generator.seed(SEED % 65536 + 20);
    g3.generator.seed(SEED % 65536 + 30);
    g4.generator.seed(SEED % 65536 + 40);
    g5.generator.seed(SEED % 65536 + 50);
    g6.generator.seed(SEED % 65536 + 60);
    g7.generator.seed(SEED % 65536 + 70);
    g8.generator.seed(SEED % 65536 + 80);
    
    // clear window
    window->clearWindow();

    SDL_Event event;
    bool startGame = true;
    bool timer = false;

    Uint32 startTime = SDL_GetTicks();
    scoreBoard.start = startTime;

    bool randomized = false;
    bool temp;

    char msg[1000];

    while(menu->isRunning) {
        if(!timer && SDL_GetTicks() - startTime > finishTime - 9000) {
            Mix_PlayChannel(18, tenSecTimer, 0);
            timer = true;
        }
        int i = 0;
        temp = false;
        if(SDL_GetTicks() - startTime >= 4000) {
            while(SDL_PollEvent(& event)) {
                if(event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) ) {
                    // send quit message to server
                    if(*server != NULL) {
                        if(SDLNet_TCP_Send(*server, "QUIT", 5) < 5) {
                            std::cout << "Unable to send client 1 message! SDLNet Error: " << SDLNet_GetError() << "\n";
                        }
                    }
                    menu->isRunning = false;
                    return true;
                }
                p1.handleEvent(event, SDL_GetKeyboardState(NULL), 0);
                p2.handleEvent(event, SDL_GetKeyboardState(NULL), 0);
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
                if(createNew) {
                    g8.handleEvent(event, &p1, &p2);
                }
            }
            if(menu->onlinePossible) {
                while(SDLNet_CheckSockets(*set, 0) > 0) {
                    if(SDLNet_SocketReady(*server) != 0) {
                        if(SDLNet_TCP_Recv(*server, msg, 1000) > 0) {
                            if(strcmp(msg, "QUIT") == 0) {
                                menu->onlinePossible = false;
                                break;
                            }
                        }
                    }
                }
            }
        }
        window->clearWindow();
        window->renderTexture(background, NULL, &bg);
        p1.move();
        p2.move();
        p1.pacpacCollision(&p2);
        for(i = 0; i < numEat; i ++) {
            manager.eatables[i].checkIfEaten(temp);
            manager.eatables[i].render(window);
        }
        p1.isBuffed = temp;
        p2.isBuffed = temp;
        Uint32 timeNow = SDL_GetTicks() - startTime;
        if(timeNow >= timeToChangeMode && !changeMode) {
            switchGhostMode(2, &g1, &g2, &g3, &g4, &g5, &g6, &g7, &g8);
            changeMode = true;
        }
        if(timeNow >= timeToChangeMode && (!changedMode1 || !changedMode2)) {   
            if(g1.mode != 3 && g1.mode != 4 && !changedMode1) {
                g5.splitGhost(g1);
                changedMode1 = true;
            }
            if(g2.mode != 3 && g2.mode != 4 && !changedMode2) {
                g6.splitGhost(g2);
                changedMode2 = true;
            }    
        }
        if(timeNow >= timeToRandomize && !randomized){
            switchGhostMode(1, &g1, &g2, &g3, &g4, &g5, &g6, &g7, &g8);
            randomized = true;
        }
        if(timeNow >= timeToFinale && !finalMode) {
            switchGhostMode(5, &g1, &g2, &g3, &g4, &g5, &g6, &g7, &g8);
            finalMode = true;
        }
        if(timeNow >= timeToFinale && !createNew) {
            if(g3.mode != 3 && g3.mode != 4) {
                g8.splitGhost(g3);
                createNew = true;
            }
        }
        GhostUpdate(&p1, &p2, &g1, &g2, &g3, &g4, &g5, &g6, &g7, &g8, timeNow);
        ScoreUpdate(&scoreBoard, &p1, &p2, window);
        scoreBoard.render(window);
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
        manager.renderPortals(window);
        RenderElements(&p1, &p2, &g1, &g2, &g3, &g4, &g5, &g6, &g7, &g8, timeNow, window);
        renderBlackScreen(&p1, &p2, window, timeNow);
        window->updateWindow();
        if(startGame) {
            Mix_PlayChannel(12, start, 0);
            startGame = false;
        }
        if(SDL_GetTicks() - startTime > finishTime + 500) {
            menu->isRunning = false;
            menu->isOver = true;
            menu->isAtEnd = true;
            Mix_HaltChannel(-1);
            Mix_PlayChannel(19, buzzer, 0);
            SDL_Delay(1000);
        }
        if(p1.lives == 0 || p2.lives == 0) {
            menu->isRunning = false;
            menu->isOver = true;
            menu->isAtEnd = true;
            SDL_Delay(500);
            Mix_HaltChannel(-1);
            Mix_PlayChannel(19, buzzer, 0);
        }
        if(p1.lives != 0 && p2.lives != 0) {
            if(p1.score > p2.score)
                menu->winner = 1;
            else if(p1.score < p2.score)
                menu->winner = 2;
            else {
                if(p1.lives > p2.lives)
                    menu->winner = 1;
                else if(p1.lives < p2.lives)
                    menu->winner = 2;
                else 
                    menu->winner = 0;
            }
        }
        else {
            if(p1.lives != 0 && p2.lives == 0)
                menu->winner = 1;
            else if(p1.lives == 0 && p2.lives != 0)
                menu->winner = 2;
            else {
                if(p1.score > p2.score)
                    menu->winner = 1;
                else if(p1.score < p2.score)
                    menu->winner = 2;
                else
                    menu->winner = 0;
            }
        }
    }
    g1.free();
    g2.free();
    g3.free();
    g4.free();
    g5.free();
    g6.free();
    g7.free();
    g8.free();
    manager.freePortals();
    manager.freeEatables();
    p2.free(); p1.free();
    scoreBoard.free1();
    if(background != NULL) {
        SDL_DestroyTexture(background);
        background = NULL;
    }
    maze.free();
    return false;
}

bool gameOnline(Menu* menu, Window* window, int id, TCPsocket* server, SDLNet_SocketSet* set, unsigned int SEED) {
    changeMode = false;
    changedMode1 = false;
    changedMode2 = false; 
    finalMode = false; 
    createNew = false;

    srand(SEED);
    unsigned randomBG = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 generator(randomBG);
    std::string bgType = std::to_string(generator() % (unsigned int)6 + (unsigned int)1);

    // create maze
    Maze maze(16, 45, 15, 25);
    maze.bgType = bgType;

    SDL_Color boundaryColor = {0xFF, 0x00, 0x00, 0xFF};
    SDL_Texture* background = SDL_CreateTexture(window->getRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1025, 1025);
   
    SDL_Rect bg = {0, 0, 1025, 1025};
    SDL_Rect scr = {1025, 0, 380, 1025};
    SDL_SetTextureBlendMode(background, SDL_BLENDMODE_BLEND);
    maze.loadTexture(window);

    window->setRenderTarget(background);
   
    maze.createBase(window, 0, 0, boundaryColor);
    maze.createBasicStructure(window);
    maze.generateMazeRandom(window);

    int i = 0;
    while(!maze.BFS() && i < 16) {
        maze.refreshMaze(window);
        maze.createBase(window, 0, 0, boundaryColor);
        maze.createBasicStructure(window);
        maze.generateMazeRandom(window);
        i ++;
    }


    window->setRenderTarget(NULL);
    

    // create pacman
    Pacman p1(&maze, window, 1);
    Pacman p2(&maze, window, 2);

    // create scoreboard
    Scoreboard scoreBoard(&scr, window, &p1, &p2, bgType);
    
    // create manager
    Manager manager(&maze, SEED);
    manager.generateEatables(window);
    manager.generatePortals(window);
    int numEat = manager.eatables.size();

    for(int k = 0; k < numEat; k ++) {
        manager.eatables[k].setPacman(&p1, &p2);
    }

    // create ghosts
    Ghost g1(&maze, TYPE_BLINKY, 1, window, 0);
    Ghost g2(&maze, TYPE_PINKY, 1, window, 1);
    Ghost g3(&maze, TYPE_INKY, 1, window, 2);
    Ghost g4(&maze, TYPE_CLYDE, 1, window, 3);
    Ghost g5(&maze, TYPE_BLINKY, 1, window, 5);   // second blinky
    Ghost g6(&maze, TYPE_PINKY, 1, window, 6);   // second pinky
    Ghost g7(&maze, TYPE_CLYDE, 2, window, 4);
    Ghost g8(&maze, TYPE_INKY, 1, window, 7);   // second inky
    g1.generator.seed(SEED % 65536 + 10);
    g2.generator.seed(SEED % 65536 + 20);
    g3.generator.seed(SEED % 65536 + 30);
    g4.generator.seed(SEED % 65536 + 40);
    g5.generator.seed(SEED % 65536 + 50);
    g6.generator.seed(SEED % 65536 + 60);
    g7.generator.seed(SEED % 65536 + 70);
    g8.generator.seed(SEED % 65536 + 80);
    
    // clear window
    window->clearWindow();

    SDL_Event event;
    bool startGame = true;
    bool timer = false;

    Uint32 startTime = SDL_GetTicks();
    scoreBoard.start = startTime;

    bool randomized = false;
    bool temp;

    string sendMsg = "default", final = "GAME OVER";
    int len;
    char recvdMsg[1000];

    string Quit = "QUIT";
    bool upFlag = false, downFlag = false, rightFlag = false, leftFlag = false;
    bool message = false;

    while(menu->isRunning) {
        Uint32 startLoop = SDL_GetTicks();
        if(!timer && SDL_GetTicks() - startTime > finishTime - 9000) {
            Mix_PlayChannel(18, tenSecTimer, 0);
            timer = true;
        }
        int i = 0;
        temp = false;
        if(SDL_GetTicks() - startTime >= 4000) {

            while(SDL_PollEvent(& event)) {
                if(event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) ) {
                    // send message to server
                    if(*server != NULL) {
                        if(SDLNet_TCP_Send(*server, Quit.c_str(), 5) < 5) {
                            std::cout << "Unable to send client 1 message! SDLNet Error: " << SDLNet_GetError() << "\n";
                        }
                    }
                    menu->isRunning = false;
                    return true;
                }
                sendMsg = "default";
                // send message to server
                if(id==1 && event.key.repeat == 0){
                    if(event.type == SDL_KEYDOWN) {
                        switch(event.key.keysym.sym) {
                            case SDLK_w: sendMsg = "upDown"; break;
                            case SDLK_s: sendMsg = "downDown"; break;
                            case SDLK_d: sendMsg = "rightDown"; break;
                            case SDLK_a: sendMsg = "leftDown"; break;
                            case SDLK_g: sendMsg = "parry"; break;
                            default: sendMsg = "default"; break;
                        }
                    }
                    else if(event.type == SDL_KEYUP) {
                        switch(event.key.keysym.sym) {
                            case SDLK_w: sendMsg = "upUP"; break;
                            case SDLK_s: sendMsg = "downUP"; break;
                            case SDLK_d: sendMsg = "rightUP"; break;
                            case SDLK_a: sendMsg = "leftUP"; break;
                            default: sendMsg = "default"; break;
                        }
                    }
                    if(sendMsg != "default") {
                        len = sendMsg.length();
                        if(*server != NULL) {
                            if(SDLNet_TCP_Send(*server, sendMsg.c_str(), len + 1) < len + 1) {
                                std::cout << "Unable to send client 1 message! SDLNet Error: " << SDLNet_GetError() << "\n";
                            }
                        }
                    }    
                }
                else if(id == 2 && event.key.repeat == 0){
                    if(event.type == SDL_KEYDOWN) {
                        switch(event.key.keysym.sym) {
                            case SDLK_UP: sendMsg = "upDown"; break;
                            case SDLK_DOWN: sendMsg = "downDown"; break;
                            case SDLK_RIGHT: sendMsg = "rightDown"; break;
                            case SDLK_LEFT: sendMsg = "leftDown"; break;
                            case SDLK_m: sendMsg = "parry"; break;
                            default: sendMsg = "default"; break;
                        }
                    }
                    else if(event.type == SDL_KEYUP) {
                        switch(event.key.keysym.sym) {
                            case SDLK_UP: sendMsg = "upUP"; break;
                            case SDLK_DOWN: sendMsg = "downUP"; break;
                            case SDLK_RIGHT: sendMsg = "rightUP"; break;
                            case SDLK_LEFT: sendMsg = "leftUP"; break;
                            default: sendMsg = "default"; break;
                        }
                    }
                    if(sendMsg != "default") {
                        len = sendMsg.length();
                        if(*server != NULL) {
                            if(SDLNet_TCP_Send(*server, sendMsg.c_str(), len + 1) < len + 1) {
                                std::cout << "Unable to send client 2 message! SDLNet Error: " << SDLNet_GetError() << "\n";
                            }
                        }
                    } 
                }
                if(id == 1) p1.handleEvent(event, SDL_GetKeyboardState(NULL), 0);
                if(id == 2) p2.handleEvent(event, SDL_GetKeyboardState(NULL), 0);
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
                if(createNew) {
                    g8.handleEvent(event, &p1, &p2);
                }
            }
            if(*server != NULL) {
                while(SDLNet_CheckSockets(*set, 0) > 0) {

                    SDL_Event E;
                    E.key.repeat = 0;
                    E.type = SDL_KEYUP;
                    E.key.keysym.sym = SDLK_t;
                    // check server activity
                    if(SDLNet_SocketReady(*server) != 0) {
                        // receive message from server
                        if(SDLNet_TCP_Recv(*server, recvdMsg, 1000) > 0){
                            if(strcmp(recvdMsg, "default") != 0) {
                                if(!(upFlag || rightFlag || downFlag || leftFlag)) {
                                    if(strcmp(recvdMsg, "upDown") == 0){
                                        if(id==1){
                                            E.key.keysym.sym = SDLK_UP;
                                            E.type = SDL_KEYDOWN;
                                        } 
                                        else { 
                                            E.key.keysym.sym = SDLK_w;
                                            E.type = SDL_KEYDOWN;
                                        }
                                    }
                                    else if(strcmp(recvdMsg, "downDown") == 0){
                                        if(id==1){
                                            E.key.keysym.sym = SDLK_DOWN;
                                            E.type = SDL_KEYDOWN;
                                        } 
                                        else { 
                                            E.key.keysym.sym = SDLK_s;
                                            E.type = SDL_KEYDOWN;
                                        }
                                    }
                                    else if(strcmp(recvdMsg, "leftDown") == 0){
                                        if(id==1){
                                            E.key.keysym.sym = SDLK_LEFT;
                                            E.type = SDL_KEYDOWN;
                                        } 
                                        else { 
                                            E.key.keysym.sym = SDLK_a;
                                            E.type = SDL_KEYDOWN;
                                        }
                                    }
                                    else if(strcmp(recvdMsg, "rightDown") == 0){
                                        if(id==1){
                                            E.key.keysym.sym = SDLK_RIGHT;
                                            E.type = SDL_KEYDOWN;
                                        } 
                                        else { 
                                            E.key.keysym.sym = SDLK_d;
                                            E.type = SDL_KEYDOWN;
                                        }
                                    }
                                }
                                if(strcmp(recvdMsg, "parry") == 0){
                                    if(id==1){
                                        E.key.keysym.sym = SDLK_m;
                                        E.type = SDL_KEYDOWN;
                                    } 
                                    else { 
                                        E.key.keysym.sym = SDLK_g;
                                        E.type = SDL_KEYDOWN;
                                    }
                                }
                                else if(strcmp(recvdMsg, "upUP") == 0){
                                    upFlag = false;
                                    if(id==1){
                                        E.key.keysym.sym = SDLK_UP;
                                        E.type = SDL_KEYUP;
                                    } 
                                    else { 
                                        E.key.keysym.sym = SDLK_w;
                                        E.type = SDL_KEYUP;
                                    }
                                }
                                else if(strcmp(recvdMsg, "downUP") == 0){
                                    downFlag = false;
                                    if(id==1){
                                        E.key.keysym.sym = SDLK_DOWN;
                                        E.type = SDL_KEYUP;
                                    } 
                                    else { 
                                        E.key.keysym.sym = SDLK_s;
                                        E.type = SDL_KEYUP;
                                    }
                                }
                                else if(strcmp(recvdMsg, "leftUP") == 0){
                                    leftFlag = false;
                                    if(id==1){
                                        E.key.keysym.sym = SDLK_LEFT;
                                        E.type = SDL_KEYUP;
                                    } 
                                    else { 
                                        E.key.keysym.sym = SDLK_a;
                                        E.type = SDL_KEYUP;
                                    }
                                }
                                else if(strcmp(recvdMsg, "rightUP") == 0){
                                    rightFlag = false;
                                    if(id==1){
                                        E.key.keysym.sym = SDLK_RIGHT;
                                        E.type = SDL_KEYUP;
                                    } 
                                    else { 
                                        E.key.keysym.sym = SDLK_d;
                                        E.type = SDL_KEYUP;
                                    }
                                }
                                else if(strcmp(recvdMsg, "QUIT") == 0) {
                                    menu->isRunning = false;
                                    menu->isAtEnd = true;
                                    menu->isOver = true;
                                    menu->winner = 3;
                                    menu->onlinePossible = false;
                                    Mix_HaltChannel(-1);
                                    Mix_PlayChannel(19, buzzer, 0);
                                    SDL_Delay(500);
                                    break;
                                }
                            }
                        }
                    }

                    if(id == 2) p1.handleEvent(E, SDL_GetKeyboardState(NULL), 1);
                    if(id == 1) p2.handleEvent(E, SDL_GetKeyboardState(NULL), 1);
                    g1.handleEvent(E, &p1, &p2);
                    g2.handleEvent(E, &p1, &p2);
                    g3.handleEvent(E, &p1, &p2);
                    g4.handleEvent(E, &p1, &p2);
                    g7.handleEvent(E, &p1, &p2);
                    if(changedMode1){
                        g5.handleEvent(E, &p1, &p2);
                    }
                    if(changedMode2) {
                        g6.handleEvent(E, &p1, &p2);
                    }
                    if(createNew) {
                        g8.handleEvent(E, &p1, &p2);
                    } 
                }       
            }
        }
        if(!menu->isRunning)
            break;
        window->clearWindow();
        window->renderTexture(background, NULL, &bg);
        p1.move();
        p2.move();
        p1.pacpacCollision(&p2);
        

        for(i = 0; i < numEat; i ++) {
            manager.eatables[i].checkIfEaten(temp);
            manager.eatables[i].render(window);
        }
        p1.isBuffed = temp;
        p2.isBuffed = temp;



        Uint32 timeNow = SDL_GetTicks() - startTime;
        if(timeNow >= timeToChangeMode && !changeMode) {
            switchGhostMode(2, &g1, &g2, &g3, &g4, &g5, &g6, &g7, &g8);
            changeMode = true;
        }
        if(timeNow >= timeToChangeMode && (!changedMode1 || !changedMode2)) {   
            if(g1.mode != 3 && g1.mode != 4 && !changedMode1) {
                g5.splitGhost(g1);
                changedMode1 = true;
            }
            if(g2.mode != 3 && g2.mode != 4 && !changedMode2) {
                g6.splitGhost(g2);
                changedMode2 = true;
            }    
        }
        if(timeNow >= timeToRandomize && !randomized){
            switchGhostMode(1, &g1, &g2, &g3, &g4, &g5, &g6, &g7, &g8);
            randomized = true;
        }
        if(timeNow >= timeToFinale && !finalMode) {
            switchGhostMode(5, &g1, &g2, &g3, &g4, &g5, &g6, &g7, &g8);
            finalMode = true;
        }
        if(timeNow >= timeToFinale && !createNew) {
            if(g3.mode != 3 && g3.mode != 4) {
                g8.splitGhost(g3);
                createNew = true;
            }
        }
        GhostUpdate(&p1, &p2, &g1, &g2, &g3, &g4, &g5, &g6, &g7, &g8, timeNow);
        ScoreUpdate(&scoreBoard, &p1, &p2, window);
        scoreBoard.render(window);
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
        manager.renderPortals(window);
        RenderElements(&p1, &p2, &g1, &g2, &g3, &g4, &g5, &g6, &g7, &g8, timeNow, window);
        renderBlackScreen(&p1, &p2, window, timeNow);
        window->updateWindow();
        if(startGame) {
            Mix_PlayChannel(12, start, 0);
            startGame = false;
        }
        if(SDL_GetTicks() - startTime > finishTime + 500) {
            menu->isRunning = false;
            menu->isOver = true;
            menu->isAtEnd = true;
            if(SDLNet_TCP_Send(*server, final.c_str(), 10) < 10) {
                std::cout << "Unable to send message to server! SDLNet Error: " << SDLNet_GetError() << "\n";
            }
            Mix_HaltChannel(-1);
            Mix_PlayChannel(19, buzzer, 0);
            SDL_Delay(1000);
        }
        if(p1.lives == 0 || p2.lives == 0) {
            menu->isRunning = false;
            menu->isOver = true;
            menu->isAtEnd = true;
            SDL_Delay(500);
            if(SDLNet_TCP_Send(*server, final.c_str(), 10) < 10) {
                std::cout << "Unable to send message to server! SDLNet Error: " << SDLNet_GetError() << "\n";
            }
            Mix_HaltChannel(-1);
            Mix_PlayChannel(19, buzzer, 0);
        }
        if(p1.lives != 0 && p2.lives != 0) {
            if(p1.score > p2.score)
                menu->winner = 1;
            else if(p1.score < p2.score)
                menu->winner = 2;
            else {
                if(p1.lives > p2.lives)
                    menu->winner = 1;
                else if(p1.lives < p2.lives)
                    menu->winner = 2;
                else 
                    menu->winner = 0;
            }
        }
        else {
            if(p1.lives != 0 && p2.lives == 0)
                menu->winner = 1;
            else if(p1.lives == 0 && p2.lives != 0)
                menu->winner = 2;
            else {
                if(p1.score > p2.score)
                    menu->winner = 1;
                else if(p1.score < p2.score)
                    menu->winner = 2;
                else
                    menu->winner = 0;
            }
        }
        Uint32 loopTime = SDL_GetTicks() - startLoop;
        if(loopTime > 25 && !message && menu->isRunning) {
            std::cout << "Connection to server is slow. Some packet loss/delay may be there!\n";
            message = true;
        }
    }
    g1.free();
    g2.free();
    g3.free();
    g4.free();
    g5.free();
    g6.free();
    g7.free();
    g8.free();
    manager.freePortals();
    manager.freeEatables();
    p2.free(); p1.free();
    scoreBoard.free1();
    if(background != NULL) {
        SDL_DestroyTexture(background);
        background = NULL;
    }
    maze.free();
    return false;
}

int getNumber(char ch[], int size) {
    int num = 0;
    for(int i = 0; i < size; i ++) {
        
        if(ch[i] != '\0') {
            if(isalpha(ch[i]))
                continue;
            else
                num = 10 * num + ch[i] - '0';
        }
        else {
            break;
        }
    }
    return num;
}

int main(int argc, char** argv) {
    if(!SDL_init()) {
        return 0;
    }
    Window window("PACMAN", 1405, 1025);
    if(!window.getSuccess()) {
        close();
        return 0;
    }
    unsigned int SEED;

    bool quit = false;
    bool onlinePossible = true;
    SDL_Event event;
    

    // set connection with server if available
    SDLNet_SocketSet set;
    set = SDLNet_AllocSocketSet(2);
    if(set == NULL) {
        std::cout << "Could not create socket set! SDLNet Error: " << SDLNet_GetError() << "\n";
        std::cout << "Only local play available!\n";
        onlinePossible = false;
    }
    TCPsocket server;
    IPaddress ip;
    // player id
    int id = 0;    
    char msg[1000];


    if(SDLNet_ResolveHost(&ip, "127.0.0.1", 1234) < 0) {
        std::cout << "Could not resolve host! SDLNet Error: " << SDLNet_GetError() << "\n";
        std::cout << "Only local play available!\n";
        onlinePossible = false;
    }
    else {
        server = SDLNet_TCP_Open(&ip);
        if(server == NULL) {
            std::cout << "Could not open socket! SDLNet Error: " << SDLNet_GetError() << "\n";
            std::cout << "Only local play available!\n";
            onlinePossible = false;
        }
        else {
            if(SDLNet_TCP_AddSocket(set, server) < 0) {
                std::cout << "Could not add server to socket set! SDLNet Error: " << SDLNet_GetError() << "\n";
                std::cout << "Only local play available!\n";
                onlinePossible = false;
            }
            else {
                if(SDLNet_CheckSockets(set, 500) < 0) {
                    std::cout << "No response from server! SDLNet Error: " << SDLNet_GetError() << "\n";
                    std::cout << "Only local play available!\n";
                    onlinePossible = false;
                }
                else {
                    if(SDLNet_SocketReady(server) != 0) {
                        if(SDLNet_TCP_Recv(server, msg, 1000) > 0){
                            if(strcmp(msg, "1") == 0)
                                id = 1;
                            else if(strcmp(msg, "2") == 0)
                                id = 2;
                        }
                    }
                    else {
                        std::cout << "No response from server! SDLNet Error: " << SDLNet_GetError() << "\n";
                        std::cout << "Only local play available!\n";
                        onlinePossible = false;
                    }
                }
            }   
        }    
    }
    // connection set


    LoadMusic();    
    Menu menu(&window);
    menu.onlinePossible = onlinePossible;

    Mix_PlayMusic(bground, -1);
    Mix_VolumeMusic(50);
    string start = "START", Quit = "QUIT";
    bool firstMatch = true;

    while(!quit) {
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                quit = true;
                if(server != NULL) {
                    // send quit message to server
                    if(SDLNet_TCP_Send(server, Quit.c_str(), 5) < 5) {
                        std::cout << "Unable to send message to server! SDLNet Error: " << SDLNet_GetError() << "\n";
                        std::cout << "Only local play available!\n";
                        menu.onlinePossible = false;
                    }
                }
                break;
            }
            menu.handleEvent(event);
        }
        if(quit)
            break;
        if(menu.onlinePossible) {
            if(server != NULL) {
                while(SDLNet_CheckSockets(set, 0) > 0) {
                    if(SDLNet_SocketReady(server) != 0) {
                        if(SDLNet_TCP_Recv(server, msg, 1000) > 0) {
                            if(strcmp(msg, "QUIT") == 0) {
                                menu.onlinePossible = false;
                                break;
                            }
                        }
                    }
                } 
            }
            else {
                menu.onlinePossible = false;
            }
        }
        if(menu.isRunning == true) {
            if(menu.mode == 1) {
                // random seed, asynchronised
                SEED = time(0);
                // local game
                quit = game(&menu, &window, id, &server, &set, SEED);
            }
            else if(menu.mode == 2 && !menu.onlinePossible) {
                window.clearWindow();
                menu.render(&window);
                window.updateWindow();
            }
            else if(menu.mode == 2 && menu.onlinePossible) {
                // online play is available
                // display connecting screen
                window.clearWindow();
                menu.render(&window);
                window.updateWindow();

                bool isConnected = false;
                // send start message to server
                if(SDLNet_TCP_Send(server, start.c_str(), 6) < 6) {
                    std::cout << "Unable to send message to server! SDLNet Error: " << SDLNet_GetError() << "\n";
                    std::cout << "Only local play available!\n";
                    menu.onlinePossible = false;
                }
                else {
                    while(menu.isRunning) {
                        if(SDL_PollEvent(&event)) {
                            if(event.type == SDL_QUIT || event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                                quit = true;
                                menu.isRunning = false;  
                                if(server != NULL) {
                                    // send quit message to server
                                    if(SDLNet_TCP_Send(server, Quit.c_str(), 5) < 5) {
                                        std::cout << "Unable to send message to server! SDLNet Error: " << SDLNet_GetError() << "\n";
                                        std::cout << "Only local play available!\n";
                                        menu.onlinePossible = false;
                                    }
                                }
                                break;
                            }
                            menu.handleEvent(event);
                        }
                        if(!menu.isRunning) {
                            if(server != NULL) {
                                // send retreat message to server
                                if(SDLNet_TCP_Send(server, "RETREAT", 8) < 8) {
                                    std::cout << "Unable to send message to server! SDLNet Error: " << SDLNet_GetError() << "\n";
                                    std::cout << "Only local play available!\n";
                                    menu.onlinePossible = false;
                                }
                            }
                            break;
                        }
                        if(menu.onlinePossible) {
                            while(SDLNet_CheckSockets(set, 0) > 0) {
                                if(SDLNet_SocketReady(server) != 0) {
                                    if(SDLNet_TCP_Recv(server, msg, 1000) > 0) {
                                        if(strcmp(msg, "QUIT") == 0) {
                                            menu.onlinePossible = false;
                                            break;
                                        }
                                        else {
                                            SEED = getNumber(msg, 1000);
                                            isConnected = true;
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                        window.clearWindow();
                        menu.render(&window);
                        window.updateWindow();    
                        if(isConnected) {
                            quit = gameOnline(&menu, &window, id, &server, &set, SEED);              
                        } 
                    }               
                }         
            }
        }
        else {
            window.clearWindow();
            menu.render(&window);
            window.updateWindow();
        }    
    }

    Mix_HaltMusic();
    Mix_HaltChannel(-1);
    SDL_Delay(5000);
    menu.free();
    window.free();
    ClearMusic();
    if(set != NULL) {
        SDLNet_TCP_DelSocket(set, server);
        if(server != NULL) {
            SDLNet_TCP_Close(server);
        }
        SDLNet_FreeSocketSet(set);
    }
    else {
        if(server != NULL) {
            SDLNet_TCP_Close(server);
        }
    }
    close();
}

