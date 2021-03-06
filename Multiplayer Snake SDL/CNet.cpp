#include "CNet.h"

// *** CNet ***
bool CNet::Init() {
    if (SDLNet_Init() < 0) {
        std::cerr << "SDLNet_Init: " << SDLNet_GetError() << std::endl;
        return false;
    }
    else
        return true;
}

void CNet::Quit() {
    SDLNet_Quit();
}


// *** CNetMessage ***
CNetMessage::CNetMessage() {
    reset();
}

void CNetMessage::reset() {
    for (int i =0; i < 256; i++)
        buffer [i] = 0;
    state = EMPTY;
}

void CNetMessage::finish() {
    if (state == READING)
        state = FULL;
}

int CNetMessage::NumToLoad() {
    if (state == EMPTY)
        return 255;
    else
        return 0;
}

int CNetMessage::NumToUnLoad() {
    if (state == FULL)
        return strlen(buffer) + 1;
    else
        return 0;
}

void CNetMessage::LoadBytes(charbuf& inputbuffer, int n) {
    for (int i = 0; i < n; i++)
        buffer[i] = inputbuffer[i];
    state = READING;
}

void CNetMessage::UnLoadBytes(charbuf& destbuffer) {
    for (int i=0; i < this->NumToUnLoad(); i++)
        destbuffer[i] = buffer[i];
    reset();
}

// *** CIPAddress ***
CIpAddress::CIpAddress (Uint16 port) {
	if (SDLNet_ResolveHost(&m_Ip, NULL, port) < 0){
		std::cerr << "SDLNet_ResolveHost: " << SDLNet_GetError() << std::endl;
		m_Ip.host = 0;
		m_Ip.port = 0;
	}
}

CIpAddress::CIpAddress (std::string host, Uint16 port) {
	if (SDLNet_ResolveHost(&m_Ip, host.c_str(), port) < 0) {
		std::cerr << "SDLNet_ResolveHost: " << SDLNet_GetError() << std::endl;
		m_Ip.host = 0;
		m_Ip.port = 0;
	}
}

CIpAddress::CIpAddress() {
	m_Ip.host = 0;
	m_Ip.port = 0;
}

bool CIpAddress::Ok() const {
	return !(m_Ip.port == 0);
}

void CIpAddress::SetIp (IPaddress sdl_ip) { //sets a CTcpSocket object from a existing SDL socket
	m_Ip = sdl_ip;
}

IPaddress CIpAddress::GetIpAddress() const {
	return m_Ip;
}

Uint32 CIpAddress::GetHost() const {
	return m_Ip.host;
}

Uint16 CIpAddress::GetPort() const {
	return m_Ip.port;
}


// *** CTcpSocket ***
CTcpSocket::CTcpSocket() {
    m_Socket = NULL;
    set = SDLNet_AllocSocketSet(1);
}

CTcpSocket::~CTcpSocket() {
    if (!(m_Socket == NULL)) {
          SDLNet_TCP_DelSocket(set,m_Socket);
        SDLNet_FreeSocketSet(set);
        SDLNet_TCP_Close (m_Socket);
    }
}

void CTcpSocket::SetSocket (TCPsocket the_sdl_socket) {
    if (!(m_Socket == NULL)) {
          SDLNet_TCP_DelSocket(set,m_Socket);
          SDLNet_TCP_Close (m_Socket);
      }
    m_Socket = the_sdl_socket;
    if(SDLNet_TCP_AddSocket(set,m_Socket)==-1) {
        std::cerr << "SDLNet_TCP_AddSocket: " << SDLNet_GetError() << std::endl;
        m_Socket = NULL;
    }
}

bool CTcpSocket::Ok() const {
    return !(m_Socket == NULL);
}

bool CTcpSocket::Ready() const {
    bool rd = false;
    int numready = SDLNet_CheckSockets(set, 0);
    if (numready == -1)
        std::cerr << "SDLNet_CheckSockets: " << SDLNet_GetError() << std:: endl;
    else
        if (numready)
            rd = SDLNet_SocketReady (m_Socket);
    return rd;
}

void CTcpSocket::OnReady() {

}


