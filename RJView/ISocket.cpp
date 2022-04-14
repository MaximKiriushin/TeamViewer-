#include "ISocket.h"

ISocket::ISocket()
{
	ZeroMemory(&this->socInfo, sizeof(this->socInfo));
	ZeroMemory(&this->clientInfo, sizeof(this->clientInfo));
}

void ISocket::initSocket(int port)
{
	socInfo.sin_port = htons(port); // port that server will listen for
	socInfo.sin_family = AF_INET;// must be AF_INET
	socInfo.sin_addr.s_addr = INADDR_ANY;// when there are few ip's for the machine. We will use always "INADDR_ANY"
	this->setType(SERVER);
}

void ISocket::initSocket(int port, std::string ipAdress)
{
	socInfo.sin_port = htons(port); // port that client will listen to
	socInfo.sin_family = AF_INET;// must be AF_INET
	inet_pton(AF_INET, ipAdress.c_str(), &this->socInfo.sin_addr);// the IP of the server
	this->setType(CLIENT);
}

bool ISocket::getType()
{
	return this->socType;
}

void ISocket::setType(bool type)
{
	this->socType = type;
}
