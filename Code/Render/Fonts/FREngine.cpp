#include "FREngine.h"
#include "../GRender.h"
#include <stdint.h>

FREngine::FREngine()
{

    Init();

}

FREngine::~FREngine()
{

    Cleanup();

}

bool FREngine::Init()
{

    if(FT_Init_FreeType(&Library))
    {
        return false;
    }

    if (GRenderer::getRenderer()->getRenderLog() != 0)
        GRenderer::getRenderer()->getRenderLog()->writeData("Initialized FreeType font engine\n");

    return true;
}

bool FREngine::Load(File* filenode)
{
    //Null sent return false
    if (filenode == 0) return false;

    //Type not a file, return false
    if (filenode->getType() != fileId) return false;

    //Empty file obviously isn't a font
    if (filenode->length() == 0) return false;

    DataInputStream* strm = filenode->createInputStream();

    if (strm == 0) return false;

    if (strm->open(false) == false)
    {
        delete strm;
        return false;
    }

    unsigned char* data = new unsigned char[ filenode->length() ];

    if (strm->read(data, filenode->length()) == false)
    {
        delete[] data;
        delete strm;

        return false;
    }

    delete strm;

    int Return = FT_New_Memory_Face(Library, data, filenode->length(), 0, &FontFace);

    //TODO: Work out a way of deleting the data when it's no longer needed
    //delete[] data;

    if(Return == FT_Err_Unknown_File_Format)
    {
        //The file is not in the right file format.
        return false;
    }
    else if(Return)
    {
        //Unable to load font file.
        return false;
    }

    GlyphSlot = FontFace->glyph;

    std::string lString = "Loaded freetype font: ";
    lString += filenode->getName();
    lString += "\n";

    if (GRenderer::getRenderer()->getRenderLog() != 0)
        GRenderer::getRenderer()->getRenderLog()->writeData(lString.c_str());

    return true;
}

void FREngine::LoadFontmap(int PointSize)
{
    SetFontPointSize(PointSize);

    FontMap newFontMap;

    newFontMap.PointSize = PointSize;

    for(int c = 0; c < 255; c++)
    {

        FT_Load_Char(FontFace, (char)c, FT_LOAD_RENDER);

        newFontMap.CharList[c].CharWidth   = GlyphSlot->bitmap.width;
        newFontMap.CharList[c].CharHeight  = GlyphSlot->bitmap.rows;

        newFontMap.CharList[c].Width   = NextPowerTwo(GlyphSlot->bitmap.width);
        newFontMap.CharList[c].Height  = NextPowerTwo(GlyphSlot->bitmap.rows);
        newFontMap.CharList[c].AdvanceX = GlyphSlot->advance.x >> 6;

        newFontMap.CharList[c].OffsetX = GlyphSlot->bitmap_left;
        newFontMap.CharList[c].OffsetY = GlyphSlot->bitmap_top;

        uint8_t* Data = new uint8_t[2 * newFontMap.CharList[c].Width * newFontMap.CharList[c].Height];

        for(int j = 0; j < newFontMap.CharList[c].Height; j++)
        {

            for(int i = 0; i < newFontMap.CharList[c].Width; i++)
            {

                Data[2 * (i + j * newFontMap.CharList[c].Width)] = Data[2 * (i + j* newFontMap.CharList[c].Width) + 1] =
                            (i >= GlyphSlot->bitmap.width || j >= GlyphSlot->bitmap.rows) ?
                            0 : GlyphSlot->bitmap.buffer[i + GlyphSlot->bitmap.width * j];
            }
        }

        newFontMap.CharList[c].charTexture = GRenderer::getRenderer()->getGraphicsAPI()->newTexture();

        newFontMap.CharList[c].charTexture->loadTextureData(input_data_byte_luminance_alpha, newFontMap.CharList[c].Width, newFontMap.CharList[c].Height, false, Data);

        delete[] Data;
    }

    FontMaps.push_back(newFontMap);

    printf("Pushed back fontmap\n");
}

//TODO: Merge the two render functions
void FREngine::Render(int PointSize, unsigned int X, unsigned int Y, std::string Text)
{
    printf("Render called\n");

    int Index = -1;

    for(unsigned int i = 0; i < FontMaps.size(); i++)
    {

        if(FontMaps[i].PointSize == PointSize)
        {

            Index = i;
            break;

        }

    }

    //Cast to a integer here to that a compiler error isn't thrown due to the precision loss between unsigned and signed
    if(Index < 0 || Index >= static_cast<int>(FontMaps.size()))
    {
        LoadFontmap(PointSize);
        Index = FontMaps.size() - 1;

        if(Index <= 0)
        {
            return;
        }
    }

    for(unsigned int i = 0; i < Text.size(); i++)
    {

        FontMaps[Index].Render(Text[i], X, Y);
        FontChar* tempChar = FontMaps[Index].GetChar(Text[i]);

        if(tempChar)
        {

            X += tempChar->AdvanceX;

        }

    }
}

void FREngine::Render(int PointSize, unsigned int X, unsigned int Y, unsigned int width, unsigned int height, std::string Text)
{

    int Index = -1;

    for(unsigned int i = 0; i < FontMaps.size(); i++)
    {
        if(FontMaps[i].PointSize == PointSize)
        {
            Index = i;
            break;
        }
    }

    //Cast to avoid precision loss warning
    if(Index < 0 || Index >= static_cast<int>(FontMaps.size()))
    {
        LoadFontmap(PointSize);
        Index = FontMaps.size() - 1;

        if(Index <= 0)
        {
            return;
        }
    }


    int normalWidth = 0;
    int normalHeightMax = 0;

    for (unsigned int i = 0; i < Text.size(); i++)
    {

        FontChar* tempChar = FontMaps[Index].GetChar(Text[i]);
        normalWidth += tempChar->AdvanceX;

        if (normalHeightMax < tempChar->CharHeight)
        {

            normalHeightMax = tempChar->CharHeight;

        }

    }

    float XScaler = ((float) width) / ((float) normalWidth);
    float YScaler = ((float) height) / ((float) normalHeightMax);

    for(unsigned int i = 0; i < Text.size(); i++)
    {

        FontChar* tempChar = FontMaps[Index].GetChar(Text[i]);

        //FontMaps[Index].Render(Text[i], X, Y, tempChar->OffsetX * XScaler, tempChar->OffsetY, tempChar->Width * XScaler, tempChar->Height);
        FontMaps[Index].Render(Text[i], X, Y, tempChar->OffsetX * XScaler, -tempChar->OffsetY * YScaler, tempChar->Width * XScaler, tempChar->Height * YScaler);

        if(tempChar)
        {

            X += (tempChar->AdvanceX * XScaler);

        }

    }

}

void FREngine::Cleanup()
{

    GRenderer::getRenderer()->getRenderLog()->writeData("Freeing up FreeType resources\n");

    for(unsigned int i = 0; i < FontMaps.size(); i++)
    {
        FontMaps[i].Cleanup();
    }

    FontMaps.clear();

    FT_Done_FreeType(Library);
}

void FREngine::SetFontPointSize(int Point)
{
    FT_Set_Char_Size(FontFace, 0, Point * 64, 72, 72);
}

void FREngine::SetFontPixelSize(int Pixels)
{
    FT_Set_Pixel_Sizes(FontFace, 0, Pixels);
}

int FREngine::NextPowerTwo(int X)
{
    int rval = 1;

    while(rval < X) rval <<= 1;

    return rval;
}
