#include "Communicator.h"


#define TWO 2
#define THREE 3
#define FIVE 5
#define TEN 10
#define SIZE 100

using namespace std;

void Communicator::bindAndListen()
{
	// notice that we step out to the global namespace
	// for the resolution of the function accept

	// this accepts the client and create a specific socket from server to this client
	SOCKET client_socket = ::accept(m_serverSocket, NULL, NULL);
	if (client_socket == INVALID_SOCKET)
		throw std::exception(__FUNCTION__);

	std::cout << "Client accepted. Server and client can speak" << std::endl;

	// the function that handle the conversation with the client
	thread thr(&Communicator::handleNewClient, this, client_socket);
	thr.detach();
}

void Communicator::handleNewClient(SOCKET clientSocket)
{
	Deserializer dez = Deserializer();
	char msg[SIZE] = { 0 };
	bool status;
	int counter = 0;
	string ip, port;
	string outputMsg = "";
	int i = 0;
	while(true){
		try
		{
			recv(clientSocket, msg, SIZE, 0);
			std::string strMsg(msg);
			if (strMsg.substr(0, 3) == "100") {
				status = dez.resgiterUserIntoServer(strMsg);
				if (status == true) {
					outputMsg = buildingMessage("True", REGISTER_USER_SUCSSES);
					send(clientSocket, outputMsg.c_str(), outputMsg.size(), 0);
				}
				else {
					outputMsg = buildingMessage("False", REGISTER_USER_FAILED);
					send(clientSocket, outputMsg.c_str(), outputMsg.size(), 0);
				}
			}
			else if (strMsg.substr(0, 3) == "110") {
				vector<std::string> userInfo = dez.GettingPortAndIp(msg);
				
				for (auto i = userInfo.begin(); i != userInfo.end(); ++i){
					if (counter == 0) {
						ip = *i;
					}
					else if (counter == 1) {
						port = *i;
					}
					counter++;
				}
				outputMsg = buildingMessage(ip + ',' + port, GETTING_USER);
				send(clientSocket, outputMsg.c_str(), outputMsg.size(), 0);
			}
			for (i = 0; i < SIZE; i++)
			{
				msg[i] = 0;
			}
		}
		catch (const std::exception& e)
		{
			cout << e.what() << endl;
			closesocket(clientSocket);
		}
	}
}

string Communicator::buildingMessage(string msg, int code)
{
	return to_string(code) + zeroPaddingStr(to_string(msg.length())) + msg;
}

string Communicator::zeroPaddingStr(string msg)
{
	std::string padding(size_t(10) - msg.size(), '0');
	return padding + msg;

}

void Communicator::startHandleRequest()
{
	const int port = 7726;
	struct sockaddr_in sa = { 0 };

	sa.sin_port = htons(port); // port that server will listen for
	sa.sin_family = AF_INET;   // must be AF_INET
	sa.sin_addr.s_addr = INADDR_ANY;    // when there are few ip's for the machine. We will use always "INADDR_ANY"
	m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// again stepping out to the global namespace
	// Connects between the socket and the configuration (port and etc..)
	if (::bind(m_serverSocket, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - bind");

	// Start listening for incoming requests of clients
	if (listen(m_serverSocket, SOMAXCONN) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - listen");
	std::cout << "Listening on port " << port << std::endl;

	while (true)
	{
		// the main thread is only accepting clients 
		// and add then to the list of handlers
		std::cout << "Waiting for client connection request" << std::endl;
		bindAndListen();
	}
}
