#ifndef _INTERFACE_ELEMENT_DEF_H_
#define _INTERFACE_ELEMENT_DEF_H_
#include <Input/InputEvent.h>

class InterfaceLayer;

/**
 * @brief the abstraction of a base element within the engine
 */
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

    virtual void draw(InterfaceLayer* IL) = 0;

    virtual bool handleInput(Input_Event e) = 0;

    /**
     * @brief Return the X position of the element
     *
     */

    float getX()
    {
        return x;
    }

    /**
     * @brief Return the Y position of the element
     *
     */

    float getY()
    {

        return y;
    }

    /**
     * @brief Get the width of the element
     *
     */
    float getWidth()
    {

        return width;
    }

    /**
     * @brief Get the height of the element
     *
     */
    float getHeight()
    {

        return height;
    }
};

#endif //_INTERFACE_ELEMENT_DEF_H_
