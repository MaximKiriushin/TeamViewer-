#pragma once
#include <bitset>
#include <iostream>
#include <thread>
#include <map>
#include <iostream>
#include <exception>
#include "IDatabase.h"

using namespace std;
class Deserializer 
{
public:
	bool resgiterUserIntoServer(string msg);
	vector<std::string> GettingPortAndIp(string msg);
private:
	IDatabase db = IDatabase();
};
