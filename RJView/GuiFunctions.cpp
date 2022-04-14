#include "GuiFunctions.h"


VOID WriteString(HDC hdc, const wchar_t* string)
{
    Graphics    graphics(hdc);
    SolidBrush  brush(Color(255, 80, 187, 230));
    FontFamily  fontFamily(L"Times New Roman");
    GUIWindows* guiWin = GUIWindows::getInstance();
    RECT rcClient;
    GetClientRect(guiWin->getMainWindow(), &rcClient);
    Font        font(&fontFamily, 24, FontStyleBold, UnitPixel);
    PointF      pointF(10.0f, 20.0f);

    graphics.DrawString(string, -1, &font, pointF, &brush);
}
LRESULT CALLBACK  mainWindowProc(HWND hWnd, UINT message,
    WPARAM wParam, LPARAM lParam)
{
    //HDC          hdc;
    //PAINTSTRUCT  ps;
    GUIWindows* guiWin = GUIWindows::getInstance();
    RECT rcClient;
    switch (message)
    {
    case WM_PAINT:
        GetClientRect(hWnd, &rcClient);
        MoveWindow(guiWin->getRecieveButton(),
            (rcClient.right / 2) - (rcClient.right / 10),
            (rcClient.bottom / 2) - (rcClient.bottom / 10),
            (rcClient.right / 10),
            (rcClient.bottom / 10),
            false);
        MoveWindow(guiWin->getSendButton(),
            (rcClient.right / 2) - (rcClient.right / 10),
            (rcClient.bottom / 2) - (2 * (rcClient.bottom / 10)),
            (rcClient.right / 10),
            (rcClient.bottom / 10),
            false);
        ShowWindow(guiWin->getRecieveButton(), SW_SHOW);
        ShowWindow(guiWin->getSendButton(), SW_SHOW);
        ShowWindow(guiWin->getMainWindow(), SW_SHOW);
        return 0;
    case WM_SYSCOMMAND:
        if (wParam == SC_CLOSE)
        {
            PostQuitMessage(0);
            return(TRUE);
        }
        else if (wParam == SC_MAXIMIZE)
        {
            return 0;
        }
        else if (wParam == SC_MINIMIZE)
        {
            return 0;
        }
        else
        {
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}
LRESULT dlgShowInfoProc(HWND   hDlg, UINT   Msg, WPARAM wParam, LPARAM lParam)
{
    GUIWindows* guiWin = GUIWindows::getInstance();
    switch (Msg)
    {
    case WM_INITDIALOG:
    {   
        DB_talker db;
        std::wstring str_turned_to_wstr = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(db.ceatingNewUserInDB());
        LPCWSTR wide_string = str_turned_to_wstr.c_str();
        SetDlgItemText(hDlg, IDC_INFOLABEL, L"Your password");
        SetDlgItemText(hDlg, IDC_INFOTEXT, wide_string);
        break;
    }
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case IDCANCEL:
        {
            guiWin->setInfoDialog(NULL);
            EnableWindow(guiWin->getRecieveButton(), true);
            DestroyWindow(hDlg);
            break;
        }
        default:
            break;
        }
    }
    default:
        break;
    }
    return 0;
}
LRESULT dlgConnectProc(HWND   hDlg, UINT   Msg, WPARAM wParam, LPARAM lParam)
{
    GUIWindows* guiWin = GUIWindows::getInstance();
    switch (Msg)
    {
    case WM_INITDIALOG:
    {
        SetDlgItemText(hDlg, IDC_INPUTLABEL, L"Enter Password");
        break;
    }
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_CONNECT:
        {
            DB_talker db;
            TCHAR password[16];
            WORD passwordLen = (WORD)SendDlgItemMessage(hDlg,
                IDC_INPUT,
                EM_LINELENGTH,
                (WPARAM)0,
                (LPARAM)0);

            string strPass = tCharToString(password, passwordLen, hDlg);

            vector<string> userIpAndPort = db.checkIfUserExist(strPass);

            if (passwordLen >= 16)
            {
                SetDlgItemText(hDlg, IDC_ERRMSG, L"Too many characters.");
                ShowWindow(GetDlgItem(hDlg, IDC_ERRMSG), SW_SHOW);
            }
            else if (passwordLen == 0)
            {
                SetDlgItemText(hDlg, IDC_ERRMSG, L"No characters entered.");
                ShowWindow(GetDlgItem(hDlg, IDC_ERRMSG), SW_SHOW);
            }
            else if (userIpAndPort.empty()) {
                SetDlgItemText(hDlg, IDC_ERRMSG, L"User doesn't exist");
                ShowWindow(GetDlgItem(hDlg, IDC_ERRMSG), SW_SHOW);
            }
            else
            {
                ShowWindow(GetDlgItem(hDlg, IDC_ERRMSG), SW_HIDE);

                password[passwordLen] = 0;
                
                startClient(userIpAndPort[0], stoi(userIpAndPort[1]));


                createStreamWindow(GetModuleHandle(NULL), 0, GetCommandLine(), SW_SHOWMAXIMIZED);
                PostMessageA(hDlg, WM_COMMAND, IDCANCEL, NULL);
            }
            break;
        }
        case IDCANCEL:
            EndDialog(hDlg, wParam);
            break;
        }
    default:
        break;
    }
    return 0;
}
LRESULT CALLBACK  sendButtonProc(HWND hWnd, UINT message,
    WPARAM wParam, LPARAM lParam)
{
    GUIWindows* guiWin = GUIWindows::getInstance();
    PAINTSTRUCT  ps;
    HDC hdc;
    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        WriteString(hdc, L"Send");
        EndPaint(hWnd, &ps);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_LBUTTONDOWN:
    {
        guiWin->setInfoDialog(CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_SHOWINFO), guiWin->getMainWindow(), dlgShowInfoProc));
        ShowWindow(guiWin->getInfoDialog(), SW_SHOW);

        startServer();

        return 0;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}
