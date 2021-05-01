#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <cstring>

using namespace std;

TCPsocket server, client1, client2;


int main(){
	SDL_Init(SDL_INIT_EVERYTHING);
    SDLNet_Init();
    IPaddress ip;
    SDLNet_ResolveHost(&ip,NULL,1235);
    server = SDLNet_TCP_Open(&ip);
    int client_cnt = 0;
    while(client_cnt < 2)
    {
        if (client_cnt == 0) {
            client1 = SDLNet_TCP_Accept(server);
            if(client1) {cout<<"Client 1 Connected\n"; client_cnt++;}
        }
        if (client_cnt == 1) {
            client2 = SDLNet_TCP_Accept(server);
            if(client2) {cout<<"Client 2 Connected\n"; client_cnt++;}
        }
    }
    SDL_Event e;
    while(1){
        char msg1[1000];
        char msg2[1000];
        if(SDLNet_TCP_Recv(client1, msg1, 1000) > 0){
        	SDLNet_TCP_Send(client2, msg1, 1000);
        }
        if(SDLNet_TCP_Recv(client2, msg2, 1000) > 0){
        	SDLNet_TCP_Send(client1, msg2, 1000);
        }
    }
    SDLNet_TCP_Close(server);
    SDLNet_TCP_Close(client1);
    SDLNet_TCP_Close(client2);
    SDLNet_Quit();
    SDL_Quit();
    return 0;
}
