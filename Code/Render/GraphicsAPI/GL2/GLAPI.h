#ifndef _OPENGL2_GRAPHICS_API_DEF_H_
#define _OPENGL2_GRAPHICS_API_DEF_H_
#include "../GraphicsAPI.h"
#include "./VertexSet.h"
#include "./GLXWindow.h"
#include "GLTexture.h"
#include "GLShader.h"
#include "GLShaderFactory.h"

#define GLEW_HEADER <GL/glew.h>
#define GLEXT_HEADER <GL/glext.h>
#define GL_HEADER <GL/gl.h>
#define GLU_HEADER <GL/glu.h>

class GL2GraphicsAPI : public GraphicsAPI
{
private:
	static const char* TAG;

    GLuint frameBufferObjectID;
    GLuint renderBufferID;

    char* apiSummary;
    ShaderFactory* m_shaderFactory;

    void setAPISummary(const char* String);

    #if defined (_BUILD_WIN32_)

        #error "Windows support incomplete (GL2/GLAPI.h)"

    #elif defined(_BUILD_LINUX_)

        GWindowGLX m_renderWindow;
    #else

        #error "Platform not known/supported (GL2/GLAPI.h)"

    #endif

public:
    GL2GraphicsAPI();
    ~GL2GraphicsAPI();

    RenderWindow* getRenderWindow();
    VertexSet* newVertexSet() { return new GL2VertexSet(); }
    const char* getAPISummary() { return apiSummary; }
    Texture* newTexture() { return new GLTexture; }
    ShaderFactory* getShaderFactory() { return m_shaderFactory; }

    void clearScreen();

    bool initialize();
    void setupFor3D(unsigned int width, unsigned int height);
    void setupFor2D(unsigned int width, unsigned int height);

    void pushMatrix();
    void popMatrix();
    void resetMatrix();

    void translateMatrix(float x, float y, float z);
    void rotateMatrix(float x, float y, float z);

    void enableBlending();
    void disableBlending();

    void clearActiveTexture();

    void setupViewport(unsigned int start_x, unsigned int start_y, unsigned int end_x, unsigned int end_y);

    void clearErrors();
    void setDepthTesting(bool value);

    void setWireframeRender(bool val);

    bool setRenderToTexture(unsigned int width, unsigned int height, Texture* tex, Texture* depth);
    bool setupRenderToTextureTextures(unsigned int w, unsigned int h, Texture* colour, Texture* depth);
};

#endif //_OPENGL2_GRAPHICS_API_DEF_H_
