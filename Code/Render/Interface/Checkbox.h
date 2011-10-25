#ifndef _INTERFACE_CHECKBOX_DEF_H_
#define _INTERFACE_CHECKBOX_DEF_H_

#include "IElement.h"
#include "../TextureMonitor/TextureMonitor.h"
#include "Colour.h"

namespace IElements
{

class Checkbox : public IElement
{
protected:

    bool m_mouseOver;
    bool m_mouseDown;
    bool m_isChecked;

public:
    Checkbox(float x, float y, float w, float h, Colour m_downClr, Colour m_upClr);
    ~Checkbox();

    void draw(InterfaceLayer* IL);
    bool handleInput(Input_Event e);
};

}

#endif //_INTERFACE_CHECKBOX_DEF_H_
