#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <cstring>

using namespace std;

TCPsocket server, client;
 
void initializeNetwork(){
	SDL_Init(SDL_INIT_EVERYTHING);
    SDLNet_Init();
}

void quitNetwork(){
	SDLNet_TCP_Close(server);
    SDLNet_TCP_Close(client);
    SDLNet_Quit();
    SDL_Quit();
}

void createServer(){
	IPaddress ip;
    SDLNet_ResolveHost(&ip,NULL,1235);
    server = SDLNet_TCP_Open(&ip);
    while(1)
    {
        TCPsocket temp = SDLNet_TCP_Accept(server);
        if(temp) cout<<"Client Connected\n"; break;
    }
}

void createClient(){
    IPaddress ip;
    SDLNet_ResolveHost(&ip,"127.0.0.1",1235);
   	client = SDLNet_TCP_Open(&ip);
}

void sendToServer(const char* s){
	SDLNet_TCP_Send(server, s, strlen(s)+1);
}

void sendToClient(const char* s){
	SDLNet_TCP_Send(client, s, strlen(s)+1);
}

char[] recieveAtClient(){
	char text[10000];
	SDLNet_TCP_Recv(client, text, 10000);
	return text;
}

char[] recieveAtServer(){
	char text[10000];
	SDLNet_TCP_Recv(server, text, 10000);
	return text;
}
