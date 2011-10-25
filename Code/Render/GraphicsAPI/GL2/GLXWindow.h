#ifndef _GLX_WINDOW_DEF_H_
#define _GLX_WINDOW_DEF_H_
#include "../Window.h"

#include <stdio.h>
#include <stdlib.h>

#include <GL/glx.h>
#include <GL/gl.h>

#define X11_H <X11/X.h>
#define X11_KEYSYM <X11/keysym.h>

#include X11_H

class GWindowGLX : public RenderWindow
{
private:
    Postbox<Input_Event>* m_InputPostbox;

    Display* disp;

    int screen;

    Window win;

    GLXContext ctx;
    Colormap cmap;

    XVisualInfo* vi;

    XSetWindowAttributes attr;

    bool fs;

    bool doubleBuffered;

    int x, y;

    unsigned int width, height;
    unsigned int depth;

    GRenderer* m_boundRenderer;

public:

    GWindowGLX();
    ~GWindowGLX();

    void setInputPostbox(Postbox<Input_Event>* nPb)
    {
        m_InputPostbox = nPb;
    }

    bool create(const char *, int , int, GRenderer*);
    void destroy();

    void update();
    void finalFrame();

    int getWidth()
    {
        return width;
    }
    int getHeight()
    {
        return height;
    }

    void setTitle(const char* newTitle);
    void setDimensions(unsigned int width, unsigned int height);
    void setFullscreen(bool value);

};

#endif //_GLX_WINDOW_DEF_H
