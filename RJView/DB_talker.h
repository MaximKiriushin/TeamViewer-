#pragma once

#include "TCPSocket.h"
#include "RSAEncryption.h"

#include <cstring>
#include <iostream>
#include <string>
#include<cstdlib>
#include<ctime>
#include <Windows.h>
#include <Vector>

#define DB_PORT 7726
#define DB_IP "127.0.0.1"

#define CREATE_USER 0x64
#define GET_USER 0x6E

using namespace std;

class DB_talker
{
public:
	DB_talker();
	~DB_talker();

	//Talking to db
	string ceatingNewUserInDB();
	vector<string> checkIfUserExist(string password);

	//DB commands
	string getPasswordOfUser();
	string getIpAdrrOfUser();
	string getPortOfUser();

	//Getters&Setters
	string getHostname();
	string createPassword();
	string getIp();



private:

	WSAInitializer wsa;
	TCPSocket tcpSoc;

	string passwordOfUSer;
	string ipAdrrOfUser;
	string portOfUser;
};
