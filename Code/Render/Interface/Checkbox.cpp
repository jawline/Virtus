#include "Checkbox.h"
#include "InterfaceLayer.h"
#include "../GRender.h"
#include <Input/Keys.h>

Checkbox::Checkbox(float x, float y, float w, float h, Color m_downClr, Color m_upClr) : IElement(x, y, w, h)
{
    m_mouseOver = false;
    m_mouseDown = false;
    m_isChecked = false;
}

Checkbox::~Checkbox()
{

}


void Checkbox::draw(InterfaceLayer* IL)
{

    GRenderer::getRenderer()->getGraphicsAPI()->enableBlending();


    if (m_isChecked)
    {
        if (m_mouseOver)
        {
            IL->drawQuadWithUIElement(x, y, width, height, 3, 1);
        }
        else
        {
            IL->drawQuadWithUIElement(x, y, width, height, 1, 1);
        }
    }
    else if (m_mouseOver)
    {
        IL->drawQuadWithUIElement(x, y, width, height, 2, 1);
    }
    else
    {
        IL->drawQuadWithUIElement(x, y, width, height, 0, 1);
    }

    GRenderer::getRenderer()->getGraphicsAPI()->disableBlending();
}

bool Checkbox::handleInput(Input_Event e)
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

                    m_Over = true;

                    if (e.Event_Data[0] == MOUSE_DOWN)
                    {
                        m_mouseDown = true;
                    }
                    else if (e.Event_Data[0] == MOUSE_UP)
                    {
                        if (m_mouseDown)
                            m_isChecked = !m_isChecked;
                    }


                }
            }

            if (m_Over)
            {

                m_mouseOver = true;

            }
            else
            {
                m_mouseDown = false;
                m_mouseOver = false;
            }

            return false;

            break;
        }

    }

    return m_Result;
    return false;
}
