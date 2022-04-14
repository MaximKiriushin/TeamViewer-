#pragma once

#include "TCPSocket.h"
#include "UDPSocket.h"
#include <thread>
#include "RSAEncryption.h"
#include "ImageHandler.h"
#include "CommunicatorHelper.h"
#include "FileHelper.h"
#include "RSAEncryption.h"
#include "GuiFunctions.h"
#include "GuiWindows.h"

#include <iostream>
#include <string>
#include <Windows.h>

struct communicationInfo 
{
	bool type;
	int port;
	std::string ip;
}typedef communicationInfo;

int partDisassemble(char* buff);
void partAssemble(char* buff, int id, char** sendbuff, int size);

class Communicator
{
public:
	Communicator();
	Communicator(communicationInfo info);
	~Communicator();
	void startCommunication();
	void closeCommunication();
	void startTcpCommunication();
	void closeTcpCommunication();
	void startUdpCommunication();
	void closeUdpCommunication();
	char* encapsMsg(int encType, int msgType, int dataLen, const char* data);
private:
	void imageHandle(msgInfo info);
	void imageHandle2(int partSize);
	bool type;
	bool hasImageInfo;
	imageInfo imgInfo;
	WSAInitializer wsa;
	TCPSocket tcpSoc;
	UDPSocket udpSoc;
	ImageHandler imgHandler;
	std::thread tcpCommunication;
	std::thread udpCommunication;
};