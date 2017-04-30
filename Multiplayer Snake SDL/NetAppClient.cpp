#include <iostream>
#include <math.h>
#include "NetApp.h"
#include "snake.h"
#include "Bullet.h"
#include "level.h"

extern Snake_TCP_Data sendData;
extern Snake_TCP_Data receivedData;
extern bool restart;

// *** CNetClient ***
CNetClient::CNetClient()
{
    //tcpclient = new CClientSocket();

    std::cout << "Enter IP address of the Server: ";
    bool valid = false;
    do
    {
        std::string ipstr;
        std::cin >> ipstr;
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        //CIpAddress ipAddr(ip, 1234);
        if(SDLNet_ResolveHost(&ip, ipstr.c_str(), 1234) < 0)
        {
            //remoteip = new CIpAddress(ipAddr);
            std::cerr << "SDLNet_ResolveHost: " << SDLNet_GetError() << std::endl;
            std::cerr << "Could not connect to IP address!" << std::endl;
            std::cout << "Enter IP address of the Server: ";
        }
        else
        {
            valid = true;
        }
    }
    while(!valid);

    socket = SDLNet_TCP_Open(&ip);
    socket_set = SDLNet_AllocSocketSet(1);
    SDLNet_TCP_AddSocket(socket_set, socket);

    sendData.playerColor = playerColor;
    sendData.nextSnakeDir = DIR_UP;
    sendData.currSnakeDir = DIR_UP;
    sendData.fireBullet = false;
    sendData.restartLevel = false;
    sendData.dead = false;
    sendData.pos.x = sendData.pos.y = 0;
}

void CNetClient::CloseSocket(void) {
	if(SDLNet_TCP_DelSocket(socket_set, socket) == -1) {
		fprintf(stderr, "%s\n", SDLNet_GetError());
		exit(-1);
	}

	SDLNet_FreeSocketSet(socket_set);
	SDLNet_TCP_Close(socket);
}

CNetClient::~CNetClient()
{
	CloseSocket();
}

bool CNetClient::CheckSocket(void) {
	if(SDLNet_CheckSockets(socket_set, TIMEOUT) == -1) {
		fprintf(stderr, "%s\n", SDLNet_GetError());
		exit(-1);
	}

	return SDLNet_SocketReady(socket);
}

void CNetClient::HandleConnection()
{
    if(CheckSocket()) {
        int num_recv = SDLNet_TCP_Recv(socket, &receivedData, sizeof(receivedData));

        if(num_recv <= 0) {
            CloseSocket();

            const char* err = SDLNet_GetError();
            if(strlen(err) == 0) {
                printf("DB: server shutdown\n");
            } else {
                fprintf(stderr, "ER: SDLNet_TCP_Recv: %s\n", err);
            }
            exit(-1);
        } else {

            SynchronizeSnakePos();
            if(receivedData.fireBullet)   CreateBullet(&snakes[receivedData.playerColor]);
            if(receivedData.dead)         snakes[receivedData.playerColor].dead = true;
            if(receivedData.restartLevel) restart = true;
        }

    }
}

void CNetClient::SendMsg(char snkIdx)
{
    sendData.playerColor = snakes[snkIdx].color;
    sendData.nextSnakeDir = snakes[snkIdx].nextDir;
    sendData.currSnakeDir = snakes[snkIdx].currDir;
    sendData.fireBullet = fireBullet;
    sendData.pos = snakes[snkIdx].pos;
    sendData.dead = snakes[snkIdx].dead;

	unsigned int num_sent = SDLNet_TCP_Send(socket, &sendData, sizeof(sendData));
	if(num_sent < sizeof(sendData)) {
		fprintf(stderr, "ER: SDLNet_TCP_Send: %s\n", SDLNet_GetError());
		CloseSocket();
	}

    if(fireBullet) {
        CreateBullet(&snakes[playerColor]);
        fireBullet = false;
    }
    // Synchronize timing
    previousTime = currentTime = SDL_GetTicks();

	// Wait for server response
	/*if(SDLNet_CheckSockets(socket_set, 2000) == -1) {
		fprintf(stderr, "%s\n", SDLNet_GetError());
		exit(-1);
	}
	if(SDLNet_SocketReady(socket)) {
        int num_recv = SDLNet_TCP_Recv(socket, &receivedData, sizeof(receivedData));

        if(num_recv <= 0) {
            CloseSocket();

            const char* err = SDLNet_GetError();
            if(strlen(err) == 0) {
                printf("Lost connection!\n");
            } else {
                fprintf(stderr, "ER: SDLNet_TCP_Recv: %s\n", err);
            }
            exit(-1);
        } else {
        }
	}*/
}

