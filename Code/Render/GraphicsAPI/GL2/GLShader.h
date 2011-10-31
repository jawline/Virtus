#ifndef _OPENGL_SHADER_DEF_H_
#define _OPENGL_SHADER_DEF_H_
#include "../Shader.h"

#include <GL/gl.h>

class GLShader : public Shader
{
private:
	static const char* TAG;
	static unsigned int m_numCreated;

    unsigned int m_debugID;

    GLuint m_vertexShaderID;
    GLuint m_fragmentShaderID;
    GLuint m_shaderProgramID;

    GLuint* m_textureLocations;
    unsigned int m_numTextures;

    void deleteShader();

public:

    GLShader();
    ~GLShader();

    bool loadShader(const char* vert, const char* frag);

    void activateShader();
    void deactivateShader();


    void setNumberOfTextures(unsigned int number);
    void setTextureName(unsigned int i, const char* Name);
    void setTexture(unsigned int i, Texture* tex);
};

#endif //_OPENGL_SHADER_DEF_H_
