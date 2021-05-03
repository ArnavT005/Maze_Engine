#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_net.h>
#include "sound.hpp"
#include "window.hpp"
#include "maze.hpp"
#include "pacman.hpp"
#include "ghost.hpp"
#include "manager.hpp"
#include "scoreboard.hpp"
#include "menu.hpp"

int timeToChangeMode = 14000;
int timeToRandomize = 24000;
int timeToFinale = 34000;
int finishTime = 44000;
bool changeMode = false, changedMode1 = false, changedMode2 = false; 
bool finalMode = false, createNew = false;
SDL_Point points[952576];

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

bool game(Menu* menu, Window* window) {
    changeMode = false;
    changedMode1 = false;
    changedMode2 = false; 
    finalMode = false; 
    createNew = false;

    // create maze
    Maze maze(16, 45, 15, 25);
   
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


    window->setRenderTarget(NULL);
    

    // create pacman
    Pacman p1(&maze, window, 1);
    Pacman p2(&maze, window, 2);

    // create scoreboard
    Scoreboard scoreBoard(&scr, window, &p1, &p2);
    
    // create manager
    Manager manager(&maze);
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
    Ghost g5;
    Ghost g6;
    Ghost g7(&maze, TYPE_CLYDE, 2, window, 4);
    Ghost g8;
    
    // clear window
    window->clearWindow();

    SDL_Event event;
    bool startGame = true;
    bool timer = false;

    Uint32 startTime = SDL_GetTicks();
    scoreBoard.start = startTime;

    bool randomized = false;
    bool temp;

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
                    return true;
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
            switchGhostMode(2, &g1, &g2, &g3, &g4, NULL, NULL, &g7, NULL);
            changeMode = true;
        }
        if(timeNow >= timeToChangeMode && (!changedMode1 || !changedMode2)) {   
            if(g1.mode != 3 && g1.mode != 4) {
                g5 = g1;
                g5.channel = 5;
                changedMode1 = true;
            }
            if(g2.mode != 3 && g2.mode != 4) {
                g6 = g2;
                g6.channel = 6;
                changedMode2 = true;
            }    
        }
        if(timeNow >= timeToRandomize && !randomized){
            switchGhostMode(1, &g1, &g2, &g3, &g4, &g5, &g6, &g7, NULL);
            randomized = true;
        }
        if(timeNow >= timeToFinale && !finalMode) {
            switchGhostMode(5, &g1, &g2, &g3, &g4, &g5, &g6, &g7, NULL);
            finalMode = true;
        }
        if(timeNow >= timeToFinale && !createNew) {
            if(g3.mode != 3 && g3.mode != 4) {
                g8 = g3;
                g8.channel = 7;
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
    p2.free(); p1.free();
    scoreBoard.free1();
    if(background != NULL) {
        SDL_DestroyTexture(background);
        background = NULL;
    }
    maze.free();
    return false;
}

bool gameOnline(Menu* menu, Window* window, int id, TCPsocket* server, SDLNet_SocketSet* set) {
    changeMode = false;
    changedMode1 = false;
    changedMode2 = false; 
    finalMode = false; 
    createNew = false;

    // create maze
    Maze maze(16, 45, 15, 25);
   
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


    window->setRenderTarget(NULL);
    

    // create pacman
    Pacman p1(&maze, window, 1);
    Pacman p2(&maze, window, 2);

    // create scoreboard
    Scoreboard scoreBoard(&scr, window, &p1, &p2);
    
    // create manager
    Manager manager(&maze);
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
    Ghost g5;
    Ghost g6;
    Ghost g7(&maze, TYPE_CLYDE, 2, window, 4);
    Ghost g8;
    
    // clear window
    window->clearWindow();

    SDL_Event event;
    bool startGame = true;
    bool timer = false;

    Uint32 startTime = SDL_GetTicks();
    scoreBoard.start = startTime;

    bool randomized = false;
    bool temp;

    string sendMsg = "default";
    int len;
    char recvdMsg[1000];

    while(menu->isRunning) {
        if(!timer && SDL_GetTicks() - startTime > finishTime - 9000) {
            Mix_PlayChannel(18, tenSecTimer, 0);
            timer = true;
        }
        int i = 0;
        temp = false;
        if(SDL_GetTicks() - startTime >= 4000) {

                // receive message from server
                // no waiting
                //int num = SDLNet_CheckSockets(*set, 0);
                // if(num < 0) {
                //     if(id == 1)
                //         std::cout << "Error while checking sockets (client 1)! SDLNet Error: " << SDLNet_GetError() << "\n";
                //     else
                //         std::cout << "Error while checking sockets (client 2)! SDLNet Error: " << SDLNet_GetError() << "\n";
                // }
                // else if(num > 0) {
                // }
                while(SDLNet_CheckSockets(*set, 0) > 0) {

                    SDL_Event E;
                    E.type = SDL_KEYDOWN;
                    E.key.repeat = 0;
                    // check server activity
                    if(SDLNet_SocketReady(*server) != 0) {
                        // receive message from server
                        if(SDLNet_TCP_Recv(*server, recvdMsg, 1000) > 0){
                            std::cout << recvdMsg << "\n";
                            if(strcmp(recvdMsg, "default") != 0) {
                                if(strcmp(recvdMsg, "up") == 0){if(id==1){E.key.keysym.sym = SDLK_UP;} else { E.key.keysym.sym = SDLK_w;}}
                                if(strcmp(recvdMsg, "down") == 0){if(id==1){E.key.keysym.sym = SDLK_DOWN;} else { E.key.keysym.sym = SDLK_s;}}
                                if(strcmp(recvdMsg, "left") == 0){if(id==1){E.key.keysym.sym = SDLK_LEFT;} else { E.key.keysym.sym = SDLK_a;}}
                                if(strcmp(recvdMsg, "right") == 0){if(id==1){E.key.keysym.sym = SDLK_RIGHT;} else { E.key.keysym.sym = SDLK_d;}}
                                if(strcmp(recvdMsg, "parry") == 0){if(id==1){E.key.keysym.sym = SDLK_m;} else { E.key.keysym.sym = SDLK_g;}}
                                p1.handleEvent(E, SDL_GetKeyboardState(NULL));
                                p2.handleEvent(E, SDL_GetKeyboardState(NULL));
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
                            }
                        }
                    }    
                }
            while(SDL_PollEvent(& event)) {
                if(event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) ) {
                    return true;
                }

                // send message to server
                if(id==1 && event.type == SDL_KEYDOWN && event.key.repeat == 0){
                    switch(event.key.keysym.sym) {
                        case SDLK_w: sendMsg = "up"; break;
                        case SDLK_s: sendMsg = "down"; break;
                        case SDLK_d: sendMsg = "right"; break;
                        case SDLK_a: sendMsg = "left"; break;
                        case SDLK_g: sendMsg = "parry"; break;
                        default: sendMsg = "default"; break;
                    }
                    len = sendMsg.length();
                    if(SDLNet_TCP_Send(*server, sendMsg.c_str(), len + 1) < len + 1) {
                        std::cout << "Unable to send client 1 message! SDLNet Error: " << SDLNet_GetError() << "\n";
                    }
                }
                else if(id == 2 && event.type == SDL_KEYDOWN && event.key.repeat == 0){
                    switch(event.key.keysym.sym) {
                        case SDLK_UP: sendMsg = "up"; break;
                        case SDLK_DOWN: sendMsg = "down"; break;
                        case SDLK_RIGHT: sendMsg = "right"; break;
                        case SDLK_LEFT: sendMsg = "left"; break;
                        case SDLK_m: sendMsg = "parry"; break;
                        default: sendMsg = "default"; break;
                    }
                    len = sendMsg.length();
                    if(SDLNet_TCP_Send(*server, sendMsg.c_str(), len + 1) < len + 1) {
                        std::cout << "Unable to send client 2 message! SDLNet Error: " << SDLNet_GetError() << "\n";
                    }
                }
                else {
                    sendMsg = "default";
                    len = sendMsg.length();
                    if(SDLNet_TCP_Send(*server, sendMsg.c_str(), len + 1) < len + 1) {
                        if(id == 1)
                            std::cout << "Unable to send client 1 message! SDLNet Error: " << SDLNet_GetError() << "\n";
                        else
                            std::cout << "Unable to send client 2 message! SDLNet Error: " << SDLNet_GetError() << "\n";
                    }                   
                }


                if(id == 1) p1.handleEvent(event, SDL_GetKeyboardState(NULL));
                if(id == 2) p2.handleEvent(event, SDL_GetKeyboardState(NULL));
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
            switchGhostMode(2, &g1, &g2, &g3, &g4, NULL, NULL, &g7, NULL);
            changeMode = true;
        }
        if(timeNow >= timeToChangeMode && (!changedMode1 || !changedMode2)) {   
            if(g1.mode != 3 && g1.mode != 4) {
                g5 = g1;
                g5.channel = 5;
                changedMode1 = true;
            }
            if(g2.mode != 3 && g2.mode != 4) {
                g6 = g2;
                g6.channel = 6;
                changedMode2 = true;
            }    
        }
        if(timeNow >= timeToRandomize && !randomized){
            switchGhostMode(1, &g1, &g2, &g3, &g4, &g5, &g6, &g7, NULL);
            randomized = true;
        }
        if(timeNow >= timeToFinale && !finalMode) {
            switchGhostMode(5, &g1, &g2, &g3, &g4, &g5, &g6, &g7, NULL);
            finalMode = true;
        }
        if(timeNow >= timeToFinale && !createNew) {
            if(g3.mode != 3 && g3.mode != 4) {
                g8 = g3;
                g8.channel = 7;
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
    p2.free(); p1.free();
    scoreBoard.free1();
    if(background != NULL) {
        SDL_DestroyTexture(background);
        background = NULL;
    }
    maze.free();
    return false;
}

int main(int argc, char** argv) {
    if(!SDL_init()) {
        return 0;
    }
    // 1023
    Window window("Maze", 1405, 1025);
    if(!window.getSuccess()) {
        close();
        return 0;
    }
    srand(time(0));

    bool quit = false;
    SDL_Event event;
    SDLNet_SocketSet set;
    set = SDLNet_AllocSocketSet(2);
    if(set == NULL) {
        cout << "Could not create socket set! SDLNet Error: " << SDLNet_GetError() << "\n";
        close();
        return 0;
    }
    TCPsocket server;
    IPaddress ip;

    LoadMusic();    
    Menu menu(&window);

    Mix_PlayMusic(bground, -1);
    Mix_VolumeMusic(50);

    bool firstMatch = true;
    int id;

    while(!quit) {
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                quit = true;
                if(server != NULL) {
                    SDLNet_TCP_Close(server);
                }
            }
            menu.handleEvent(event);
        }
        if(menu.isRunning == true) {
            if(menu.mode == 1) 
                quit = game(&menu, &window);
            else if(menu.mode == 2) {
                // transition screen, connecting to server
                if(firstMatch) {
                    if(SDLNet_ResolveHost(&ip, "127.0.0.1", 1234) < 0) {
                        std::cout << "Could not resolve host! SDLNet Error: " << SDLNet_GetError() << "\n";
                    }
                    else {
                        server = SDLNet_TCP_Open(&ip);
                        if(server == NULL) {
                            std::cout << "Could not open socket! SDLNet Error: " << SDLNet_GetError() << "\n";
                        }
                        else {
                            if(SDLNet_TCP_AddSocket(set, server) < 0) {
                                cout << "Could not add server to socket set! SDLNet Error: " << SDLNet_GetError() << "\n";
                            }
                            else {
                                char msg[1000];
                                bool isConnected = false;
                                if(SDLNet_TCP_Recv(server, msg, 1000) > 0){
                                    std::cout << msg << "\n";
                                    if(strcmp(msg, "1") == 0){id = 1;}
                                    else if(strcmp(msg, "2") == 0){id = 2;}
                                }
                                if(SDLNet_TCP_Recv(server, msg, 1000) > 0) {
                                    if(strcmp(msg, "CONNECTED") == 0)
                                        isConnected = true;
                                }
                                if(isConnected) {
                                    std::cout << "Starting...\n";
                                    quit = gameOnline(&menu, &window, id, &server, &set);              
                                }
                            }    
                        }    
                    }
                    firstMatch = false; 
                }
                else {
                    quit = gameOnline(&menu, &window, id, &server, &set);
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
    window.free();
    ClearMusic();
    close();
}

