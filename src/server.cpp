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
    char id1[1000] = "1", id2[1000] = "2", connected[1000] = "CONNECTED";
    while(client_cnt < 2)
    {
        if (client_cnt == 0) {
            client1 = SDLNet_TCP_Accept(server);
            if(client1) {
            	SDLNet_TCP_Send(client1, id1, 1000);
            	cout<<"Client 1 Connected\n"; client_cnt++;}
        }
        if (client_cnt == 1) {
            client2 = SDLNet_TCP_Accept(server);
            if(client2) {
            	SDLNet_TCP_Send(client2, id2, 1000);
                cout<<"Client 2 Connected\n"; client_cnt++;}
        }
    }
    SDLNet_TCP_Send(client1, connected, 1000);
    SDLNet_TCP_Send(client2, connected, 1000);
    while(1){
        char msg1[1000];
        char msg2[1000];
        if(SDLNet_TCP_Recv(client1, msg1, 1000) > 0){
        	cout << "Hello1\n";
            cout << msg1 << "\n";
            if(SDLNet_TCP_Send(client2, msg1, 1000) < 1000)
                cout << "Error\n";
        }
        if(SDLNet_TCP_Recv(client2, msg2, 1000) > 0){
        	cout << "Hello2\n";
            cout << msg2 << "\n";
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
