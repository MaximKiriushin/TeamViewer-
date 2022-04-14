#pragma once

#include "ISocket.h"

#define SIZE 1000
#define UDP_MAX_SIZE 60000

class UDPSocket : public ISocket
{
public:
	UDPSocket();
	~UDPSocket();
	void setupServer();
	void setupClient();
	void sendMessage(std::string msg);
	void sendMessage(const char* msg, int length);
	std::string receiveMessage(int msgSize);
	char* receiveMessageBuff(int msgSize);
	msgInfo receiveMessageInfo(int chunckSize);
	void closeSocket();
	std::string receiveMessage();
private:
	std::string peek(int peekLength);
};