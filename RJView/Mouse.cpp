#include "Mouse.h"

std::mutex socketSenMutex;

void Mouse::sendMouseCord(TCPSocket* tcpSoc, RsaKeys server)
{
    POINT pos;
    LPPOINT lPos = &pos;
    string msg, enc;

    while(true){
        GetCursorPos(lPos);
        msg = to_string(lPos->x) + "#" + to_string(lPos->y);
        enc = encrypt(server.first, stringToChar(msg), msg.length());
        Sleep(100);
        socketSenMutex.lock();
        tcpSoc->sendMessage(buildingMessage(enc, MOUSE_CORD_CODE));
        socketSenMutex.unlock();
    }
}

void Mouse::recvMouseCord(string cords)
{
    char msg[100] = { 0 };
    string strMsg;
    int cordX, cordY;
 
    cordX = stoi(cords.substr(0, cords.find("#")));
    cordY = stoi(cords.substr(cords.find("#") + 1));
    SetCursorPos(cordX, cordY);
}

void Mouse::checkIfLClickPreesed(TCPSocket* tcpSoc, RsaKeys server)
{
    string lPressed = "lPressed";
    string enc;

    while (true) {
        if ((GetKeyState(VK_LBUTTON) & 0x80) != 0)
        {
            enc = encrypt(server.first, stringToChar(lPressed), lPressed.length());
            socketSenMutex.lock();
            tcpSoc->sendMessage(buildingMessage(enc, LEFT_PRESSED));
            socketSenMutex.unlock();
        }
        this_thread::sleep_for(chrono::milliseconds(75));
    }

}

void Mouse::pressOnLButton()
{
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

void Mouse::checkIfRClickPreesed(TCPSocket* tcpSoc, RsaKeys server)
{
    string rPressed = "rPressed";
    string enc;

    while (true) {
        if ((GetKeyState(VK_RBUTTON) & 0x80) != 0)
        {
            enc = encrypt(server.first, stringToChar(rPressed), rPressed.length());
            socketSenMutex.lock();
            tcpSoc->sendMessage(buildingMessage(enc, RIGHT_PRESSED));
            socketSenMutex.unlock();
        }
        this_thread::sleep_for(chrono::milliseconds(75));
    }
}

void Mouse::pressOnRButton()
{
    mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
    mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
}


string Mouse::zeroPaddingStr(string cordX, string cordY)
{
    std::string paddingForX(size_t(6) - cordX.size(), '0');
    std::string paddingForY(size_t(6) - cordY.size(), '0');
    return paddingForX + cordX + paddingForY + cordY;
}
