#pragma once

#include "TCPSocket.h"
#include "UDPSocket.h"
#include "RSAEncryption.h"

#include "Keyboard.h"
#include "Mouse.h"

#include <thread>
#include <iostream>
#include <string>
#include <Windows.h>

using namespace std;

class Client
{
public:
	Client(string ip, int port);
	~Client();

	//Start servers by need
	void ConnecToTcpServer();
	void ConnecToUdpServer();

	//main
	void mainTcpClient();

	//Rsa functions
	void rsaHandShakeClient();

	//Keyboard
	void startKeyboard();

	//Mouse
	void startMouse();

	//Closing servers by need
	void closeTcpClient();
	void closeUdpClient();


private:
	WSAInitializer wsa;
	TCPSocket tcpSoc;
	UDPSocket udpSoc;

	Keyboard k;
	Mouse m;

	string _ipAdress;
	int _port;
	RsaKeys _serverKeys;
	RsaKeys _clientKeys;
};



