#include "UserData.h" 

UserData* UserData::instance = nullptr;

UserData::UserData()
    :tcpSoc(), udpSoc()
{
    this->isOpenServer = false;
    this->isOpenClient = false;
    this->passwordFromUser = "";
}
UserData::~UserData() {}

string UserData::getPasswordFromUser()
{
    return this->passwordFromUser;
}

void UserData::setPasswordFromUser(string passwordFromUserDB)
{
    this->passwordFromUser = passwordFromUserDB;
}

string UserData::getPasswordOfUser()
{
    return this->passwordOfUser;
}

void UserData::setPasswordOfUser(string passwordOFUserFromDB)
{
    this->passwordOfUser = passwordOFUserFromDB;
}

string UserData::getIpAdrrOfUser()
{
    return this->ipAderOfUser;
}

void UserData::setIpAdrrOfUser(string ipAdrrOfUserDB)
{
    this->ipAderOfUser = ipAdrrOfUserDB;
}

int UserData::getPortOfUser()
{
    return stoi(this->portOfUser);
}

void UserData::setPortOfUser(string portOfUserDB)
{
    this->portOfUser = portOfUserDB;
}

UserData* UserData::getInstance()
{
    if (instance == 0)
    {
        instance = new UserData();
    }

    return instance;
}

void UserData::startServer()
{
    if ((!this->isOpenServer) && (!this->isOpenClient))
    {
        
    }
}

void UserData::startClient(string ip, int port, int mode, int whoAmI)
{
    if ((!this->isOpenClient) && (!this->isOpenServer))
    {
       
    }
}
bool UserData::isOpenService()
{
    if ((!this->isOpenClient) && (!this->isOpenServer))
    {
        return false;
    }
    return true;
}
