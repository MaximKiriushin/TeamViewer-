#pragma once


#include "TCPSocket.h"
#include "UDPSocket.h"
#include "GUIWindows.h"
#include "GuiFunctions.h"
#include <windows.h>

#define COMMUNICATION 0x3B5
#define DB 0x3B6

#define CLIENT 0xDA
#define SERVER 0xDB
using namespace std;
class UserData
{
public:
	UserData();
	~UserData();

	//getters&setters:
	string getPasswordFromUser();
	void setPasswordFromUser(string passwordFromUserDB);

	string getPasswordOfUser();
	void setPasswordOfUser(string passwordFromUserFromDB);

	string getIpAdrrOfUser();
	void setIpAdrrOfUser(string ipAdrrOfUserDB);

	int getPortOfUser();
	void setPortOfUser(string portOfUserDB);

	static UserData* getInstance();
	void startServer();
	void startClient(std::string ip, int port, int mode, int whoAmI);
	bool isOpenService();

private:

	string passwordFromUser;
	string passwordOfUser;
	string ipAderOfUser;
	string portOfUser;

	static UserData* instance;
	TCPSocket tcpSoc;
	UDPSocket udpSoc;
	bool isOpenServer;
	bool isOpenClient;
};

