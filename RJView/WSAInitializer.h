#pragma once
#pragma comment(lib, "ws2_32.lib")
#include <WS2tcpip.h>

class WSAInitializer
{
public:
	WSAInitializer();
	~WSAInitializer();
};
