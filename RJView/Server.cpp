#include "Server.h"

Server::Server()
	:wsa(), tcpSoc(), udpSoc()
{
}

Server::~Server() {}

void Server::startTcpServer()
{
	this->tcpSoc.initSocket(PROGRAM_PORT);
	this->tcpSoc.setupServer();
	rsaHandShakeServer();
}

void Server::startUdpServer()
{
	this->udpSoc.initSocket(PROGRAM_PORT);
	this->udpSoc.setupServer();
}

void Server::mainTcpServer()
{
	while(true){
		msgInfo msgInf = this->tcpSoc.receiveMessageInfo();
		string msg = this->tcpSoc.receiveMessage(stoi(msgInf.msgLengthStr));
		switch (stoi(msgInf.msgCodeStr)) {
			case SEND_KEY: {
				std::thread reciveKeyBoardInput(&Keyboard::updateAndReciveKeyboardKeys, &k, decrypt(this->_serverKeys.second, stringToChar(msg), msg.length()));
				reciveKeyBoardInput.detach();
				break;
			}
			case MOUSE_CORD_CODE: {
				std::thread reciveMouseCord(&Mouse::recvMouseCord, &m, decrypt(this->_serverKeys.second, stringToChar(msg), msg.length()));
				reciveMouseCord.detach();
				break;
			}	
			case LEFT_PRESSED: {
				std::thread pressLClick(&Mouse::pressOnLButton, &m);
				pressLClick.detach();
				break;
			}
			case RIGHT_PRESSED: {
				std::thread pressRClick(&Mouse::pressOnRButton, &m);
				pressRClick.detach();
				break;
			}

		}
	}  
}

void Server::rsaHandShakeServer()
{
	string enc;
	string serverPublicKey;
	string clientPublicKey;
	char* serverPublicKeyChar = nullptr;

	//Getting client's rsa public key
	this->_serverKeys = createKeys();
	clientPublicKey = this->tcpSoc.receiveMessage();
	this->_clientKeys.first = rsaKeyReciveMsg(clientPublicKey);

	//Sending server's public key enc
	serverPublicKey = rsaKeySendMsg(this->_serverKeys.first);
	serverPublicKeyChar = stringToChar(serverPublicKey);
	enc = encrypt(this->_clientKeys.first, serverPublicKeyChar, serverPublicKey.length());
	this->tcpSoc.sendMessage(buildingMessage(enc, ENC_KEY));

	mainTcpServer();
}

void Server::closeTcpServer()
{
	this->tcpSoc.closeSocket();
}

void Server::closeUdpServer()
{
	this->udpSoc.closeSocket();
}

