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

#define SEND_KEY 0xBD

using namespace std;
class Keyboard
{
public:
	void sendKeyboardKeys(TCPSocket* tcpSoc, RsaKeys server);
	void updateAndReciveKeyboardKeys(string data);

	string zeroPaddingStr(string key);
private:
	
	std::mutex _socketSenMutex;
};
