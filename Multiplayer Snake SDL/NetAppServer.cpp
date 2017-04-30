#include <iostream>
#include <math.h>
#include "NetApp.h"
#include "snake.h"
#include "Bullet.h"
#include "level.h"

Snake_TCP_Data sendData;
Snake_TCP_Data receivedData;
int numOfPlayers;

extern bool HandleWaitForPlayersEvents();
extern bool restart;

// *** CNetHost ***
CNetHost::CNetHost()
{
    SDLNet_ResolveHost(&ip, NULL, 1234);

    server_socket = SDLNet_TCP_Open(&ip);

    socket_set = SDLNet_AllocSocketSet(MAX_SOCKETS+1);

    if(SDLNet_TCP_AddSocket(socket_set, server_socket) == -1) {
        std::cout << "ER: SDLNet_TCP_AddSocket: %s\n" << SDLNet_GetError() << std::endl;
    }

    next_ind = 0;
    numOfPlayers = 1;

    sendData.playerColor = playerColor;
    sendData.nextSnakeDir = DIR_UP;
    sendData.currSnakeDir = DIR_UP;
    sendData.fireBullet = false;
    sendData.restartLevel = false;
    sendData.dead = false;
    sendData.pos.x = sendData.pos.y = 0;

    for(int idx=0;idx<MAX_SOCKETS;idx++) sockets[idx] = 0;
}

void CNetHost::CloseSocket(int index) {
	if(sockets[index] == NULL) {
		fprintf(stderr, "ER: Attempted to delete a NULL socket.\n");
		return;
	}

	if(SDLNet_TCP_DelSocket(socket_set, sockets[index]) == -1) {
		fprintf(stderr, "ER: SDLNet_TCP_DelSocket: %s\n", SDLNet_GetError());
		exit(-1);
	}

	memset(&clients[index], 0x00, sizeof(Client));
	SDLNet_TCP_Close(sockets[index]);
	sockets[index] = NULL;
}

CNetHost::~CNetHost()
{
    SDLNet_TCP_DelSocket(socket_set, server_socket);
    SDLNet_TCP_Close(server_socket);

    int i;
    for(i=0; i<MAX_SOCKETS; ++i) {
        if(sockets[i] == NULL) continue;
        CloseSocket(i);
    }

    SDLNet_FreeSocketSet(socket_set);
}

int CNetHost::AcceptSocket(int index) {
	if(sockets[index]) {
		fprintf(stderr, "ER: Overriding socket at index %d.\n", index);
		CloseSocket(index);
	}

	sockets[index] = SDLNet_TCP_Accept(server_socket);
	if(sockets[index] == NULL) return 0;

	clients[index].in_use = 1;
	if(SDLNet_TCP_AddSocket(socket_set, sockets[index]) == -1) {
		fprintf(stderr, "ER: SDLNet_TCP_AddSocket: %s\n", SDLNet_GetError());
		exit(-1);
	}

	return 1;
}

void CNetHost::HandleConnection()
{
    int num_rdy = SDLNet_CheckSockets(socket_set, TIMEOUT);

    if(num_rdy <= 0) {
        // NOTE: none of the sockets are ready, update all snake positions
        /*for(int idx=0; (idx<MAX_SOCKETS); ++idx) {
            if(sockets[idx] == NULL) continue;

            for(unsigned int i=0;i<snakes.size();i++) {
                sendData.pos = snakes[i].pos;

                unsigned int num_sent = SDLNet_TCP_Send(sockets[idx], &sendData, sizeof(sendData));
                if(num_sent < sizeof(sendData)) {
                    fprintf(stderr, "ER: SDLNet_TCP_Send: %s\n", SDLNet_GetError());
                    CloseSocket(idx);
                }
            }
        }*/
    } else {
        // NOTE: some number of the sockets are ready
        bool skip = false;

        if(SDLNet_SocketReady(server_socket)) {
            int got_socket = AcceptSocket(next_ind);
            if(!got_socket) {
                num_rdy--;
                skip = true;
            } else {
                printf("New connection (curr_ind = %d)\n", next_ind);

                char plyColor[1];
                plyColor[0] = SNK_SPECTATE;
                std::cout << "Spectator joined the game!" << std::endl;

                unsigned int num_sent = SDLNet_TCP_Send(sockets[next_ind], plyColor, 1);
                if(num_sent < 1) {
                    fprintf(stderr, "ER: SDLNet_TCP_Send: %s\n", SDLNet_GetError());
                    CloseSocket(next_ind);
                }

                Player player;
                player.playerColor = plyColor[0];
                player.startGame = 1;
                player.numOfPlayers = numOfPlayers;

                num_sent = SDLNet_TCP_Send(sockets[next_ind], &player, sizeof(player));
                if(num_sent < sizeof(player)) {
                    fprintf(stderr, "ER: SDLNet_TCP_Send: %s\n", SDLNet_GetError());
                    CloseSocket(next_ind);
                }

                // NOTE: get a new index
                int chk_count;
                for(chk_count=0; chk_count<MAX_SOCKETS; ++chk_count) {
                    if(sockets[(next_ind+chk_count)%MAX_SOCKETS] == NULL) break;
                }

                next_ind = (next_ind+chk_count)%MAX_SOCKETS;

                num_rdy--;
            }
        }

        if(!skip) {
            int index;
            for(index=0; (index<MAX_SOCKETS) && num_rdy; ++index) {
                if(sockets[index] == NULL) continue;
                if(!SDLNet_SocketReady(sockets[index])) continue;

                int num_recv = SDLNet_TCP_Recv(sockets[index], &receivedData, sizeof(receivedData));

                if(num_recv <= 0) {
                    CloseSocket(index);
                    num_rdy--;
                    continue;
                }

                SynchronizeSnakePos();
                if(receivedData.dead) snakes[receivedData.playerColor].dead = true;

                for(int idx=0; (idx<MAX_SOCKETS); ++idx) {
                    if(sockets[idx] == NULL) continue;
                    if(idx==index) continue;

                    unsigned int num_sent = SDLNet_TCP_Send(sockets[idx], &receivedData, sizeof(receivedData));
                    if(num_sent < sizeof(receivedData)) {
                        fprintf(stderr, "ER: SDLNet_TCP_Send: %s\n", SDLNet_GetError());
                        CloseSocket(idx);
                    }
                }

                // Synchronize timing
                if(receivedData.fireBullet) CreateBullet(&snakes[receivedData.playerColor]);
                previousTime = currentTime = SDL_GetTicks();

                num_rdy--;
            }
        }
    }
}

