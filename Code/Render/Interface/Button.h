#ifndef _INTERFACE_ELEMENT_BUTTON_
#define _INTERFACE_ELEMENT_BUTTON_
#include "IElement.h"
#include "../TextureMonitor/TextureMonitor.h"
#include "Colour.h"

namespace IElements
{

class Button : public IElement
{
protected:

    unsigned int m_textPointSize;
    Colour m_textColour;
    Colour m_normalBg;
    Colour m_btnDownBg;
    Colour m_mouseOverBg;

    bool m_mouseOverElement;
    bool m_mouseDownOverElement;

    void (*onUpFn)(Button* Btn);

    virtual void onDown() { }
    void onUp();

    void handleButtonEvent(Input_Event e);

    char* m_buttonNormalText;
    char* m_mouseOverText;
    char* m_mouseDownText;

public:
    Button(float x, float y, float w, float h, const char* BtnNormalText, const char* BtnOverText, const char* BtnDownText, Colour textClr, Colour bgNormal, Colour mouseOver, Colour mouseDown);
    ~Button();

    void draw(InterfaceLayer* IL);
    bool handleInput(Input_Event e);

    void setOnMouseUp(void (*onUpFn)(Button* Btn));
};

}

#endif //_INTERFACE_ELEMENT_BUTTON_
