#include "TCPSocket.h"

TCPSocket::TCPSocket()
{
	this->soc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	this->clientSocket = INVALID_SOCKET;
}

TCPSocket::~TCPSocket()
{
	this->closeSocket();
}

void TCPSocket::setupServer()
{
	if (::bind(this->soc, (struct sockaddr*)&this->socInfo, sizeof(this->socInfo)) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - bind");

	// Start listening for incoming requests of clients
	if (listen(this->soc, SOMAXCONN) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - listen");

	this->clientSocket = ::accept(this->soc, NULL, NULL);
	if (this->clientSocket == INVALID_SOCKET)
		throw std::exception(__FUNCTION__);
}



void TCPSocket::setupClient()
{
	int status = ::connect(this->soc, (struct sockaddr*)&this->socInfo, sizeof(this->socInfo));
	if (status == INVALID_SOCKET)
		throw std::exception("Cant connect to server");
}

void TCPSocket::sendMessage(std::string msg)
{
	sendMessage(msg.c_str(), (int)(msg.length()));
}

void TCPSocket::sendMessage(const char* msg, int length)
{
	SOCKET sendSoc;
	sendMutex.lock();
	if (this->getType() == SERVER)
	{
		sendSoc = this->clientSocket;
	}
	else
	{
		sendSoc = this->soc;
	}
	send(sendSoc, msg, length, TCP_SEND_FLAG);
	sendMutex.unlock();
}

std::string TCPSocket::receiveMessage()
{
	SOCKET recvSoc;
	std::string fullMsg;
	if (this->getType() == SERVER)
	{
		recvSoc = this->clientSocket;
	}
	else
	{
		recvSoc = this->soc;
	}
	std::string msg = "";
	int dataSize = 0;
	char* msgCode = new char[MSG_CODE_SIZE + STRING_IDENTIFIER];
	char* msgSize = new char[MSG_SIZE + STRING_IDENTIFIER];

	ZeroMemory(msgCode, MSG_CODE_SIZE + STRING_IDENTIFIER);
	ZeroMemory(msgSize, MSG_SIZE + STRING_IDENTIFIER);

	recv(recvSoc, msgCode, MSG_CODE_SIZE, TCP_RECV_FLAG);
	recv(recvSoc, msgSize, MSG_SIZE, TCP_RECV_FLAG);

	dataSize = atoi(msgSize);
	
	msg = this->receiveMessage(dataSize);
	fullMsg = std::string(msgCode) + std::string(msgSize) + msg;
	delete[] msgCode;
	delete[] msgSize;
	
	return msg;
}

std::string TCPSocket::receiveMessage(int msgSize)
{
	SOCKET recvSoc;
	std::string msg = "";
	char* msgData = new char[msgSize + STRING_IDENTIFIER];
	ZeroMemory(msgData, msgSize + STRING_IDENTIFIER);

	if (this->getType() == SERVER)
	{
		recvSoc = this->clientSocket;
	}
	else
	{
		recvSoc = this->soc;
	}

	recv(recvSoc, msgData, msgSize, TCP_RECV_FLAG);
	msg = std::string(msgData);
	delete[] msgData;
	return msg;
}

msgInfo TCPSocket::receiveMessageInfo()
{
	return this->receiveMessageInfo(1);
}

msgInfo TCPSocket::receiveMessageInfo(int chunckSize)
{
	SOCKET recvSoc;
	msgInfo info;
	if (this->getType() == SERVER)
	{
		recvSoc = this->clientSocket;
	}
	else
	{
		recvSoc = this->soc;
	}
	std::string msg = "";
	char* msgCode = new char[MSG_CODE_SIZE * chunckSize + STRING_IDENTIFIER];
	char* msgSize = new char[MSG_SIZE * chunckSize + STRING_IDENTIFIER];

	ZeroMemory(msgCode, MSG_CODE_SIZE * chunckSize + STRING_IDENTIFIER);
	ZeroMemory(msgSize, MSG_SIZE * chunckSize + STRING_IDENTIFIER);

	recv(recvSoc, msgCode, MSG_CODE_SIZE * chunckSize, TCP_RECV_FLAG);
	recv(recvSoc, msgSize, MSG_SIZE * chunckSize, TCP_RECV_FLAG);

	info.msgCodeStr = std::string(msgCode);
	info.msgLengthStr = std::string(msgSize);

	delete[] msgCode;
	delete[] msgSize;

	return info;
}

void TCPSocket::closeSocket()
{
	closesocket(this->clientSocket);
	closesocket(this->soc);
}
