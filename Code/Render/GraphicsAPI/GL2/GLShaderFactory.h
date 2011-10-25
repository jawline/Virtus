#ifndef _GL_SHADER_FACTORY_DEF_H_
#define _GL_SHADER_FACTORY_DEF_H_
#include "../ShaderFactory.h"

class GLShaderFactory : public ShaderFactory
{
public:
    int readFragmentShaderSourceFromFile(const char* Filename, char *readBuffer, unsigned int maxReadLength);
    int readVertexShaderSourceFromFile(const char* Filename, char *readBuffer, unsigned int maxReadLength);
    Shader* loadShader(File* inputFile);
};

#endif //_GL_SHADER_FACTORY_DEF_H_
