#ifndef _GRAPHICS_API_FRAGMENT_SHADER_FACTORY_
#define _GRAPHICS_API_FRAGMENT_SHADER_FACTORY_

#include "Shader.h"
#include <Filesystem/File.h>

class ShaderFactory
{
private:

public:

    ShaderFactory() {}
    virtual ~ShaderFactory() {}

    virtual Shader* loadShader(File* shaderFile) = 0;
};

#endif //_GRAPHICS_API_FRAGMENT_SHADER_FACTORY_