void CNetHost::SendMsg(char snkIdx)
{
    // NOTE: some number of the sockets are ready
    for(int idx=0; (idx<MAX_SOCKETS); ++idx) {
        if(sockets[idx] == NULL) continue;

        sendData.playerColor = snakes[snkIdx].color;
        sendData.nextSnakeDir = snakes[snkIdx].nextDir;
        sendData.currSnakeDir = snakes[snkIdx].currDir;
        sendData.fireBullet = fireBullet;
        sendData.restartLevel = restart;
        sendData.pos = snakes[snkIdx].pos;
        sendData.dead = snakes[snkIdx].dead;

        unsigned int num_sent = SDLNet_TCP_Send(sockets[idx], &sendData, sizeof(sendData));
        if(num_sent < sizeof(sendData)) {
            fprintf(stderr, "ER: SDLNet_TCP_Send: %s\n", SDLNet_GetError());
            CloseSocket(idx);
        }
    }

    if(fireBullet) {
        CreateBullet(&snakes[playerColor]);
        fireBullet = false;
    }
    // Synchronize timing
    previousTime = currentTime = SDL_GetTicks();
}

bool CNetHost::WaitForOtherPlayers() {
    Player player;
    player.playerColor = 0;
    player.startGame = 0;
    bool startGame = HandleWaitForPlayersEvents();
    if(startGame && numOfPlayers>=2) {
        // Start game
        for(int idx=0; (idx<MAX_SOCKETS); ++idx) {
            if(sockets[idx] == NULL) continue;

            player.startGame = 1;

            unsigned int num_sent = SDLNet_TCP_Send(sockets[idx], &player, sizeof(player));
            if(num_sent < sizeof(player)) {
                fprintf(stderr, "ER: SDLNet_TCP_Send: %s\n", SDLNet_GetError());
                CloseSocket(idx);
            }
        }
        return true;
    } else if (startGame) {
        std::cout << "Not enough players!" << std::endl;
    }

    int num_rdy = SDLNet_CheckSockets(socket_set, 0);

    if(num_rdy <= 0) {

    } else {
        // NOTE: some number of the sockets are ready
        if(SDLNet_SocketReady(server_socket)) {
            int got_socket = AcceptSocket(next_ind);
            if(!got_socket) {
                num_rdy--;
            } else {
                printf("New connection (curr_ind = %d)\n", next_ind);

                char plyColor[1];
                if(numOfPlayers<4) {
                    if(numOfPlayers==1) {
                        plyColor[0] = SNK_YEL;
                        std::cout << "Yellow player joined the game!" << std::endl;
                    }
                    if(numOfPlayers==2) {
                        plyColor[0] = SNK_BLU;
                        std::cout << "Blue player joined the game!" << std::endl;
                    }
                    if(numOfPlayers==3) {
                        plyColor[0] = SNK_GRN;
                        std::cout << "Green player joined the game!" << std::endl;
                    }
                    numOfPlayers++;
                } else {
                    plyColor[0] = SNK_SPECTATE;
                    std::cout << "Spectator joined the game!" << std::endl;
                }

                unsigned int num_sent = SDLNet_TCP_Send(sockets[next_ind], plyColor, 1);
                if(num_sent < 1) {
                    fprintf(stderr, "ER: SDLNet_TCP_Send: %s\n", SDLNet_GetError());
                    CloseSocket(next_ind);
                }

                player.playerColor = plyColor[0];
                player.numOfPlayers = numOfPlayers;

                for(int idx=0; (idx<MAX_SOCKETS); ++idx) {
                    if(sockets[idx] == NULL) continue;

                    unsigned int num_sent = SDLNet_TCP_Send(sockets[idx], &player, sizeof(player));
                    if(num_sent < sizeof(player)) {
                        fprintf(stderr, "ER: SDLNet_TCP_Send: %s\n", SDLNet_GetError());
                        CloseSocket(idx);
                    }
                }

                // NOTE: get a new index
                int chk_count;
                for(chk_count=0; chk_count<MAX_SOCKETS; ++chk_count) {
                    if(sockets[(next_ind+chk_count)%MAX_SOCKETS] == NULL) break;
                }

                next_ind = (next_ind+chk_count)%MAX_SOCKETS;

                num_rdy--;
            }
        }
    }
    return false;
}
