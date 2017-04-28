#ifndef NETAPP_H_INCLUDED
#define NETAPP_H_INCLUDED

#include "SDL_net.h"
#include "GlobalTypes.h"

class CNetInterface {
public:
    virtual void HandleConnection() = 0;
    virtual void SendMsg() = 0;
};

class CNetHost : public CNetInterface {
private:
    IPaddress ip;
    TCPsocket server;
    TCPsocket client;
public:
    CNetHost();
    ~CNetHost();

    void HandleConnection();
    void SendMsg();
};

class CNetClient : public CNetInterface {
private:
    IPaddress ip;
    TCPsocket client;
public:
    CNetClient();
    ~CNetClient();

    void HandleConnection();
    void SendMsg();
};

#endif // NETAPP_H_INCLUDED
