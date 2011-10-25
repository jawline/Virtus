#include "Button.h"
#include "InterfaceLayer.h"
#include <Input/Keys.h>
#include "../GRender.h"

IElements::Button::Button(float x, float y, float w, float h, const char* NormalText, const char* OverText, const char* DownText, Colour textClr, Colour bgNormal, Colour mouseOver, Colour mouseDown) : IElement(x, y, w, h)
{
    m_mouseDownOverElement = false;

    m_buttonNormalText = new char[strlen(NormalText)];
    strcpy(m_buttonNormalText, NormalText);

    m_mouseOverText = new char[strlen(OverText)];
    strcpy(m_mouseOverText, OverText);

    m_mouseDownText = new char[strlen(DownText)];
    strcpy(m_mouseDownText, DownText);

    m_textColour = textClr;
    m_normalBg = bgNormal;
    m_mouseOverBg = mouseOver;
    m_btnDownBg = mouseDown;

    onUpFn = 0;
}

IElements::Button::~Button()
{
    delete[] m_buttonNormalText;
    delete[] m_mouseOverText;
    delete[] m_mouseDownText;

    printf("Button freed\n");
}

void IElements::Button::draw(InterfaceLayer* IL)
{

    GRenderer::getRenderer()->getGraphicsAPI()->enableBlending();

    Colour drawColour;

    char* m_drawStr = 0;

    if (m_mouseDownOverElement == true)
    {

        m_drawStr = m_mouseDownText;
        drawColour = m_btnDownBg;

    }
    else if (m_mouseOverElement == true)
    {

        m_drawStr = m_mouseOverText;
        drawColour = m_mouseOverBg;

    }
    else
    {

        m_drawStr = m_buttonNormalText;
        drawColour = m_normalBg;

    }


//    IL->nullActiveTexture();

 //   IL->setActiveColour(drawColour);

    unsigned int l_uiLeftLength = 30; //10 pixels for the left
    unsigned int l_uiRightLength = 30; //10 pixels for the right

    unsigned int l_uiTopHeight = 5;
    unsigned int l_uiBottomHeight = 5;

    unsigned int l_centerHeight = height - l_uiTopHeight - l_uiBottomHeight;

    unsigned int l_centerLength = width - l_uiLeftLength - l_uiRightLength;

    IL->drawQuadWithUIElement(x, y, l_uiLeftLength, height, 0, 0);

    if ((l_centerLength > 0) && (l_centerHeight > 0))
    {
        IL->drawQuadWithUIElement(x + l_uiLeftLength, y, l_centerLength, l_uiTopHeight, 2, 0);
        IL->drawQuadWithUIElement(x + l_uiLeftLength, y + l_uiTopHeight, l_centerLength, l_centerHeight, 4, 0);
        IL->drawQuadWithUIElement(x + l_uiLeftLength, y + l_uiTopHeight + l_centerHeight, l_centerLength, l_uiTopHeight, 3, 0);
    }

    IL->drawQuadWithUIElement(x + l_uiLeftLength + l_centerLength, y, l_uiRightLength, height, 1, 0);

//TODO: Some other way of pushing colour    IL->setActiveColour(m_textColour);

    IL->getDefaultTextRenderer()->Render(64, x + (width / 10) , y + height - (height / 4), width - ((width / 10) * 2), height - (height / 2), m_drawStr);


    GRenderer::getRenderer()->getGraphicsAPI()->disableBlending();
}

void IElements::Button::handleButtonEvent(Input_Event e)
{

    if (e.Event_Data[0] == MOUSE_DOWN)
    {

        onDown();

        m_mouseDownOverElement = true;

    }
    else if (e.Event_Data[0] == MOUSE_UP)
    {
        if (m_mouseDownOverElement == true)
        {

            onUp();

        }

        m_mouseDownOverElement = false;
    }
    else if (e.Event_Data[0] == MOUSE_MOTION)
    {

        m_mouseOverElement = true;

    }

}

void IElements::Button::setOnMouseUp(void (*newOnUpFn)(Button* Btn))
{
    onUpFn = newOnUpFn;
}

void IElements::Button::onUp()
{

    if (onUpFn)
        onUpFn(this);

}

bool IElements::Button::handleInput(Input_Event e)
{
    bool m_Result = false;

    switch (e.Event_ID)
    {
    case input_event_button_one:
    {
        float event_x, event_y;
        event_x = e.Event_Data[1];
        event_y = e.Event_Data[2];

        bool m_Over = false;

        if (event_x > x && event_x < (x + width))
        {
            if (event_y > y && event_y < (y + height))
            {
                handleButtonEvent(e);
                m_Over = true;
            }
        }

        if (m_Over)
        {
            m_mouseOverElement = true;
        }
        else
        {
            m_mouseDownOverElement = false;
            m_mouseOverElement = false;
        }

        return m_Over;

        break;
    }
    }

    return m_Result;
}
