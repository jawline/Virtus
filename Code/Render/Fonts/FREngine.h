#ifndef _FR_ENGINE_H
#define _FR_ENGINE_H
#include <ft2build.h>
#include FT_FREETYPE_H
#include <string>
#include <iostream>
#include <vector>
#include <Filesystem/File.h>

#include "FontMap.h"

class FREngine
{
public:
    std::vector<FontMap>    FontMaps;
private:
    static const char* TAG;

    FT_Library	    Library;
    FT_Face         FontFace;
    FT_GlyphSlot    GlyphSlot;

public:
    FREngine();
    ~FREngine();

    bool Init();
    bool Load(File* file);
    void LoadFontmap(int PointSize);
    void Render(int PointSize, unsigned int X, unsigned int Y, std::string Line);
    void Render(int PointSize, unsigned int X, unsigned int Y, unsigned int drawWidth, unsigned int drawHeight, std::string Line);
    void Cleanup();

public:
    void SetFontPointSize(int Point);
    void SetFontPixelSize(int Pixels);

private:
    int NextPowerTwo(int X);

};

#endif