void SynchronizeSnakePos() {
    snakes[receivedData.playerColor].nextDir = receivedData.nextSnakeDir;
    snakes[receivedData.playerColor].currDir = receivedData.currSnakeDir;
    snakes[receivedData.playerColor].pos = receivedData.pos;
    snakes[receivedData.playerColor].screenPos.x = roundf(receivedData.pos.x);
    snakes[receivedData.playerColor].screenPos.y = roundf(receivedData.pos.y);
    if(receivedData.dead) UpdateAliveSnakeMove(receivedData.playerColor);
}

char CNetClient::GetColor() {
    char plyColor[1];
    plyColor[0] = playerColor;

	if(SDLNet_CheckSockets(socket_set, 2000) == -1) {
		fprintf(stderr, "%s\n", SDLNet_GetError());
		exit(-1);
	}

	if(SDLNet_SocketReady(socket)) {
	    int num_recv = SDLNet_TCP_Recv(socket, plyColor, 1);

        if(num_recv <= 0) {
            CloseSocket();

            const char* err = SDLNet_GetError();
            if(strlen(err) == 0) {
                printf("DB: server shutdown\n");
            } else {
                fprintf(stderr, "ER: SDLNet_TCP_Recv: %s\n", err);
            }
            exit(-1);
        } else {

        }
	} else {
	    std::cout << "Server timed out!" << std::endl;
	    exit(-1);
	}

    switch(plyColor[0]) {
    case SNK_BLU:
        std::cout << "Your color is: Blue" << std::endl;
        break;
    case SNK_YEL:
        std::cout << "Your color is: Yellow" << std::endl;
        break;
    case SNK_GRN:
        std::cout << "Your color is: Green" << std::endl;
        break;
    case SNK_SPECTATE:
        std::cout << "You are a Spectator!" << std::endl;
        break;
    }
    sendData.playerColor = plyColor[0];

	return plyColor[0];
}

bool CNetClient::WaitForOtherPlayers() {
    Player player;
    player.playerColor = 0;
    player.startGame = 0;
    player.numOfPlayers = 0;

    if(SDLNet_CheckSockets(socket_set, 0) == -1) {
		fprintf(stderr, "%s\n", SDLNet_GetError());
		exit(-1);
	}

	if(SDLNet_SocketReady(socket)) {
	    int num_recv = SDLNet_TCP_Recv(socket, &player, sizeof(player));

        if(num_recv <= 0) {
            CloseSocket();

            const char* err = SDLNet_GetError();
            if(strlen(err) == 0) {
                printf("DB: server shutdown\n");
            } else {
                fprintf(stderr, "ER: SDLNet_TCP_Recv: %s\n", err);
            }
            exit(-1);
        } else if(!player.startGame) {
            switch(player.playerColor) {
            case SNK_YEL:
                std::cout << "Yellow player joined the game!" << std::endl;
                break;
            case SNK_BLU:
                std::cout << "Blue player joined the game!" << std::endl;
                break;
            case SNK_GRN:
                std::cout << "Green player joined the game!" << std::endl;
                break;
            case SNK_SPECTATE:
                std::cout << "Spectator joined the game!" << std::endl;
                break;
            }
        }
        numOfPlayers=player.numOfPlayers;
	}

	return player.startGame;
}
