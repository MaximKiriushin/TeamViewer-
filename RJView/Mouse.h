#pragma once

#include "TCPSocket.h"
#include "RSAEncryption.h"

#include <iostream>
#include <chrono> 
#include <Windows.h>
#include <chrono>
#include <thread>
#include <string>
#include <cstdlib>
#include <mutex>

#define MOUSE_CORD_CODE 0xBA
#define LEFT_PRESSED 0xBB
#define RIGHT_PRESSED 0xBC


using namespace std;
class Mouse
{
public:
	void sendMouseCord(TCPSocket* tcpSoc, RsaKeys server);
	void recvMouseCord(string cords);
	void checkIfLClickPreesed(TCPSocket* tcpSoc, RsaKeys server);
	void pressOnLButton();
	void checkIfRClickPreesed(TCPSocket* tcpSoc, RsaKeys server);
	void pressOnRButton();

	string zeroPaddingStr(string cordX, string cordY);
};

