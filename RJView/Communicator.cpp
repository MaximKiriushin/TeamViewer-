#include "Communicator.h"

void partAssemble(char* buff, int id, char** sendbuff, int size)
{
	char* idBuff = nullptr;
	*sendbuff = new char[size + CHUNK_ID_LENGTH];
	zeroPadding(id, CHUNK_ID_LENGTH, &idBuff);
	copyCharStr(idBuff, *sendbuff, CHUNK_ID_LENGTH);
	copyCharStrOffset(buff, *sendbuff, size, CHUNK_ID_LENGTH);
	delete[] idBuff;
}

int partDisassemble(char* buff)
{
	char* idBuff = new char[CHUNK_ID_LENGTH + STRING_IDENTIFIER];
	int id = 0;
	if (buff) {
		ZeroMemory(idBuff, CHUNK_ID_LENGTH + STRING_IDENTIFIER);
		copyCharStr(buff, idBuff, CHUNK_ID_LENGTH);
		id = atoi(idBuff);
		delete[] idBuff;
	}
	return id;
}

Communicator::Communicator()
{
	this->type = CLIENT;
	this->hasImageInfo = false;
	this->udpSoc.initSocket(8826, "127.0.0.1");
	//this->tcpSoc.initSocket(8827, "127.0.0.1");
}

Communicator::Communicator(communicationInfo info)
{
	this->hasImageInfo = false;
	if (info.type == CLIENT)
	{
		this->type = CLIENT;
		this->udpSoc.initSocket(info.port, info.ip);
		//this->tcpSoc.initSocket(info.port+1, info.ip);
	}
	else
	{
		this->udpSoc.initSocket(info.port);
		//this->tcpSoc.initSocket(info.port+1);
		this->type = SERVER;
	}
}

Communicator::~Communicator()
{
}

void Communicator::startCommunication()
{
	//this->tcpCommunication = std::thread(&Communicator::startTcpCommunication, this);
	//this->tcpCommunication.detach();
	this->udpCommunication = std::thread(&Communicator::startUdpCommunication, this);
	this->udpCommunication.detach();
}

void Communicator::closeCommunication()
{
	//this->closeTcpCommunication();
	this->closeUdpCommunication();
}

void Communicator::startTcpCommunication()
{
	if (this->type == SERVER)
	{
		this->tcpSoc.setupServer();
	}
	else
	{
		this->tcpSoc.setupClient();
	}
}

void Communicator::closeTcpCommunication()
{
	this->tcpSoc.closeSocket();
}

void Communicator::startUdpCommunication()
{
	msgInfo inf;
	if (this->type == SERVER)
	{
		this->udpSoc.setupServer();
		this->udpSoc.receiveMessage(9);//establish connection
		while (true)
		{
			imageHandle2(0);
			//Sleep(100);
		}
	}
	else
	{
		this->udpSoc.setupClient();
		this->udpSoc.sendMessage("establish");
		while (true)
		{
			imageHandle2(0);
		}
	}
	/*
	std::cout << "-a-\n";
	msgInfo info;
	if (this->type == SERVER)
	{
		this->udpSoc.setupServer();
		this->udpSoc.receiveMessage(9);
		while (getKeysState() != bothKeys) {}
		std::cout << "1\n";
		while (true)
		{
			this->imageHandle(info);
			this->udpSoc.receiveMessage(3);
		}
	}
	else
	{
		this->udpSoc.setupClient();
		this->udpSoc.sendMessage("establish");
		while (getKeysState() != bothKeys) {}
		while (1)
		{
			info = udpMessageInfo();
			this->imageInfoHandle(info);
			this->imageHandle(info);
			this->udpSoc.sendMessage("im"); //update that got imageInfo
		}
	***/
}

void Communicator::closeUdpCommunication()
{
	this->udpSoc.closeSocket();
}

char* Communicator::encapsMsg(int encType, int msgType, int dataLen, const char* data)
{
	return nullptr;
}


void Communicator::imageHandle(msgInfo info)
{
	int chunckSize = 20000;
	std::string msg;
	char* imgBuff = nullptr;
	if (this->type == CLIENT)
	{
		while (!hasImageInfo){}
		imgBuff = this->udpSoc.receiveMessageBuff(chunckSize);
		saveImage(imgBuff, this->imgInfo);
		delete[] imgBuff;
		imgBuff = nullptr;
		this->hasImageInfo = false;
	} 
	else
	{
		this->imgHandler.captureImage();
		this->imgHandler.saveImage(100); //this parameter should change according to internet quality
		this->imgInfo.name = this->imgHandler.getImageName();
		this->imgInfo.imageLength = this->imgHandler.getImageSize();
		getFile(this->imgInfo, &imgBuff);
		int chunckCount = this->imgInfo.imageLength / chunckSize;
		if (this->imgInfo.imageLength - (chunckSize * chunckCount))
		{
			chunckCount += 1;
		}
		for(int i = 0; i < chunckCount; i++)
		{
			this->udpSoc.sendMessage(imgBuff + (i * chunckSize), chunckSize);
		}
		delete[] imgBuff;
		imgBuff = nullptr;
	}
}

void Communicator::imageHandle2(int partSize)
{
	int chunckSize = CHUNK_PART;
	char* imgBuff = nullptr;
	std::string msg;
	if (this->type == CLIENT)
	{
		imgInfo.imageLength = chunckSize;
		imgBuff = this->udpSoc.receiveMessageBuff(chunckSize + CHUNK_ID_LENGTH);
		saveImage(imgBuff + CHUNK_ID_LENGTH, this->imgInfo, partDisassemble(imgBuff));
		GUIWindows* guiWin = GUIWindows::getInstance();
		RedrawWindow(guiWin->getStreamWindow(), NULL, NULL, RDW_INVALIDATE | RDW_NOCHILDREN);
		Sleep(20);
		delete[] imgBuff;
		imgBuff = nullptr;
	}
	else
	{
		int i = 0;
		this->imgHandler.captureImage();
		this->imgHandler.saveImage(100); //this parameter should change according to internet quality
		this->imgInfo.name = this->imgHandler.getImageName();
		this->imgInfo.imageLength = this->imgHandler.getImageSize();
		getFile(this->imgInfo, &imgBuff);

		int chunckCount = this->imgInfo.imageLength / CHUNK_PART;
		char* msgBuff = nullptr;
		for (i = 0; i < chunckCount; i++)
		{
			partAssemble(imgBuff + (i * CHUNK_PART), i, &msgBuff, CHUNK_PART);
			this->udpSoc.sendMessage(msgBuff, chunckSize + CHUNK_ID_LENGTH);
			delete[] msgBuff;
			msgBuff = nullptr;
			Sleep(50);
		}
		if (this->imgInfo.imageLength - (CHUNK_PART * chunckCount))
		{
			partAssemble(imgBuff + (i * CHUNK_PART), i, &msgBuff, this->imgInfo.imageLength - (CHUNK_PART * chunckCount));
			this->udpSoc.sendMessage(msgBuff, this->imgInfo.imageLength - (CHUNK_PART * chunckCount) + CHUNK_ID_LENGTH);
			delete[] msgBuff;
			Sleep(50);
		}

		delete[] imgBuff;
	}
}