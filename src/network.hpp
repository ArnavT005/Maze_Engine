#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <cstring>

using namespace std;

TCPsocket server, client1, client2;
 
void initializeNetwork(){
	SDL_Init(SDL_INIT_EVERYTHING);
    SDLNet_Init();
}

void quitNetwork(){
	SDLNet_TCP_Close(server);
    SDLNet_TCP_Close(client1);
    SDLNet_TCP_Close(client2);
    SDLNet_Quit();
    SDL_Quit();
}

void createServer(){
	IPaddress ip;
    SDLNet_ResolveHost(&ip,NULL,1235);
    server = SDLNet_TCP_Open(&ip);
    bool flag = true;
    while(1)
    {
        TCPsocket temp = SDLNet_TCP_Accept(server);
        if(temp){
            if(flag){
                client1 = temp; flag = false;
                cout<<"Client 1 Connected\n";
            }
            else{
                client2 = temp;
                cout<<"Client 2 Connected\n";
                break;
            }
        }
    }
}

void createClient(){
    IPaddress ip;
    SDLNet_ResolveHost(&ip,"127.0.0.1",1235);
   	server = SDLNet_TCP_Open(&ip);
}

void sendToServer(const char* s){
	SDLNet_TCP_Send(server, s, strlen(s)+1);
}

char[] recieveAtClient(){
    char text[10000];
    SDLNet_TCP_Recv(server, text, 10000);
    return text;
}

void sendToClient(const char* s, int x){
    if(x == 1) {SDLNet_TCP_Send(client1, s, strlen(s)+1);}
    else {SDLNet_TCP_Send(client2, s, strlen(s)+1);}
}

char[] recieveAtServer(int x){
	char text[10000];
    if (x == 1) {SDLNet_TCP_Recv(client1, text, 10000);}
    else {SDLNet_TCP_Recv(client2, text, 10000);}
	return text;
}
