#include "UDPSocket.h"

UDPSocket::UDPSocket()
{
	this->soc = socket(AF_INET, SOCK_DGRAM, 0);
}

UDPSocket::~UDPSocket()
{
	this->closeSocket();
}

void UDPSocket::setupServer()
{
	if (bind(this->soc, (sockaddr*)&this->socInfo, sizeof(this->socInfo)) == SOCKET_ERROR)
	{
		std::cout << "Can't bind socket! " << WSAGetLastError() << std::endl;
		return;
	}
}

void UDPSocket::setupClient()
{

}

void UDPSocket::sendMessage(std::string msg)
{
	this->sendMessage(msg.c_str(), (int)(msg.length()));
}

void UDPSocket::sendMessage(const char* msg, int length)
{
	if (this->getType() == SERVER)
	{
		sendto(this->soc, msg, length, 0, (sockaddr*)&this->clientInfo, sizeof(this->clientInfo));
	}
	else
	{
		sendto(this->soc, msg, length, 0, (sockaddr*)&this->socInfo, sizeof(this->socInfo));
	}
}

std::string UDPSocket::receiveMessage(int msgSize)
{
	std::string msg = "";
	char* buff = nullptr;
	buff = this->receiveMessageBuff(msgSize);
	msg = std::string(buff);
	delete[] buff;
	return msg;
}

char* UDPSocket::receiveMessageBuff(int msgSize)
{
	char* buff = new char[msgSize+1];
	int clientLength = sizeof(this->clientInfo); // The size of the client information

	ZeroMemory(&this->clientInfo, clientLength);
	ZeroMemory(buff, msgSize+1);

	int bytesIn = recvfrom(this->soc, buff, msgSize, 0, (sockaddr*)&this->clientInfo, &clientLength);
	if (bytesIn == SOCKET_ERROR)
	{
		std::cout << "Error receiving from client " << WSAGetLastError() << std::endl;
	}
	return buff;
}

msgInfo UDPSocket::receiveMessageInfo(int chunckSize)
{
	SOCKET recvSoc;
	msgInfo info;
	int clientLength = sizeof(this->clientInfo);
	std::string msg = "";

	msg = peek((MSG_CODE_SIZE * chunckSize) + (MSG_SIZE * chunckSize));
	info.msgCodeStr = msg.substr(0, MSG_CODE_SIZE * chunckSize);
	info.msgLengthStr = msg.substr(MSG_CODE_SIZE * chunckSize, MSG_SIZE * chunckSize);
	return info;
}

void UDPSocket::closeSocket()
{
	closesocket(this->soc);
}

std::string UDPSocket::receiveMessage()
{
	return receiveMessage(SIZE);
}

std::string UDPSocket::peek(int peekLength)
{
	std::string msg = "";
	char* buff = new char[peekLength + 1];
	int clientLength = sizeof(this->clientInfo); // The size of the client information

	ZeroMemory(&this->clientInfo, clientLength);
	ZeroMemory(buff, peekLength + 1);

	int bytesIn = recvfrom(this->soc, buff, peekLength, MSG_PEEK, (sockaddr*)&this->clientInfo, &clientLength);
	if (bytesIn == SOCKET_ERROR)
	{
		//std::cout << "Peek Error receiving from client " << WSAGetLastError() << std::endl;
	}

	msg = std::string(buff);
	delete[] buff;
	return msg;
}
