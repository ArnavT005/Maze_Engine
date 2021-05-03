#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <cstring>
#include <string>
#include <vector>

using namespace std;


bool SDL_init() {
    bool success = true;
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cout << "SDL unable to initialize! SDL Error: " << SDL_GetError() << "\n";
        success = false;
    }
    else {
        if(SDLNet_Init() < 0) {
            std::cout << "SDLNet could not initialize! SDLNet Error: " << SDLNet_GetError() << "\n";
            success = false;
        }

    }
    return success;
}

void close() {
    SDLNet_Quit();
    SDL_Quit();
}

// int length(char ch[], int n) {
//     int len = 0;
//     for(int i = 0; i < n; i ++) {
//         if(ch[i] == '\0') {
//             len = i;
//             break;
//         }
//     }
//     return len;
// }   

int main(){
	
    if(!SDL_init()) {
        return 0;
    }
    SDLNet_SocketSet set;
    set = SDLNet_AllocSocketSet(3);
    if(set == NULL) {
        cout << "Could not create socket set! SDLNet Error: " << SDLNet_GetError() << "\n";
        close();
        return 0;
    }
    TCPsocket server;
    vector<TCPsocket> client(2);
    IPaddress ip;

    if(SDLNet_ResolveHost(&ip, NULL, 1234) < 0) {
        cout << "Could not resolve server! SDLNet Error: " << SDLNet_GetError() << "\n";
        SDLNet_FreeSocketSet(set);
        set = NULL;
        close();
        return 0;
    } 
    else {   
        server = SDLNet_TCP_Open(&ip);
        if(server == NULL) {
            cout << "Could not open listening socket! SDLNet Error: " << SDLNet_GetError() << "\n";
            SDLNet_FreeSocketSet(set);
            set = NULL;
            close();
            return 0;
        }
        else {
            if(SDLNet_TCP_AddSocket(set, server) < 0) {
                cout << "Could not add server to socket set! SDLNet Error: " << SDLNet_GetError() << "\n";
                SDLNet_TCP_Close(server);
                SDLNet_FreeSocketSet(set);
                set = NULL;
                close();
                return 0;
            }
            int client_cnt = 0;
            string id1 = "1", id2 = "2";
            string connected = "CONNECTED", def = "default";
            
            // connect to two clients
            while(client_cnt < 2)
            {
                if (client_cnt == 0) {
                    client[0] = SDLNet_TCP_Accept(server);
                    if(client[0] != NULL) {
                        if(SDLNet_TCP_AddSocket(set, client[0]) < 0) {
                            cout << "Could not add client 1 to socket set! SDLNet Error: " << SDLNet_GetError() << "\n";
                            SDLNet_TCP_Close(client[0]);
                            SDLNet_TCP_DelSocket(set, server);
                            SDLNet_TCP_Close(server);
                            SDLNet_FreeSocketSet(set);
                            set = NULL;
                            close();
                            return 0;
                        }
                    	if(SDLNet_TCP_Send(client[0], id1.c_str(), 2) < 2) {
                            cout << "Unable to send message to client 2! SDLNet Error: " << SDLNet_GetError() << "\n";
                            SDLNet_TCP_DelSocket(set, client[0]);
                            SDLNet_TCP_Close(client[0]);
                            SDLNet_TCP_DelSocket(set, server);
                            SDLNet_TCP_Close(server);
                            SDLNet_FreeSocketSet(set);
                            set = NULL;
                            close();
                            return 0;
                        }
                    	cout<<"Client 1 Connected\n"; 
                        client_cnt++;
                    }
                }
                if (client_cnt == 1) {
                    client[1] = SDLNet_TCP_Accept(server);
                    if(client[1] != NULL) {
                    	if(SDLNet_TCP_AddSocket(set, client[1]) < 0) {
                            cout << "Could not add client 2 to socket set! SDLNet Error: " << SDLNet_GetError() << "\n";
                            SDLNet_TCP_Close(client[1]);
                            SDLNet_TCP_DelSocket(set, client[0]);
                            SDLNet_TCP_Close(client[0]);
                            SDLNet_TCP_DelSocket(set, server);
                            SDLNet_TCP_Close(server);
                            SDLNet_FreeSocketSet(set);
                            set = NULL;
                            close();
                            return 0;
                        }
                        if(SDLNet_TCP_Send(client[1], id2.c_str(), 2) < 2) {
                            cout << "Unable to send message to client 2! SDLNet Error: " << SDLNet_GetError() << "\n";
                            SDLNet_TCP_DelSocket(set, client[1]);
                            SDLNet_TCP_Close(client[1]);
                            SDLNet_TCP_DelSocket(set, client[0]);
                            SDLNet_TCP_Close(client[0]);
                            SDLNet_TCP_DelSocket(set, server);
                            SDLNet_TCP_Close(server);
                            SDLNet_FreeSocketSet(set);
                            set = NULL;
                            close();
                            return 0;
                        }
                        cout<<"Client 2 Connected\n"; 
                        client_cnt++;
                    }
                }
            }
            // connected to both clients

            if(SDLNet_TCP_Send(client[0], connected.c_str(), 10) < 10) {
                cout << "Unable to send message to client 1! SDLNet Error: " << SDLNet_GetError() << "\n";
                SDLNet_TCP_DelSocket(set, client[1]);
                SDLNet_TCP_Close(client[1]);
                SDLNet_TCP_DelSocket(set, client[0]);
                SDLNet_TCP_Close(client[0]);
                SDLNet_TCP_DelSocket(set, server);
                SDLNet_TCP_Close(server);
                SDLNet_FreeSocketSet(set);
                set = NULL;
                close();
                return 0;
            } 
            if(SDLNet_TCP_Send(client[1], connected.c_str(), 10) < 10) {
                cout << "Unable to send message to client 2! SDLNet Error: " << SDLNet_GetError() << "\n";                
                SDLNet_TCP_DelSocket(set, client[1]);
                SDLNet_TCP_Close(client[1]);
                SDLNet_TCP_DelSocket(set, client[0]);
                SDLNet_TCP_Close(client[0]);
                SDLNet_TCP_DelSocket(set, server);
                SDLNet_TCP_Close(server);
                SDLNet_FreeSocketSet(set);
                set = NULL;
                close();
                return 0;
            }       

            // communicate with clients
            // used to store messages
            char msg1[1000], msg2[1000];
            int len1, len2, activity1, activity2;

            while(1){
                // check for socket activity, do not wait
                if(SDLNet_CheckSockets(set, 0) < 0) {
                    cout << "Error generated while checking sockets! SDLNet Error: " << SDLNet_GetError() << "\n";
                    SDLNet_TCP_DelSocket(set, client[1]);
                    SDLNet_TCP_Close(client[1]);
                    SDLNet_TCP_DelSocket(set, client[0]);
                    SDLNet_TCP_Close(client[0]);
                    SDLNet_TCP_DelSocket(set, server);
                    SDLNet_TCP_Close(server);
                    SDLNet_FreeSocketSet(set);
                    set = NULL;
                    close();
                    return 0;
                }
                else {
                    // check for activity of client[0]
                    activity1 = SDLNet_SocketReady(client[0]);
                    activity2 = SDLNet_SocketReady(client[1]);
                    if(activity1 != 0) {
                        len1 = SDLNet_TCP_Recv(client[0], msg1, 1000);
                        if(len1 > 0) {
                            // send message to client 2
                            string s = "";
                            for(int i = 0; i < len1 - 1; i ++) {
                                s = s + msg1[i];
                            }
                            if(SDLNet_TCP_Send(client[1], s.c_str(), len1) < len1) {
                                cout << "Unable to send client 1 message to client 2! SDLNet Error: " << SDLNet_GetError() << "\n";                
                                SDLNet_TCP_DelSocket(set, client[1]);
                                SDLNet_TCP_Close(client[1]);
                                SDLNet_TCP_DelSocket(set, client[0]);
                                SDLNet_TCP_Close(client[0]);
                                SDLNet_TCP_DelSocket(set, server);
                                SDLNet_TCP_Close(server);
                                SDLNet_FreeSocketSet(set);
                                set = NULL;
                                close();
                                return 0;
                            }
                        }
                    }
                    if(activity2 != 0) {
                        len2 = SDLNet_TCP_Recv(client[1], msg2, 1000);
                        if(len2 > 0) {
                            // send message to client 1
                            string s = "";
                            for(int i = 0; i < len2 - 1; i ++) {
                                s = s + msg2[i];
                            }
                            if(SDLNet_TCP_Send(client[0], s.c_str(), len2) < len2) {
                                cout << "Unable to send client 2 message to client 1! SDLNet Error: " << SDLNet_GetError() << "\n";                
                                SDLNet_TCP_DelSocket(set, client[1]);
                                SDLNet_TCP_Close(client[1]);
                                SDLNet_TCP_DelSocket(set, client[0]);
                                SDLNet_TCP_Close(client[0]);
                                SDLNet_TCP_DelSocket(set, server);
                                SDLNet_TCP_Close(server);
                                SDLNet_FreeSocketSet(set);
                                set = NULL;
                                close();
                                return 0;
                            }
                        } 
                    }
                }
            }
        }  
    }      
    return 0;
}
