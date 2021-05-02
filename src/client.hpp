#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>

using namespace std;

TCPsocket server;

void connectToServer(){
    SDLNet_Init();
    IPaddress ip;
    SDLNet_ResolveHost(&ip,"127.0.0.1",1235);
    server = SDLNet_TCP_Open(&ip);
}

void disconnectToServer(){
	SDLNet_TCP_Close(server);
    SDLNet_Quit();
}

