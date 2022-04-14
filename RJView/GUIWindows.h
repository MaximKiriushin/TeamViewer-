#pragma once

#include "TCPSocket.h"
#include "UDPSocket.h"

#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <mutex>
#include "resource.h"
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

class GUIWindows
{
private:
    static GUIWindows* instance;
    HWND mainWindow;
    HWND streamWindow;
    HWND sendButton;
    HWND recieveButton;
    HWND infoDialog;
    GUIWindows();
    ~GUIWindows();
public:
    /* Static access method. */
    static GUIWindows* getInstance();

    void setMainWindow(HWND hWnd);
    HWND getMainWindow();

    void setStreamWindow(HWND hWnd);
    HWND getStreamWindow();

    void setSendButton(HWND hWnd);
    HWND getSendButton();

    void setRecieveButton(HWND hWnd);
    HWND getRecieveButton();

    void setInfoDialog(HWND hWnd);
    HWND getInfoDialog();
};
