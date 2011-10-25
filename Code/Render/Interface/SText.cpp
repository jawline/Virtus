#include "SText.h"
#include "InterfaceLayer.h"

IElements::StaticText::StaticText(int PS, int X, int Y, std::string T) : IElement(X, Y, 0, 0)
{
    m_pointSize = PS;
    m_x = X;
    m_y = Y;
    m_textData = T;
}

void IElements::StaticText::draw(InterfaceLayer* IL)
{
    IL->drawText(m_pointSize, m_x, m_y, m_textData, 0);
}
