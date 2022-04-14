#pragma once
#include <string>;
#include <iostream>;
#include "SqliteDataBase.h"
using namespace std;

class IDatabase
{
public:
	IDatabase();
	bool addNewUser(string hostName, string password, string ipAdress, string port);
	vector<std::string> getIpAndPortInfo(string password);
private:
	SqliteDataBase dbAccess;
};

