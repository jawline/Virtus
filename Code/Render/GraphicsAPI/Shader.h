#ifndef _GRAPHICS_API_SHADER_DEF_H_
#define _GRAPHICS_API_SHADER_DEF_H_
#include "Texture.h"

class Shader
{
private:

public:

    Shader() {}
    virtual ~Shader() {}

    virtual void activateShader() = 0;
    virtual void deactivateShader() = 0;

    virtual void setNumberOfTextures(unsigned int number) = 0;
    virtual void setTextureName(unsigned int i, const char* Name) = 0;
    virtual void setTexture(unsigned int i, Texture* tex) = 0;
};

#endif //_GRAPHICS_API_SHADER_DEF_H_
