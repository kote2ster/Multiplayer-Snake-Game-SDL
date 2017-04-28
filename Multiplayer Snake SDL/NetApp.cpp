#include <iostream>
#include "NetApp.h"
#include "snake.h"
#include "Bullet.h"

typedef struct Snake_TCP_Data {
    char playerColor;
    Snake_Dir nextSnakeDir;
    bool fireBullet;
} Snake_TCP_Data;

Snake_TCP_Data sendData;
Snake_TCP_Data receivedData;

// *** CNetHost ***
CNetHost::CNetHost()
{
    SDLNet_ResolveHost(&ip, NULL, 1234);

    server = SDLNet_TCP_Open(&ip);

    sendData.playerColor = playerColor;
    sendData.nextSnakeDir = DIR_UP;
    sendData.fireBullet = false;
}

CNetHost::~CNetHost()
{

}

void CNetHost::HandleConnection()
{
    //static const char* hello = "HELLO CLIENT";
    client = SDLNet_TCP_Accept(server);
    if(client) {
        if( SDLNet_TCP_Recv(client, &receivedData, sizeof(receivedData)) > 0 ) {
            snakes[receivedData.playerColor].nextDir = receivedData.nextSnakeDir;
            if(receivedData.fireBullet) {
                Bullet bullet;
                bullet.dir = snakes[receivedData.playerColor].currDir;
                bullet.pos = snakes[receivedData.playerColor].pos;
                bullet.screenPos = snakes[receivedData.playerColor].screenPos;
                bullet.levelPosX = snakes[receivedData.playerColor].levelPosX;
                bullet.levelPosY = snakes[receivedData.playerColor].levelPosY;
                bullet.markForDelete = false;
                bullets.push_back(bullet);
            }
        }
        sendData.nextSnakeDir = snakes[playerColor].nextDir;
        sendData.fireBullet = fireBullet;
        SDLNet_TCP_Send(client, &sendData, sizeof(sendData));
    }
}

void CNetHost::SendMsg()
{

}

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

    sendData.playerColor = playerColor;
    sendData.nextSnakeDir = DIR_UP;
    sendData.fireBullet = false;
}

CNetClient::~CNetClient()
{

}

void CNetClient::HandleConnection()
{
    client = SDLNet_TCP_Open(&ip);
    if(client) {
        sendData.nextSnakeDir = snakes[playerColor].nextDir;
        sendData.fireBullet = fireBullet;
        SDLNet_TCP_Send(client, &sendData, sizeof(sendData));
        //client = SDLNet_TCP_Open(&ip);
        if( SDLNet_TCP_Recv(client, &receivedData, sizeof(receivedData)) > 0 ){
            snakes[receivedData.playerColor].nextDir = receivedData.nextSnakeDir;
            if(receivedData.fireBullet) {
                Bullet bullet;
                bullet.dir = snakes[receivedData.playerColor].currDir;
                bullet.pos = snakes[receivedData.playerColor].pos;
                bullet.screenPos = snakes[receivedData.playerColor].screenPos;
                bullet.levelPosX = snakes[receivedData.playerColor].levelPosX;
                bullet.levelPosY = snakes[receivedData.playerColor].levelPosY;
                bullet.markForDelete = false;
                bullets.push_back(bullet);
            }
        }
    }
}

void CNetClient::SendMsg()
{
    //static const char* hello = "ASD";
    //client = SDLNet_TCP_Open(&ip);
    //SDLNet_TCP_Send(client, &sendData, sizeof(sendData));
}
