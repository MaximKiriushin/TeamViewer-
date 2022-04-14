#pragma once
#include "sqlite3.h"
#include <stdio.h>
#include "io.h"
#include <string>
#include <vector>
#include <iostream>
using namespace std;
class SqliteDataBase {
public:
	virtual bool open();
	virtual void close();
	virtual void clear();
	virtual bool createUser(string hostName, string password, string ipAdress, string port);
	virtual vector<std::string> getIpAndPort(string password);
private:
	sqlite3* db;

};
