#include "NetApp.h"

// *** CNetMessageApp ***
int CNetMessageApp::NumToLoad(){
	if (state == EMPTY)
		return 1;
	else
		return 0;
}

int CNetMessageApp::NumToUnLoad() {
	if (state == FULL)
		return 1;
	else
		return 0;
}

void CNetMessageApp::LoadByte(char ID) {
	charbuf c;
	c[0] = ID;
	LoadBytes(c, 1);
	finish();
}

char CNetMessageApp::UnLoadByte() {
   charbuf c;
   UnLoadBytes (c);
   return c[0];
}

// *** CNetHost ***
CNetHost::CNetHost() {
    tcplistener = new CHostSocket(1234);
    if(!tcplistener->Ok())
        exit(EXIT_FAILURE);

    tcpclient = new CClientSocket();

    connected = false;
}

CNetHost::~CNetHost() {
    if(tcplistener) delete tcplistener;
    if(tcpclient) delete tcpclient;
}

void CNetHost::HandleConnection() {
    if(!connected) {
        // if not connected, listen to the port to detect if there is a client waiting there
        if(tcplistener->Accept(*tcpclient)) {
            connected = true;
        }
    } else {
        // if connected, checks the socket for messages ready to be read
        if(tcpclient->Ready()) {
            // if there is a message, try to receive it. In case of disconnection, the TCP protocol sends a message with no bytes
            if(tcpclient->Receive(msg)) {
                // if there is a valid message
                std::cout << msg.UnLoadByte();
            } else {
                connected = false;
            }
        }
    }
}

void CNetHost::SendMsg() {

}

// *** CNetClient ***
CNetClient::CNetClient() {
    tcpclient = new CClientSocket();

    std::cout << "Enter IP address of the Server: ";
    bool valid = false;
    do {
        std::string ip;
        std::cin >> ip;
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        CIpAddress ipAddr(ip, 1234);
        if(ipAddr.Ok()) {
            remoteip = new CIpAddress(ipAddr);
            valid = true;
        } else {
            std::cerr << "Could not connect to IP address!" << std::endl;
            std::cout << "Enter IP address of the Server: ";
        }
    } while(!valid);

    connected = false;
}

CNetClient::~CNetClient() {
    if(remoteip) delete remoteip;
    if(tcpclient) delete tcpclient;
}

void CNetClient::HandleConnection() {
    // if not connected, listen to the port to detect if there is a client waiting there
    if(!connected) {
        if(tcpclient->Connect(*remoteip)) {
            if(tcpclient->Ok())
                connected = true;
        }
    } else {
        // if connected, checks the socket for messages ready to be read
        if(tcpclient->Ready()) {
            // if there is a message, try to receive it. In case of disconnection, the TCP protocol sends a message with no bytes
            if(tcpclient->Receive(msg)) {
                // if there is a valid message
                std::cout << msg.UnLoadByte();
            }
        } else {
            connected = false;
        }
    }
}

void CNetClient::SendMsg() {
    msg.LoadByte('a');
    tcpclient->Send(msg);
}
