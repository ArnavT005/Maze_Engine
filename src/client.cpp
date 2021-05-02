#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>

using namespace std;

TCPsocket server;

void connectToServer(){
	SDL_Init(SDL_INIT_EVERYTHING);
    SDLNet_Init();
    IPaddress ip;
    SDLNet_ResolveHost(&ip,"127.0.0.1",1235);
    server = SDLNet_TCP_Open(&ip);
}

void disconnectToServer(){
	SDLNet_TCP_Close(server);
    SDLNet_Quit();
}

int main() {
	connectToServer();
	char msg1[1000];
	SDLNet_TCP_Recv(server, msg1, 1000);
	cout << msg1 << "\n";
	const char* s;
	string x;
	while(1) {
	cin >> x;
	s = x.c_str();
	int result = SDLNet_TCP_Send(server, s, 1000);
	if(result < 1000) {
		cout << "Error\n";
	}
		int n = SDLNet_TCP_Recv(server, msg1, 1000);
		if(n > 0) {
			cout << msg1;
		}

	}
	return 0;
}

