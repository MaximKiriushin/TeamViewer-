#include "Deserializer.h"

bool Deserializer::resgiterUserIntoServer(string msg)
{
    string hostName = msg.substr(4, msg.find(",") - 4);
    msg = msg.substr(msg.find(",") + 1);
    string password = msg.substr(0, msg.find(","));
    msg = msg.substr(msg.find(",") + 1);
    string ipAdress = msg.substr(0, msg.find(","));
    msg = msg.substr(msg.find(",") + 1);
    string port = msg.substr(0, msg.find(","));
    return db.addNewUser(hostName, password, ipAdress, port);
}

vector<std::string> Deserializer::GettingPortAndIp(string msg)
{
    string password = msg.substr(4, msg.find(",") - 4);
    return db.getIpAndPortInfo(password);
}
