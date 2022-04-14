#include "DB_talker.h"



DB_talker::DB_talker()
	:tcpSoc()
{
	tcpSoc.initSocket(DB_PORT, DB_IP);
	tcpSoc.setupClient();
	this->passwordOfUSer = "";
}

DB_talker::~DB_talker() {}

string DB_talker::ceatingNewUserInDB()
{
	string recvMsg;
	string justCreatedPass = createPassword();
	string msg = to_string(CREATE_USER) + "#" + getHostname() + "," + justCreatedPass + "," + getIp() + "," + "8826";

	this->tcpSoc.sendMessage(msg);
	recvMsg = this->tcpSoc.receiveMessage();
	
	std::string recvMsgStr(recvMsg);
	if (recvMsgStr == "True") {
		return justCreatedPass;
	}
	this->tcpSoc.closeSocket();
	return "ERROR";
}

vector<string> DB_talker::checkIfUserExist(string password)
{
	vector<string> userIpAndPort;
	string strRecvMsg;
	string msg = to_string(GET_USER) + "#" + password;

	this->tcpSoc.sendMessage(msg);
	strRecvMsg = this->tcpSoc.receiveMessage();

	string userIpAder = strRecvMsg.substr(0, strRecvMsg.find(","));
	strRecvMsg = strRecvMsg.substr(strRecvMsg.find(",") + 1);
	string usersPort = strRecvMsg;
	if(userIpAder != "" && usersPort != ""){
		userIpAndPort.push_back(userIpAder);
		userIpAndPort.push_back(usersPort);
	}
	this->tcpSoc.closeSocket();
	return userIpAndPort;
}


string DB_talker::getPasswordOfUser()
{
	return this->passwordOfUSer;
}

string DB_talker::getIpAdrrOfUser()
{
	return this->ipAdrrOfUser;
}

string DB_talker::getPortOfUser()
{
	return this->portOfUser;
}

string DB_talker::getHostname()
{
	char chostname[100];
	gethostname(chostname, 100);
	string hostname(chostname);
	return hostname;
}

string DB_talker::createPassword()
{
	const char alphanum[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	string password;
	srand(time(0));
	for (int i = 0; i < 3; i++)
		this->passwordOfUSer += alphanum[rand() % 62];
	return this->passwordOfUSer;
}

string DB_talker::getIp()
{
	FILE* fp, * outputfile;
	char var[60];
	/*
	fp = _popen("curl http://myexternalip.com/raw", "r");
	fgets(var, sizeof(var), fp);
	_pclose(fp);
	*/
	
	fp = _popen("ipconfig | findstr IPv4", "r");
	fgets(var, sizeof(var), fp);
	_pclose(fp);
	string localIp(var);
	localIp = localIp.substr(localIp.find(":") + 2);
	localIp = localIp.substr(0, localIp.length() - 1);
	
	return localIp;

}