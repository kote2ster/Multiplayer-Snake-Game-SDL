#ifndef NETAPP_H_INCLUDED
#define NETAPP_H_INCLUDED

#include "SDL_net.h"
#include "GlobalTypes.h"

#define MAX_SOCKETS 0x10
#define TIMEOUT 0

typedef struct {
	int in_use;
} Client;

typedef struct {
	char playerColor;
	char startGame;
	int numOfPlayers;
} Player;

typedef struct Snake_TCP_Data {
    char playerColor;
    Snake_Dir nextSnakeDir;
    Snake_Dir currSnakeDir;
    bool fireBullet;
    bool restartLevel;
    bool dead;
    Snake_Pos pos;
    char winnerColor;
} Snake_TCP_Data;

void InitData();
void SynchronizeSnakePos();
extern int numOfPlayers;
extern Uint32 previousTime;
extern Uint32 currentTime;

class CNetInterface {
public:
    virtual void HandleConnection() = 0;
    virtual void SendMsg(char snkIdx) = 0;
    virtual bool WaitForOtherPlayers() = 0;
    virtual void CheckForWinner() = 0;
};

class CNetHost : public CNetInterface {
private:
    int next_ind;
    TCPsocket server_socket;
    Client clients[MAX_SOCKETS];
    SDLNet_SocketSet socket_set;
    TCPsocket sockets[MAX_SOCKETS];

    IPaddress ip;

    int AcceptSocket(int index);
    void CloseSocket(int index);
public:
    CNetHost();
    ~CNetHost();

    bool WaitForOtherPlayers();
    void HandleConnection();
    void SendMsg(char snkIdx);
    void CheckForWinner();
};

class CNetClient : public CNetInterface {
private:
    TCPsocket socket;
    SDLNet_SocketSet socket_set;

    IPaddress ip;
    void CloseSocket(void);
    bool CheckSocket(void);
public:
    CNetClient();
    ~CNetClient();

    char GetColor();
    bool WaitForOtherPlayers();
    void HandleConnection();
    void SendMsg(char snkIdx);
    void CheckForWinner();
};

#endif // NETAPP_H_INCLUDED
