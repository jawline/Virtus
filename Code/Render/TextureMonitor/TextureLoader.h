#ifndef _TGA_LOADER_DEF_H_
#define _TGA_LOADER_DEF_H_
#include "../GraphicsAPI/Texture.h"
#include <Filesystem/Stream.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

struct tga_header
{

    int8_t identLength;

    int8_t colourMapType;

    int8_t imageType;
    int16_t colourMapStart;
    int16_t colourMapLength;
    int8_t colourMapBits;

    int16_t xStart;
    int16_t yStart;

    int16_t width;
    int16_t height;

    int8_t bits;
    int8_t descriptor;

} __attribute__ ((packed));

class TextureLoader
{

public:

    bool load(Texture* Tex, DataInputStream* inStream);

private:

    bool loadCompressedTGA(Texture* Tex, DataInputStream* f, tga_header* head);
    bool loadUncompressedTGA(Texture* Tex, DataInputStream* f, tga_header* head);

};

#endif //_TGA_LOADER_DEF_H_
