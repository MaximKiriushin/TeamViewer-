#pragma once
#include "Communicator.h"
#include <exception>
#include <iostream>
#include <string>



class Server
{
private:
	Communicator m_communicator;

public:
	void run();
};

