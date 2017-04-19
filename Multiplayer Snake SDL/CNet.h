#ifndef CNET_H_INCLUDED
#define CNET_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>
#include <cstring>
#include "SDL_net.h"

typedef char charbuf [256];

class CNet{
  public:
    static bool Init(); //Initialize SDL_net
    static void Quit(); //Exit SDL_net
};

class CNetMessage {
  protected:
    charbuf buffer;  //message stored in buffer, limited to 256 bytes
    enum bufstates { //message states
        EMPTY,
        READING,
        WRITING,
        FULL
    };
    bufstates state; //the message state
    void reset();    // reset message: fulfill it with zeroes and change its state to EMPTY
  public:
    CNetMessage();   //constructor
    //Virtual member function that indicates how many bytes may have to be loaded into the instance. By default, it works with strings that have a maximum
    //of 255 characters.
    virtual int NumToLoad();

    //Virtual member function that indicates how many bytes may have to be downloaded from the instance.
    virtual int NumToUnLoad();

    void LoadBytes(charbuf& inputbuffer, int n); //Load a char set into the message buffer
    void UnLoadBytes(charbuf& destbuffer); //Unload a char set from the buffer
    void finish(); //set the state object to full. No more data to be loaded
};

class CIpAddress {
  private:
	IPaddress m_Ip; //the IPaddress structure
  public:
	CIpAddress(); //constructor
	CIpAddress (Uint16 port); //create and associate a port to the instance
	CIpAddress (std::string host, Uint16 port); //create and associate a port and a host to the instance
	void SetIp (IPaddress sdl_ip); //set a CIpAddress object from an existing SDL IPaddress
	bool Ok() const; //True if the object have a port and a host associated to it
	IPaddress GetIpAddress() const; //return a SDL_net IPaddress structure
	Uint32 GetHost() const; //return the host
	Uint16 GetPort() const; //return the port
};

class CTcpSocket {
  protected:
    TCPsocket m_Socket; //the TCP socket structure
    SDLNet_SocketSet set; //a set of sockets. Used here only to check existing packets
  public:
    CTcpSocket();
    virtual ~CTcpSocket();
    virtual void SetSocket (TCPsocket  the_sdl_socket); //set a CTcpSocket object from a existing SDL socket
    bool Ok() const; //indicate if theres is a TCPsocket associated to the instance
    bool Ready() const; //True if there are bytes ready to be read
    virtual void OnReady(); //pure virtual
};

class CClientSocket;

class CHostSocket : public CTcpSocket {
  public:
    CHostSocket (CIpAddress& the_ip_address); //create and open a new socket, with an existing CIpAddress object

    CHostSocket (Uint16 port); //create and open a new socket with the desired port
    bool Accept (CClientSocket&); //set a client CTcpSocket object after listening to the port
    virtual void OnReady(); //pure virtual
};

class CClientSocket : public CTcpSocket {
  private:
    CIpAddress m_RemoteIp; //the CIpAddress object corresponding to the remote host
  public:
    CClientSocket(); //constructor
    CClientSocket (std::string host, Uint16 port); //Create the object and connect to a host, in a given port
    bool Connect (CIpAddress& remoteip); //make a connection to communicate with a remote host
    bool Connect (CHostSocket& the_listener_socket); //make a connection to communicate with a client
    void SetSocket (TCPsocket  the_sdl_socket); //set a CTcpSocket object from an existing SDL_net socket
    CIpAddress getIpAddress () const; //return a CIpAddress object associated to the remote host
    virtual void OnReady(); //pure virtual
    bool Receive(CNetMessage& rData); //receive data and load it into a CNetMessage object
    bool Send (CNetMessage& sData); //send data from a CNetMessage object
};

#endif // CNET_H_INCLUDED
