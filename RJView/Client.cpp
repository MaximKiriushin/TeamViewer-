#include "Client.h"


Client::Client(string ip, int port)
	:wsa(), tcpSoc(), udpSoc()
{
	this->_ipAdress = ip;
	this->_port = port;
}

Client::~Client() {}

void Client::ConnecToTcpServer()
{
	this->tcpSoc.initSocket(this->_port, this->_ipAdress);
	this->tcpSoc.setupClient();
	rsaHandShakeClient();
}

void Client::ConnecToUdpServer()
{
	this->udpSoc.initSocket(this->_port, this->_ipAdress);
	this->udpSoc.setupClient();
}

void Client::mainTcpClient()
{
	startKeyboard();
	startMouse();
}

void Client::rsaHandShakeClient()
{
	string serverPublicKey;
	string clientPublicKeyString;
	char* serverPublicKeyChar = nullptr;

	//Sending client's rsa public key
	this->_clientKeys = createKeys();
	clientPublicKeyString = rsaKeySendMsg(this->_clientKeys.first);
	this->tcpSoc.sendMessage(clientPublicKeyString);

	//Getting server public key enc
	serverPublicKey = this->tcpSoc.receiveMessage();
	serverPublicKeyChar = stringToChar(serverPublicKey);
	serverPublicKey = decrypt(this->_clientKeys.second, serverPublicKeyChar, (int)serverPublicKey.length());
	this->_serverKeys.first = rsaKeyReciveMsg(serverPublicKey.substr(MSG_CODE + MSG_SIZE));

	mainTcpClient();
}

void Client::startKeyboard()
{
	std::thread sendKeyBoardInput(&Keyboard::sendKeyboardKeys, &k, &this->tcpSoc, this->_serverKeys);
	sendKeyBoardInput.detach();
}

void Client::startMouse()
{
	std::thread sendMouseCord(&Mouse::sendMouseCord, &m, &this->tcpSoc, this->_serverKeys);
	std::thread sendLClick(&Mouse::checkIfLClickPreesed, &m, &this->tcpSoc, this->_serverKeys);
	std::thread sendRClick(&Mouse::checkIfRClickPreesed, &m, &this->tcpSoc, this->_serverKeys);
	sendMouseCord.detach();
	sendLClick.detach();
	sendRClick.detach();
}

void Client::closeTcpClient()
{
	this->tcpSoc.closeSocket();
}

void Client::closeUdpClient()
{
	this->udpSoc.closeSocket();
}
