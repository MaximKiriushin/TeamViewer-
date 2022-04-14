#pragma once

#include "GUIWindows.h"
#include "DB_talker.h"
#include "Communicator.h"


#include "Server.h"
#include "Client.h"

//Convert string into wstring:
#include <codecvt>
#include <locale>
//

using namespace std;

LRESULT CALLBACK  mainWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK  receiveButtonProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK  sendButtonProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT dlgShowInfoProc(HWND   hDlg, UINT   Msg, WPARAM wParam, LPARAM lParam);
LRESULT dlgConnectProc(HWND   hDlg, UINT   Msg, WPARAM wParam, LPARAM lParam);
int WINAPI createMainWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow);
VOID UpdateImage(HDC hdc);
LRESULT CALLBACK  streamWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
int WINAPI createStreamWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow);
string tCharToString(TCHAR* string, WORD passwordLen, HWND hDlg);
void startGui();
void startServer();
void startClient(string ip, int port);
