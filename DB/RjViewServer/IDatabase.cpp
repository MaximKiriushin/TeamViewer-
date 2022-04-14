#include "IDatabase.h"

IDatabase::IDatabase()
{
	this->dbAccess.open();
}

bool IDatabase::addNewUser(string hostName, string password, string ipAdress, string port)
{
	return this->dbAccess.createUser(hostName, password, ipAdress, port);
}

vector<std::string> IDatabase::getIpAndPortInfo(string password)
{
	return this->dbAccess.getIpAndPort(password);
}
