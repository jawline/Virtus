#if defined(_BUILD_WIN32_)

#ifndef _WINDOWS_32_WINDOW_H_
#define _WINDOWS_32_WINDOW_H_
#include "GWindow.h"
#include "defines.h"

#include GL_HEADER
#include WINDOWS_HEADER

/**
 * @brief Implementation of a game window (ARCHIVED - NEEDS UPDATING)
 */
class W32Win : public GWindow
{
private:
    int m_winHeight, m_winWidth;
    WNDCLASSEX wcex;
    HWND hwnd;
    HDC hDC;
    HGLRC hRC;
    MSG msg;

public:
    int getHeight()
    {
        return m_winHeight;
    }
    int getWidth()
    {
        return m_winWidth;
    }

    bool create(const char * Title, int Width, int Height, Postbox<Input_Event>* input_postbox);

    void update();
    void swapBuffers();
};

#endif //_WINDOWS_32_WINDOW_H_

#endif
