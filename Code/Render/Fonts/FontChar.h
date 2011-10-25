#ifndef _FONT_CHAR_H
#define _FONT_CHAR_H
#include "../GraphicsAPI/Texture.h"

class FontChar
{

public:
    FontChar();
    ~FontChar();

public:
    Texture*          charTexture;

    int             CharWidth;
    int             CharHeight;

    int             Width;
    int             Height;

    int             AdvanceX;

    int             OffsetX;
    int             OffsetY;
};

#endif
