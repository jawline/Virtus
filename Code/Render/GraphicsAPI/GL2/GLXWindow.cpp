#if defined(_BUILD_LINUX_)

#include "GLXWindow.h"
#include <Input/Keys.h>
#include "../../GRender.h"

static int attrList[] =
{

    GLX_RGBA,

    GLX_DOUBLEBUFFER,

    GLX_RED_SIZE, 4,

    GLX_GREEN_SIZE, 4,

    GLX_BLUE_SIZE, 4,

    GLX_DEPTH_SIZE, 16,

    None

};

GWindowGLX::GWindowGLX()
{
    disp = 0;
    vi = 0;
    m_boundRenderer = 0;
    printf("GLX window constructor called\n");
}

GWindowGLX::~GWindowGLX()
{
    destroy();
}

void GWindowGLX::setFullscreen(bool value)
{
    //TODO: Implement fullscreen video
    printf("Fullscreen not supported on Linux (Yet)\n");
}

void GWindowGLX::setDimensions(unsigned int nwidth, unsigned int nheight)
{

    //If there is no display exit out
    if (disp == 0) return;

    XResizeWindow(disp, win, nwidth, nheight);

    width = nwidth;
    height = nheight;

}

void GWindowGLX::setTitle(const char* Title)
{

    //If there is no display exit out
    if (disp == 0) return;

    XSetStandardProperties(disp, win, Title, Title, None, 0, 0, NULL);

}

void GWindowGLX::destroy()
{

    //If theres no display theres nothing to free
    if (disp == 0) return;

    printf("Destroying window\n");


    glXMakeCurrent(disp, 0, 0);
    glXDestroyContext(disp, ctx);


    XDestroyWindow(disp, win);
    XFreeColormap(disp, cmap);
    XCloseDisplay(disp);

    m_boundRenderer = 0;
    disp = 0;

    memset(&win, 0, sizeof(Window));
    memset(&ctx, 0, sizeof(GLXContext));
    memset(&cmap, 0, sizeof(Colormap));

    printf("Display closed all nulled\n");

}

bool GWindowGLX::create(const char * Title, int W, int H, GRenderer* Render)
{
    printf("GLX window initialized\n");

    m_InputPostbox = 0;
    int dummy = 0;

    m_boundRenderer = Render;

    printf("Creating GLX window\n");

    disp = XOpenDisplay(NULL);

    if (disp == NULL)
    {

        printf("Unable to open display\n");

        return false;

    }

    if(!glXQueryExtension(disp, &dummy, &dummy))
    {

        printf("GLX not supported\n");

        return false;
    }

    vi = glXChooseVisual(disp, DefaultScreen(disp), attrList);

    if (vi == NULL)
    {

        printf("Unable to choose double buffered RGB visual\n");
        return false;

    }

    ctx = glXCreateContext(disp, vi, /* no shared dlists */ None,
                           /* direct rendering if possible */ GL_TRUE);

    if (ctx == NULL)
    {

        printf("Unable to get a OpenGL context\n");

        return false;

    }

    /* create an X colormap since probably not using default visual */
    cmap = XCreateColormap(disp, RootWindow(disp, vi->screen), vi->visual, AllocNone);

    attr.colormap = cmap;
    attr.border_pixel = 0;
    attr.event_mask = KeyPressMask | KeyReleaseMask | PointerMotionMask | ExposureMask
                      | ButtonPressMask | ButtonReleaseMask | StructureNotifyMask | ResizeRequest;

    win = XCreateWindow(disp, RootWindow(disp, vi->screen), 0, 0,
                        W, H, 0, vi->depth, InputOutput, vi->visual,
                        CWBorderPixel | CWColormap | CWEventMask, &attr);

    XSetStandardProperties(disp, win, Title, Title, None,
                           0, 0, NULL);

    Atom wmDelete=XInternAtom(disp, "WM_DELETE_WINDOW", True);
    XSetWMProtocols(disp, win, &wmDelete, 1);

//Uncomment to make the cursor go away
    /*
        Pixmap bm_no;

        Cursor no_ptr;
        XColor black, dummyclr;
        static char bm_no_data[] = {0, 0, 0, 0, 0, 0, 0, 0};

        if (disp == 0) return false;

        cmap = DefaultColormap(disp, DefaultScreen(disp));
        XAllocNamedColor(disp, cmap, "black", &black, &dummyclr);
        bm_no = XCreateBitmapFromData(disp, win, bm_no_data, 8, 8);
        no_ptr = XCreatePixmapCursor(disp, bm_no, bm_no, &black, &black, 0, 0);

        XDefineCursor(disp, win, no_ptr);
        XFreeCursor(disp, no_ptr);

        if (bm_no != None)
        {
                XFreePixmap(disp, bm_no);
                XFreeColors(disp, cmap, &black.pixel, 1, 0);
        } */

    /*** (6) bind the rendering context to the window ***/

    glXMakeCurrent(disp, win, ctx);

    /*** (7) request the X window to be displayed on the screen ***/

    XMapWindow(disp, win);

    width = W;
    height = H;

    printf("Done\n");

    return true;
}

