#if defined(_BUILD_WIN32_)

#include "W32_Window.h"
#include "InputEvent.h"
#include "keyids.h"
#include <Windowsx.h>
Postbox<Input_Event>* iPB = 0;
bool lMouseButtonDown = false;

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
    PIXELFORMATDESCRIPTOR pfd;

    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC(hwnd);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hDC, &pfd);

    SetPixelFormat(*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext(*hDC);

    wglMakeCurrent(*hDC, *hRC);
}

void DisableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}

bool KeysDown[KEYS_END];
bool sKeysDown[KEYS_END];

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE: {
            PostQuitMessage(0);
            exit(1);
            break;
        }

        case WM_DESTROY:
        {
            return 0;
        }

        case WM_LBUTTONDOWN:
        {

            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);

            Input_Event E;
            E.Event_ID = input_event_button_one;
            E.Event_Data.push_back(MOUSE_DOWN);
            E.Event_Data.push_back(x);
            E.Event_Data.push_back(y);

            iPB->AddQueue(E);
            lMouseButtonDown = true;
            return 0;
        }

        case WM_LBUTTONUP:
        {
            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);

            Input_Event E;
            E.Event_ID = input_event_button_one;
            E.Event_Data.push_back(MOUSE_UP);
            E.Event_Data.push_back(x);
            E.Event_Data.push_back(y);

            iPB->AddQueue(E);
            lMouseButtonDown = false;
            return 0;
        }

        case WM_MOUSEMOVE:
        {
            if (lMouseButtonDown) {
                int x = GET_X_LPARAM(lParam);
                int y = GET_Y_LPARAM(lParam);

                Input_Event E;
                E.Event_ID = input_event_button_one;
                E.Event_Data.push_back(MOUSE_MOTION);
                E.Event_Data.push_back(x);
                E.Event_Data.push_back(y);

                iPB->AddQueue(E);
            }
            return 0;
        }

        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_ESCAPE:
                    PostQuitMessage(0);
                break;

                case 0x30:
                case VK_NUMPAD1:
                {
                    if (iPB)
                    {
                        Input_Event E;
                        E.Event_ID = input_event_keypressed;
                        E.Event_Data.push_back(KEY_1);
                        KeysDown[E.Event_Data[0]] = true;
                        iPB->AddQueue(E);
                    }
                    break;
                }

                case 0x53:
                {
                    Input_Event E;
                    E.Event_ID = input_event_keypressed;
                    E.Event_Data.push_back(KEY_S);
                    sKeysDown[E.Event_Data[0]] = false;
                    if (KeysDown[E.Event_Data[0]] == false) {
                        KeysDown[E.Event_Data[0]] = true;
                        iPB->AddQueue(E);
                    }
                    break;
                }

                case 0x41:
                {
                    Input_Event E;
                    E.Event_ID = input_event_keypressed;
                    E.Event_Data.push_back(KEY_A);
                    sKeysDown[E.Event_Data[0]] = false;
                    if (KeysDown[E.Event_Data[0]] == false) {
                        KeysDown[E.Event_Data[0]] = true;
                        iPB->AddQueue(E);
                    }
                    break;
                }

                case 0x44:
                {
                    Input_Event E;
                    E.Event_ID = input_event_keypressed;
                    E.Event_Data.push_back(KEY_D);
                    sKeysDown[E.Event_Data[0]] = false;
                    if (KeysDown[E.Event_Data[0]] == false) {
                        KeysDown[E.Event_Data[0]] = true;
                        iPB->AddQueue(E);
                    }
                    break;
                }

                case 0x43:
                {
                    Input_Event E;
                    E.Event_ID = input_event_keypressed;
                    E.Event_Data.push_back(KEY_C);
                    sKeysDown[E.Event_Data[0]] = false;
                    if (KeysDown[E.Event_Data[0]] == false) {
                        KeysDown[E.Event_Data[0]] = true;
                        iPB->AddQueue(E);
                    }
                    break;
                }

                case 0x57:
                {
                    Input_Event E;
                    E.Event_ID = input_event_keypressed;
                    E.Event_Data.push_back(KEY_W);
                    sKeysDown[E.Event_Data[0]] = false;
                    if (KeysDown[E.Event_Data[0]] == false) {
                        KeysDown[E.Event_Data[0]] = true;
                        iPB->AddQueue(E);
                    }

                    break;
                }

            }
        }
        break;

        case WM_KEYUP:
        {
            switch (wParam) {
                case 0x53:
                {
                    Input_Event E;
                    E.Event_ID = input_event_keypressed;
                    E.Event_Data.push_back(KEY_S);
                    sKeysDown[E.Event_Data[0]] = true;
                    printf("Event Data 0 %i\n", E.Event_Data[0]);
                    break;
                }

                case 0x41:
                {
                    Input_Event E;
                    E.Event_ID = input_event_keypressed;
                    E.Event_Data.push_back(KEY_A);
                    sKeysDown[E.Event_Data[0]] = true;
                    break;
                }

                case 0x44:
                {
                    Input_Event E;
                    E.Event_ID = input_event_keypressed;
                    E.Event_Data.push_back(KEY_D);
                    sKeysDown[E.Event_Data[0]] = true;
                    break;
                }

                case 0x43:
                {
                    Input_Event E;
                    E.Event_ID = input_event_keypressed;
                    E.Event_Data.push_back(KEY_C);
                    sKeysDown[E.Event_Data[0]] = true;
                    break;
                }

                case 0x57:
                {
                    Input_Event E;
                    E.Event_ID = input_event_keypressed;
                    E.Event_Data.push_back(KEY_W);
                    sKeysDown[E.Event_Data[0]] = true;

                    break;
                }
            }
        }
        break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

bool W32Win::create(const char * Title, int Width, int Height, Postbox<Input_Event>* input_postbox)
{
    iPB = input_postbox;
    m_winWidth = Width;
    m_winHeight = Height;

        /* register window class */
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = 0;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "SimpleEngineC";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;


    if (!RegisterClassEx(&wcex))
        return 0;

    printf("Width Height %i %i\n", m_winWidth, m_winHeight);

    /* create main window */
    hwnd = CreateWindowEx(0,
                          "SimpleEngineC",
                          Title,
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          m_winWidth,
                          m_winHeight + 40,
                          NULL,
                          NULL,
                          0,
                          NULL);

    ShowWindow(hwnd, SW_SHOWDEFAULT);

    EnableOpenGL(hwnd, &hDC, &hRC);

    return true;
}

void W32Win::swapBuffers()
{
    SwapBuffers(hDC);
}

void W32Win::update()
{
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        /* handle or dispatch messages */
        if (msg.message == WM_QUIT)
        {
            //Send a quit message
            Input_Event Ev;
            Ev.Event_ID = input_event_keypressed;
            Ev.Event_Data.push_back(KEY_0);
            iPB->AddQueue(Ev);
        }
        else
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }


    for (unsigned int i = 0; i < 256; i++)
    {
        if (sKeysDown[i] == true) {
            printf("Key %i up\n", i);

            Input_Event E;
            E.Event_ID = input_event_keyreleased;
            E.Event_Data.push_back(i);
            iPB->AddQueue(E);

            sKeysDown[i] = false;
            KeysDown[i] = false;
        }
    }
}

#endif //_BUILD_WIN32_
