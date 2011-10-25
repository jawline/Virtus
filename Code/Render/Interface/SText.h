#ifndef _STATIC_TEXT_DEF_H_
#define _STATIC_TEXT_DEF_H_
#include "IElement.h"
#include <iostream>

/**
 * @brief A piece of static text to be displayed
 */

class StaticText : public IElement
{
protected:
    int m_x, m_y, m_pointSize;
    std::string m_textData;

public:
    StaticText(int PS, int x, int y, std::string T);
    void draw(InterfaceLayer* IL);
};

#endif //_STATIC_TEXT_DEF_H_
