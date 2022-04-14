#pragma once

#include "ISocket.h"
#include <mutex>
#define TCP_SEND_FLAG 0x0
#define TCP_RECV_FLAG 0x0

class TCPSocket : public ISocket
{
public:
	TCPSocket();
	~TCPSocket();
	void setupServer();
	void setupClient();
	void sendMessage(std::string msg);
	void sendMessage(const char* msg, int length);
	std::string receiveMessage();
	std::string receiveMessage(int msgSize);
	msgInfo receiveMessageInfo();
	msgInfo receiveMessageInfo(int chunckSize);
	void closeSocket();
	SOCKET clientSocket;
private:
	std::mutex sendMutex;
};