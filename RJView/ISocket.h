#pragma once

#pragma comment (lib, "ws2_32.lib")

#include "WSAInitializer.h"

#include <string>
#include <iostream>

#define SERVER false
#define CLIENT true

#define MSG_CODE_SIZE 0x3
#define MSG_SIZE 0x0A
#define STRING_IDENTIFIER 0x1

struct msgInfo {
	int msgCode = 0;
	int msgLength = 0;
	std::string msgCodeStr = "";
	std::string msgLengthStr = "";
	std::string msg = "";
}typedef msgInfo;

class ISocket
{
public:
	ISocket();
	void initSocket(int port);
	void initSocket(int port, std::string ipAdress);
	bool getType();
	void setType(bool type);
	virtual void setupServer() = 0;
	virtual void setupClient() = 0;
	virtual void sendMessage(std::string msg) = 0;
	virtual std::string receiveMessage(int msgSize) = 0;
	virtual msgInfo receiveMessageInfo(int chunckSize) = 0;
	virtual void closeSocket() = 0;
	SOCKET soc;
	sockaddr_in socInfo;
	sockaddr_in clientInfo;
private:
	bool socType;
};