Input_Event keyEvent_ASCI(int itype, int ASCIVal)
{
    Input_Event Event;
    Event.Event_ID = itype;

    if (ASCIVal > 64 && ASCIVal < 91)
    {
        Event.Event_Flags = 0 | KEY_UPPERCASE;
    }
    else
    {
        Event.Event_Flags = 0;
    }

    switch (ASCIVal)
    {
    case ' ':
    {
        Event.Event_Data.push_back(KEY_SPACE);
        break;
    }
    case 'a':
    case 'A':
    {
        Event.Event_Data.push_back(KEY_A);
        break;
    }
    case 'b':
    case 'B':
    {
        Event.Event_Data.push_back(KEY_B);
        break;
    }
    case 'c':
    case 'C':
    {
        Event.Event_Data.push_back(KEY_C);
        break;
    }
    case 'd':
    case 'D':
    {
        Event.Event_Data.push_back(KEY_D);
        break;
    }
    case 'e':
    case 'E':
    {
        Event.Event_Data.push_back(KEY_E);
        break;
    }
    case 'f':
    case 'F':
    {
        Event.Event_Data.push_back(KEY_F);
        break;
    }
    case 'g':
    case 'G':
    {
        Event.Event_Data.push_back(KEY_G);
        break;
    }
    case 'h':
    case 'H':
    {
        Event.Event_Data.push_back(KEY_H);
        break;
    }
    case 'i':
    case 'I':
    {
        Event.Event_Data.push_back(KEY_I);
        break;
    }
    case 'j':
    case 'J':
    {
        Event.Event_Data.push_back(KEY_J);
        break;
    }
    case 'k':
    case 'K':
    {
        Event.Event_Data.push_back(KEY_K);
        break;
    }
    case 'l':
    case 'L':
    {
        Event.Event_Data.push_back(KEY_L);
        break;
    }
    case 'm':
    case 'M':
    {
        Event.Event_Data.push_back(KEY_M);
        break;
    }
    case 'n':
    case 'N':
    {
        Event.Event_Data.push_back(KEY_N);
        break;
    }
    case 'o':
    case 'O':
    {
        Event.Event_Data.push_back(KEY_O);
        break;
    }
    case 'p':
    case 'P':
    {
        Event.Event_Data.push_back(KEY_P);
        break;
    }
    case 'q':
    case 'Q':
    {
        Event.Event_Data.push_back(KEY_Q);
        break;
    }
    case 'r':
    case 'R':
    {
        Event.Event_Data.push_back(KEY_R);
        break;
    }
    case 's':
    case 'S':
    {
        Event.Event_Data.push_back(KEY_S);
        break;
    }
    case 't':
    case 'T':
    {
        Event.Event_Data.push_back(KEY_T);
        break;
    }
    case 'u':
    case 'U':
    {
        Event.Event_Data.push_back(KEY_U);
        break;
    }
    case 'v':
    case 'V':
    {
        Event.Event_Data.push_back(KEY_V);
        break;
    }
    case 'w':
    case 'W':
    {
        Event.Event_Data.push_back(KEY_W);
        break;
    }
    case 'x':
    case 'X':
    {
        Event.Event_Data.push_back(KEY_X);
        break;
    }
    case 'y':
    case 'Y':
    {
        Event.Event_Data.push_back(KEY_Y);
        break;
    }
    case 'z':
    case 'Z':
    {
        Event.Event_Data.push_back(KEY_Z);
        break;
    }

    //Numas
    case '1':
    {
        Event.Event_Data.push_back(KEY_1);
        break;
    }
    case '2':
    {
        Event.Event_Data.push_back(KEY_2);
        break;
    }
    case '3':
    {
        Event.Event_Data.push_back(KEY_3);
        break;
    }
    case '4':
    {
        Event.Event_Data.push_back(KEY_4);
        break;
    }
    case '5':
    {
        Event.Event_Data.push_back(KEY_5);
        break;
    }
    case '6':
    {
        Event.Event_Data.push_back(KEY_6);
        break;
    }
    case '7':
    {
        Event.Event_Data.push_back(KEY_7);
        break;
    }
    case '8':
    {
        Event.Event_Data.push_back(KEY_8);
        break;
    }
    case '9':
    {
        Event.Event_Data.push_back(KEY_9);
        break;
    }
    case '0':
    {
        Event.Event_Data.push_back(KEY_0);
        break;
    }

    case '\r':
    {
        Event.Event_Data.push_back(KEY_RETURN);
        break;
    }
    case '\b':
    {
        Event.Event_Data.push_back(KEY_BACKSPACE);
        break;
    }

    default:
    {
        Event.Event_ID = -1;
    }

    }

    return Event;
}

