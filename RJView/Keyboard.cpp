#include "Keyboard.h"



void Keyboard::sendKeyboardKeys(TCPSocket* tcpSoc, RsaKeys server)
{
	
	string enc;
	string stroke;

	while (true) {
		Sleep(10);
		for (int KEY = 8; KEY <= 190; KEY++)
		{
			if (GetAsyncKeyState(KEY) == -32767) {

				if (KEY == VK_CONTROL || KEY == VK_SHIFT) {
					stroke += to_string(KEY);
				}
				else {
					if (stroke != "" && (KEY != VK_LCONTROL || KEY != VK_LSHIFT)) {
						stroke += to_string(KEY);
						enc = encrypt(server.first, stringToChar(stroke), stroke.length());
						this->_socketSenMutex.lock();
						tcpSoc->sendMessage(buildingMessage(enc, SEND_KEY));
						this->_socketSenMutex.unlock();
						stroke.clear();
					}
					else {
						enc = encrypt(server.first, stringToChar(to_string(KEY)), to_string(KEY).length());
						this->_socketSenMutex.lock();
						tcpSoc->sendMessage(buildingMessage(enc, SEND_KEY));
						this->_socketSenMutex.unlock();

					}
				}
			}
		}
	}

}

string Keyboard::zeroPaddingStr(string key)
{
	std::string padding(size_t(5) - key.size(), '0');
	return padding + key;
}


void Keyboard::updateAndReciveKeyboardKeys(string data)
{
	int key = 0;

	if (data.length() > 3) {
		key = stoi(data.substr(2));
		INPUT inputs[4] = {};
		ZeroMemory(inputs, sizeof(inputs));

		inputs[0].type = INPUT_KEYBOARD;
		inputs[0].ki.wVk = VK_CONTROL;

		inputs[1].type = INPUT_KEYBOARD;
		inputs[1].ki.wVk = key;

		inputs[2].type = INPUT_KEYBOARD;
		inputs[2].ki.wVk = key;
		inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;

		inputs[3].type = INPUT_KEYBOARD;
		inputs[3].ki.wVk = VK_CONTROL;
		inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;
		UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
	}
	else {
		key = stoi(data);
		INPUT inputs[2] = {};
		ZeroMemory(inputs, sizeof(inputs));

		inputs[0].type = INPUT_KEYBOARD;
		inputs[0].ki.wVk = key;

		inputs[1].type = INPUT_KEYBOARD;
		inputs[1].ki.wVk = key;
		inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;
		UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
	}


}