// *** CHostSocket ***
CHostSocket::CHostSocket (CIpAddress& the_ip_address) : CTcpSocket() {
    IPaddress iph = the_ip_address.GetIpAddress();
    if (!(m_Socket = SDLNet_TCP_Open(&iph))) {
            SDLNet_FreeSocketSet(set);
            std::cerr << "SDLNet_TCP_Open: " << SDLNet_GetError() << std::endl;
    }
}

CHostSocket::CHostSocket (Uint16 port) : CTcpSocket() {
    CIpAddress iplistener (port);
    if (!iplistener.Ok()) {
        m_Socket = NULL;
    }
    else {
        IPaddress iph = iplistener.GetIpAddress();
        if (!(m_Socket = SDLNet_TCP_Open(&iph))) {
            SDLNet_FreeSocketSet(set);
            std::cerr << "SDLNet_TCP_Open: " << SDLNet_GetError() << std::endl;
        }
    }
}

bool CHostSocket::Accept (CClientSocket& the_client_socket) {
    TCPsocket cs;
    if ((cs = SDLNet_TCP_Accept(m_Socket))) {
        the_client_socket.SetSocket(cs);
        return true;
    }
    else
        return false;
}

void CHostSocket::OnReady() {

}


// *** CClientSocket ***
CClientSocket::CClientSocket() : CTcpSocket() {

}

CClientSocket::CClientSocket (std::string host, Uint16 port) : CTcpSocket() {
    CIpAddress remoteip (host.c_str(), port);
    if (!remoteip.Ok()) {
        m_Socket = NULL;
    }
    else {
        Connect(remoteip);
    }
}

bool CClientSocket::Connect (CHostSocket& the_listener_socket) {
    return false;
}

bool CClientSocket::Connect (CIpAddress& remoteip) {
    TCPsocket cs;
    IPaddress ip = remoteip.GetIpAddress();
    if ((cs = SDLNet_TCP_Open(&ip)))
    {
        SetSocket(cs);
        return true;
    }
    else {
        std::cerr << "SDLNet_TCP_Open: " << SDLNet_GetError() << std::endl;
        return false;
    }
}

void CClientSocket::SetSocket (TCPsocket the_sdl_socket) {
    CTcpSocket::SetSocket (the_sdl_socket);
    IPaddress* ips;
    if ((ips = SDLNet_TCP_GetPeerAddress(m_Socket))) {
    /* Print the address, converting it onto the host format */
        m_RemoteIp.SetIp(*ips);
        Uint32 hbo = m_RemoteIp.GetHost();
        Uint16 pbo = m_RemoteIp.GetPort();
        //std::cout << "Client connected: " << SDLNet_Read32(&hbo) << ' '
            //<< SDLNet_Read16 (&pbo) << std::endl;
    }
    else
        std::cerr << "SDLNet_TCP_GetPeerAddress: " << SDLNet_GetError() << std::endl;
}

CIpAddress CClientSocket::getIpAddress () const {
    return m_RemoteIp;
}

bool CClientSocket::Receive(CNetMessage& rData) {
//Firstly, check if there is a socket
    if (m_Socket == NULL)
        return false;
    charbuf buf;

//Check if the instance can receive bytes, if it can, load the number of bytes specified by NumToLoad() virtual function
    while (rData.NumToLoad() > 0) {
        if (SDLNet_TCP_Recv(m_Socket, buf, rData.NumToLoad()) > 0) {
            rData.LoadBytes (buf, rData.NumToLoad());
        }
        else {
            return false;
        }
    }
    rData.finish();
    return true;
}

bool CClientSocket::Send (CNetMessage& sData) {
//check if there is a socket
    if (m_Socket == NULL)
        return false;
    charbuf buf;
    int len;

//Check if the instance can send bytes, if it can, unload the number of bytes specified by NumToLoad() virtual function
    while ((len = sData.NumToUnLoad()) > 0) {
        sData.UnLoadBytes (buf);
        if (SDLNet_TCP_Send(m_Socket, (void *)buf, len) < len) {
            std::cerr << "SDLNet_TCP_Send: " << SDLNet_GetError() << std::endl;
            return false;
        }
    }
    return true;
}

void CClientSocket::OnReady() {

}