bool KeysDown[256];
bool sKeyDown[256];

void GWindowGLX::update()
{
    XEvent               event;
    char text[255];
    KeySym key;

    if (XPending(disp))
    {

        do
        {
            XNextEvent(disp, &event);

            switch (event.type)
            {

                case ClientMessage:
                {
                    printf("Window destroyed\n");

                    Input_Event newEvent;
                    newEvent.Event_ID = input_event_window;
                    newEvent.Event_Data.push_back(WINDOW_KILL_REQUEST);

                    GWindowGLX::m_InputPostbox->AddQueue(newEvent);

                    break;
                };

                case KeyPress:
                {
                    if (XLookupString(&event.xkey,text,255,&key,0) == 1)
                    {
                        Input_Event E = keyEvent_ASCI(input_event_keyreleased, text[0]);

                        if (E.Event_ID != -1)
                        {
                            sKeyDown[E.Event_Data[0]] = false;

                            if (KeysDown[E.Event_Data[0]] == false)
                            {

                                KeysDown[E.Event_Data[0]] = true;

                                Input_Event E = keyEvent_ASCI(input_event_keypressed, text[0]);

                                if (E.Event_ID != -1)
                                {
                                    if (m_InputPostbox)
                                        GWindowGLX::m_InputPostbox->AddQueue(E);
                                }

                            }
                        }
                    }

                    break;
                }

                case KeyRelease:
                {
                    if (XLookupString(&event.xkey,text,255,&key,0) == 1)
                    {
                        Input_Event E = keyEvent_ASCI(input_event_keyreleased, text[0]);

                        if (E.Event_ID != -1)
                            sKeyDown[E.Event_Data[0]] = true;
                    }

                    break;
                }

                case ButtonPress:
                {

                    std::vector<int> Data;
                    Data.push_back(MOUSE_DOWN);
                    Data.push_back(event.xbutton.x);
                    Data.push_back(event.xbutton.y);

                    Input_Event E;
                    E.Event_ID = input_event_button_one;
                    E.Event_Data = Data;
                    E.Event_Flags = 0;

                    if (m_InputPostbox)
                        GWindowGLX::m_InputPostbox->AddQueue(E);

                    break;
                }

                case ButtonRelease:
                {
                    std::vector<int> Data;
                    Data.push_back(MOUSE_UP);
                    Data.push_back(event.xbutton.x);
                    Data.push_back(event.xbutton.y);

                    Input_Event E;
                    E.Event_ID = input_event_button_one;
                    E.Event_Data = Data;
                    E.Event_Flags = 0;

                    if (m_InputPostbox)
                        GWindowGLX::m_InputPostbox->AddQueue(E);

                    break;
                }

                case ConfigureNotify:
                {
                    //resize(event.xconfigure.width, event.xconfigure.height);

                    /*

                    if (m_boundRenderer != 0)
                    {
                        m_boundRenderer->getGraphicsAPI()->haresizeViewport(0, 0, event.xconfigure.width, event.xconfigure.height);
                    }

                    */

                    width = event.xconfigure.width;
                    height = event.xconfigure.height;

                    break;
                }

                case MotionNotify:
                {
                    std::vector<int> Data;

                    Data.push_back(MOUSE_MOTION);
                    Data.push_back(event.xbutton.x);
                    Data.push_back(event.xbutton.y);

                    Input_Event E;
                    E.Event_ID = input_event_button_one;
                    E.Event_Data = Data;
                    E.Event_Flags = 0;

                    if (m_InputPostbox)
                        GWindowGLX::m_InputPostbox->AddQueue(E);

                    break;
                }
            }
        }
        while(XPending(disp));   /* loop to compress events */

    }

    for (unsigned int i = 0; i < 256; i++)
    {
        if (sKeyDown[i] == true)
        {
            Input_Event E;
            E.Event_ID = input_event_keyreleased;
            E.Event_Data.push_back(i);

            if (m_InputPostbox)
                GWindowGLX::m_InputPostbox->AddQueue(E);

            sKeyDown[i] = false;
            KeysDown[i] = false;
        }
    }

    return;
}

void GWindowGLX::finalFrame()
{
    glXSwapBuffers(disp, win);
}

#endif //_BUILD_LINUX_
