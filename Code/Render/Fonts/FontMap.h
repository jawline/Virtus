#ifndef _FONT_MAP_H
#define _FONT_MAP_H

#include <ft2build.h>
#include FT_FREETYPE_H

#include <string>
#include <iostream>

#include "FontChar.h"
#include "../GraphicsAPI/VertexSet.h"


class FontMap
{
public:
    FontChar    CharList[255];

    int         PointSize;

    VertexSet* drawBuffer;

public:
    FontMap();

public:
    void Render(int Char, float x, float y);
    void Render(int Char, float x, float y, float offx, float offy, float w, float h);

    void Cleanup();

public:
    FontChar* GetChar(int Char);
};

#endif
