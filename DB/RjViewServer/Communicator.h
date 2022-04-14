#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <bitset>
#include <iostream>
#include <thread>
#include <map>
#include <iostream>
#include <exception>
#include <fstream>
#include "WSAInitializer.h"
#include "Deserializer.h"
#include <WS2tcpip.h>

#define REGISTER_USER_SUCSSES 0x78
#define REGISTER_USER_FAILED 0x79

#define GETTING_USER 0x6F

using namespace std;
class Communicator
{
private:
	SOCKET m_serverSocket;

	void bindAndListen();
	void handleNewClient(SOCKET clientSocket);
	string buildingMessage(string msg, int code);
	string zeroPaddingStr(string msg);
public:
	void startHandleRequest();
};

