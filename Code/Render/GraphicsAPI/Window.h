#ifndef _RENDER_WINDOW_DEF_H_
#define _RENDER_WINDOW_DEF_H_
#include <Input/InputEvent.h>
#include <Input/Postbox.h>

class GRenderer;

//Interface
//Allows to easly swap between window creation classes for different platforms (Or even graphics API's) (as long as they extend GWindow)
class RenderWindow
{
public:
    virtual ~RenderWindow() {}

    virtual int getWidth()
    {
        return 0;
    }

    virtual int getHeight()
    {
        return 0;
    }

    virtual bool create(const char * Title, int Width, int Height, GRenderer*)
    {
        return false;    //Let the creating class select the best BPP etc. It doesn't really matter.
    }

    virtual void setTitle(const char* newTitle) {}
    virtual void setDimensions(unsigned int width, unsigned int height) {}
    virtual void setFullscreen(bool value) {}

    virtual void setInputPostbox(Postbox<Input_Event>* nPb) {  }

    virtual void update() { }
    virtual void finalFrame() { }
};

#endif //_RENDER_WINDOW_DEF_H_