LRESULT CALLBACK  receiveButtonProc(HWND hWnd, UINT message,
    WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT  ps;
    HDC hdc;
    GUIWindows* guiWin = GUIWindows::getInstance();

    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        WriteString(hdc, L"Recieve");
        EndPaint(hWnd, &ps);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_LBUTTONDOWN:
    {
        DialogBox(GetModuleHandle(NULL),
            MAKEINTRESOURCE(IDD_CONNECT),
            guiWin->getMainWindow(),
            dlgConnectProc);
        //startClient();
        return 0;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}
int WINAPI createMainWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    HWND                hWnd;
    MSG                 msg;
    WNDCLASS            wndClass;
    WNDCLASS            btnClass;
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;
    GUIWindows* guiWin = GUIWindows::getInstance();
    RECT rcClient;

    // Initialize GDI+.
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = mainWindowProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = hInstance;
    wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = TEXT("Main Window");
    RegisterClass(&wndClass);

    btnClass.style = CS_HREDRAW | CS_VREDRAW;
    btnClass.lpfnWndProc = sendButtonProc;
    btnClass.cbClsExtra = 0;
    btnClass.cbWndExtra = 0;
    btnClass.hInstance = hInstance;
    btnClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    btnClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    btnClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    btnClass.lpszMenuName = NULL;
    btnClass.lpszClassName = TEXT("Send");
    RegisterClass(&btnClass);

    btnClass.style = CS_HREDRAW | CS_VREDRAW;
    btnClass.lpfnWndProc = receiveButtonProc;
    btnClass.cbClsExtra = 0;
    btnClass.cbWndExtra = 0;
    btnClass.hInstance = hInstance;
    btnClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    btnClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    btnClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    btnClass.lpszMenuName = NULL;
    btnClass.lpszClassName = TEXT("Recieve");

    RegisterClass(&btnClass);

    hWnd = CreateWindow(
        TEXT("Main Window"),   // window class name
        TEXT("RJView"),  // window caption
        WS_OVERLAPPEDWINDOW,      // window style
        CW_USEDEFAULT,            // initial x position
        CW_USEDEFAULT,            // initial y position
        CW_USEDEFAULT,            // initial x size
        CW_USEDEFAULT,            // initial y size
        NULL,                     // parent window handle
        NULL,                     // window menu handle
        hInstance,                // program instance handle
        NULL);                    // creation parameters
    guiWin->setMainWindow(hWnd);
    GetClientRect(hWnd, &rcClient);
    guiWin->setRecieveButton(CreateWindow(
        TEXT("Recieve"),  // Predefined class; Unicode assumed 
        L"recieve",      // Button text 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
        (rcClient.right / 2) - (rcClient.right / 10),         // x position 
        (rcClient.bottom / 2) - (rcClient.bottom / 10),         // y position 
        (rcClient.right / 10),        // Button width
        (rcClient.bottom / 10),        // Button height
        hWnd,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL));      // Pointer not needed.
    guiWin->setSendButton(CreateWindow(
        TEXT("Send"),  // Predefined class; Unicode assumed 
        L"send",      // Button text 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
        (rcClient.right / 2) - (rcClient.right / 10),// x position 
        (rcClient.bottom / 2) - (2 * (rcClient.bottom / 10)),// y position 
        (rcClient.right / 10), // Button width
        (rcClient.bottom / 10),// Button height
        hWnd,     // Parent window
        NULL,       // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL));      // Pointer not needed.
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    GdiplusShutdown(gdiplusToken);
    return msg.wParam;
}


VOID UpdateImage(HDC hdc)
{
    Graphics graphics(hdc);
    Image image(L"test.bmp");
    graphics.DrawImage(&image, 0, 0);
}
LRESULT CALLBACK  streamWindowProc(HWND hWnd, UINT message,
    WPARAM wParam, LPARAM lParam)
{
    HDC          hdc;
    PAINTSTRUCT  ps;
    GUIWindows* guiWin = GUIWindows::getInstance();
    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        UpdateImage(hdc);
        EndPaint(hWnd, &ps);
        return 0;
    case WM_DESTROY:
        EnableWindow(guiWin->getRecieveButton(), true);
        EnableWindow(guiWin->getSendButton(), true);
        EnableWindow(guiWin->getMainWindow(), true);
        
        PostQuitMessage(0);
        return 0;
    case WM_COMMAND:
        HIWORD(wParam);
        return 0;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}

int WINAPI createStreamWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    HWND                hWnd;
    MSG                 msg;
    WNDCLASS            wndClass;
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;
    GUIWindows* guiWin = GUIWindows::getInstance();
    // Initialize GDI+.
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = streamWindowProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = hInstance;
    wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = TEXT("StreamWindow");

    RegisterClass(&wndClass);

    hWnd = CreateWindow(
        TEXT("StreamWindow"),   // window class name
        TEXT("Watch Stream"),  // window caption
        WS_OVERLAPPEDWINDOW,      // window style
        CW_USEDEFAULT,            // initial x position
        CW_USEDEFAULT,            // initial y position
        CW_USEDEFAULT,            // initial x size
        CW_USEDEFAULT,            // initial y size
        NULL,                     // parent window handle
        NULL,                     // window menu handle
        hInstance,                // program instance handle
        NULL);                    // creation parameters
    guiWin->setStreamWindow(hWnd);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    //startClient();
    while (GetMessage(&msg, NULL, 0, 0))
    {
        RedrawWindow(guiWin->getStreamWindow(), NULL, NULL, RDW_INVALIDATE | RDW_NOCHILDREN);
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    GdiplusShutdown(gdiplusToken);
    return msg.wParam;
}

string tCharToString(TCHAR* string, WORD passwordLen, HWND hDlg)
{
    // Get the characters. 
    SendDlgItemMessage(hDlg,
        IDC_INPUT,
        EM_GETLINE,
        (WPARAM)0, 
        (LPARAM)string);

    // Null-terminate the string. 
    string[passwordLen] = 0;

    std::basic_string<TCHAR> passwordInWstring = string;
    std::string tcharInStr(passwordInWstring.begin(), passwordInWstring.end()); //Getting password from user in string\

    return tcharInStr;
}

void startGui()
{
    createMainWindow(GetModuleHandle(NULL), 0, GetCommandLine(), SW_SHOW);
}

void startServer()
{
    communicationInfo connectInfo;
    connectInfo.port = 9926;
    connectInfo.type = SERVER;

    Server* s = new Server();
    Communicator* communicator = new Communicator(connectInfo);

    GUIWindows* guiWin = GUIWindows::getInstance();
    EnableWindow(guiWin->getRecieveButton(), false);

    std::thread serverThread(&Server::startTcpServer, s);
    serverThread.detach();

    communicator->startCommunication();
}

void startClient(string ip, int port)
{
    communicationInfo connectInfo;
    connectInfo.ip = ip;
    connectInfo.port = 9926;
    connectInfo.type = CLIENT;

    Client* c = new Client(ip, port);
    Communicator* communicator = new Communicator(connectInfo);

    GUIWindows* guiWin = GUIWindows::getInstance();
    EnableWindow(guiWin->getRecieveButton(), false);
    EnableWindow(guiWin->getSendButton(), false);

    std::thread clientThread(&Client::ConnecToTcpServer, c);
    clientThread.detach();

    communicator->startCommunication();
}
