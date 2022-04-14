#pragma once

#include "TCPSocket.h"
#include "UDPSocket.h"
#include "RSAEncryption.h"

#include "Keyboard.h"
#include "Mouse.h"

#include <iostream>
#include <string>
#include <Windows.h>

#define PROGRAM_PORT 8826

//keyboard
#define SEND_KEY 0xBD

//Mouse
#define MOUSE_CORD_CODE 0xBA
#define LEFT_PRESSED 0xBB
#define RIGHT_PRESSED 0xBC

using namespace std;

class Server
{
public:
	//c'tor & d'tor
	Server();
	~Server();

	//Start servers by need
	void startTcpServer();
	void startUdpServer();

	//main
	void mainTcpServer();

	//Rsa functions
	void rsaHandShakeServer();

	//Closing servers by need
	void closeTcpServer();
	void closeUdpServer();

	//Static instance

private:
	WSAInitializer wsa;
	TCPSocket tcpSoc;
	UDPSocket udpSoc;

	Keyboard k;
	Mouse m;

	RsaKeys _serverKeys;
	RsaKeys _clientKeys;
};

