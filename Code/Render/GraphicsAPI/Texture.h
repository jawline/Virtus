#ifndef _RENDER_TEXTURE_DEF_H_
#define _RENDER_TEXTURE_DEF_H_
#include <stdio.h>

enum InputDataFormat
{
    input_data_byte_rgb = 0,
    input_data_float_rgb,
    input_data_byte_rgba,
    input_data_float_rgba,
    input_data_byte_luminance,
    input_data_float_luminance,
    input_data_byte_luminance_alpha,
    input_data_float_luminance_alpha
};

class Texture
{
protected:

    unsigned int m_textureWidth, m_textureHeight;

public:

    Texture()
    {
        m_textureWidth = 0;
        m_textureHeight = 0;
    }

    virtual ~Texture()
    {
    }


    virtual void loadTextureData(InputDataFormat format, unsigned int width, unsigned int height, bool generateMipMaps, void* inputData) = 0;
    virtual void makeActive() = 0;

    unsigned int getWidth()
    {
        return m_textureWidth;
    }

    unsigned int getHeight()
    {
        return m_textureHeight;
    }

};

#endif //_TEXTURE_DEF_H_
