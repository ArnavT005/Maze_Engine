#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include "sound.hpp"
#include "window.hpp"
#include "maze.hpp"
#include "pacman.hpp"
#include "ghost.hpp"
#include "manager.hpp"
#include "scoreboard.hpp"
#include "client.hpp"

int timeToChangeMode = 34000;
int timeToRandomize = 64000;
int timeToFinale = 74000;
int finishTime = 104000;
bool changeMode = false, changedMode1 = false, changedMode2 = false; 
bool finalMode = false, createNew = false;
SDL_Point points[952576];

bool SDL_init() {
    bool success = true;
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
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
        }

    }
    return success;
}

void close() {
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
    else if(time > finishTime) {
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
    board->livesP1Text << "Lives: " << p1->lives;
    board->livesP2Text.str("");
    board->livesP2Text << "Lives: " << p2->lives;
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

    Ghost g1(&maze, TYPE_BLINKY, 1, &window, 0);
    Ghost g2(&maze, TYPE_PINKY, 1, &window, 1);
    Ghost g3(&maze, TYPE_INKY, 1, &window, 2);
    Ghost g4(&maze, TYPE_CLYDE, 1, &window, 3);
	Ghost g5;
	Ghost g6;
	Ghost g7(&maze, TYPE_CLYDE, 2, &window, 4);
    Ghost g8;
    window.clearWindow();

    bool quit = false;
    SDL_Event event;
    bool temp;
    Uint32 startTime = SDL_GetTicks();

    scoreBoard.start = startTime;
    bool randomized = false;
    LoadMusic();
    Mix_PlayMusic(bground, 0);
    Mix_VolumeMusic(50);
    bool startGame = true;
    bool timer = false;
    cout<<"hi0";
    connectToServer();
    string sendMsg;
    char recvdMsg[1000];
    int id;
    cout<<"hi1";
    if(SDLNet_TCP_Recv(server, recvdMsg, 1000) > 0){
	    if(strcmp(recvdMsg, "1") == 0){id = 1;}
	    else if(strcmp(recvdMsg, "2") == 0){id = 2;}
	}
    cout<<"hi";
    while(!quit) {
        if(!timer && SDL_GetTicks() - startTime > finishTime - 9000) {
            Mix_PlayChannel(18, tenSecTimer, 0);
            timer = true;
        }
        int i = 0;
        temp = false;
        if(SDL_GetTicks() - startTime >= 4000) {
            while(SDL_PollEvent(& event)) {
                if(event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) ) {
                    quit = true;
                }
                if(id==1 && event.type == SDL_KEYDOWN && event.key.repeat == 0){
                    switch(event.key.keysym.sym) {
						case SDLK_UP: sendMsg = "up"; break;
						case SDLK_DOWN: sendMsg = "down"; break;
						case SDLK_RIGHT: sendMsg = "right"; break;
						case SDLK_LEFT: sendMsg = "left"; break;
						case SDLK_m: sendMsg = "parry"; break;
						default: break;
					}
                	SDLNet_TCP_Send(server, sendMsg.c_str(), strlen(sendMsg.c_str()) + 1);
                }
                if(id == 2 && event.type == SDL_KEYDOWN && event.key.repeat == 0){
                    switch(event.key.keysym.sym) {
						case SDLK_w: sendMsg = "up"; break;
						case SDLK_s: sendMsg = "down"; break;
						case SDLK_d: sendMsg = "right"; break;
						case SDLK_a: sendMsg = "left"; break;
						case SDLK_g: sendMsg = "parry"; break;
						default: break;
					}
                	SDLNet_TCP_Send(server, sendMsg.c_str(), strlen(sendMsg.c_str()) + 1);
                }
                SDL_Event E;
                E.type = SDL_KEYDOWN;
                E.key.repeat = 0;
                if(SDLNet_TCP_Recv(server, recvdMsg, 1000) > 0){
        			if(strcmp(recvdMsg, "up") == 0){if(id==1){E.key.keysym.sym = SDLK_w;} else { E.key.keysym.sym = SDLK_UP;}}
        			if(strcmp(recvdMsg, "down") == 0){if(id==1){E.key.keysym.sym = SDLK_s;} else { E.key.keysym.sym = SDLK_DOWN;}}
        			if(strcmp(recvdMsg, "left") == 0){if(id==1){E.key.keysym.sym = SDLK_a;} else { E.key.keysym.sym = SDLK_LEFT;}}
        			if(strcmp(recvdMsg, "right") == 0){if(id==1){E.key.keysym.sym = SDLK_d;} else { E.key.keysym.sym = SDLK_RIGHT;}}
        			if(strcmp(recvdMsg, "parry") == 0){if(id==1){E.key.keysym.sym = SDLK_g;} else { E.key.keysym.sym = SDLK_m;}}
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
        		
        window.clearWindow();
        window.renderTexture(background, NULL, &bg);
        p1.move();
        p2.move();
        p1.pacpacCollision(&p2);
        for(i = 0; i < numEat; i ++) {
            manager.eatables[i].checkIfEaten(temp);
            manager.eatables[i].render(&window);
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
        manager.renderPortals(&window);
        RenderElements(&p1, &p2, &g1, &g2, &g3, &g4, &g5, &g6, &g7, &g8, timeNow, &window);
        renderBlackScreen(&p1, &p2, &window, timeNow);
        window.updateWindow();
        if(startGame) {
            Mix_PlayChannel(12, start, 0);
            startGame = false;
        }
        if(SDL_GetTicks() - startTime > finishTime + 1000) {
            break;
        }
    }
    Mix_HaltMusic();
    Mix_HaltChannel(-1);
    SDL_Delay(5000);
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
    disconnectToServer();
    maze.free();
    window.free();
    ClearMusic();
    close();
}

