#include "GUIWindows.h"
/* Null, because instance will be initialized on demand. */
GUIWindows* GUIWindows::instance = nullptr;

GUIWindows* GUIWindows::getInstance()
{
    if (instance == 0)
    {
        instance = new GUIWindows();
    }

    return instance;
}

void GUIWindows::setMainWindow(HWND hWnd)
{
    this->mainWindow = hWnd;
}

HWND GUIWindows::getMainWindow()
{
    return this->mainWindow;
}

void GUIWindows::setStreamWindow(HWND hWnd)
{
    this->streamWindow = hWnd;
}

HWND GUIWindows::getStreamWindow()
{
    return this->streamWindow;
}

void GUIWindows::setSendButton(HWND hWnd)
{
    this->sendButton = hWnd;
}

HWND GUIWindows::getSendButton()
{
    return this->sendButton;
}

void GUIWindows::setRecieveButton(HWND hWnd)
{
    this->recieveButton = hWnd;
}

HWND GUIWindows::getRecieveButton()
{
    return this->recieveButton;
}

void GUIWindows::setInfoDialog(HWND hWnd)
{
    this->infoDialog = hWnd;
}

HWND GUIWindows::getInfoDialog()
{
    return this->infoDialog;
}

GUIWindows::GUIWindows()
{
    mainWindow = NULL;
    streamWindow = NULL;
    sendButton = NULL;
    recieveButton = NULL;
}

GUIWindows::~GUIWindows()
{
    if (this->instance)
    {
        delete this->instance;
    }
}
