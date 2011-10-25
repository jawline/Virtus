#ifndef _INTERFACE_ELEMENT_DEF_H_
#define _INTERFACE_ELEMENT_DEF_H_
#include <Input/InputEvent.h>

class InterfaceLayer;

class IElement
{
protected:
    float x, y, width, height;

public:

    IElement(float x, float y, float w, float h)
    {

        IElement::x = x;
        IElement::y = y;
        IElement::width = w;
        IElement::height = h;

    }

    virtual ~IElement() {}

    virtual void draw(InterfaceLayer* IL) {}

    virtual bool handleInput(Input_Event e)
    {

        return false;
    }

    float getX()
    {

        return x;
    }

    float getY()
    {

        return y;
    }

    float getWidth()
    {

        return width;
    }

    float getHeight()
    {

        return height;
    }
};

#endif //_INTERFACE_ELEMENT_DEF_H_
