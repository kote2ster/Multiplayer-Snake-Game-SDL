#ifndef NETAPP_H_INCLUDED
#define NETAPP_H_INCLUDED

#include "CNet.h"
#include "GlobalTypes.h"

class CNetMessageApp : public CNetMessage {
private:
    //Virtual function that indicates how many bytes may have to be loaded onto the object. Overrides the parent class function member to work with only a byte
    virtual int NumToLoad();

    //Virtual function that indicates how many bytes may have to be downloaded from the object. Overrides the parent class function member to work with only a byte
    virtual int NumToUnLoad();

public:

    //Function simpler than LoadBytes(), inherited from the parent class, as it only works with one byte
    void LoadByte(char);

    //Function simpler than UnLoadBytes(), inherited from the parent class, as it only works with one byte
    char UnLoadByte();
};

class CNetInterface {
public:
    virtual void HandleConnection() = 0;
    virtual void SendMsg() = 0;
};

class CNetHost : public CNetInterface {
private:
    CHostSocket* tcplistener;
    CClientSocket* tcpclient;
    CNetMessageApp msg;
    bool connected;
public:
    CNetHost();
    ~CNetHost();

    void HandleConnection();
    void SendMsg();
};

class CNetClient : public CNetInterface {
private:
    CClientSocket* tcpclient;
    CIpAddress* remoteip;
    CNetMessageApp msg;
    bool connected;
public:
    CNetClient();
    ~CNetClient();

    void HandleConnection();
    void SendMsg();
};

#endif // NETAPP_H_INCLUDED
