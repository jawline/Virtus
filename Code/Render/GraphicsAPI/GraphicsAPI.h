#ifndef _GRAPHICS_API_DEF_H_
#define _GRAPHICS_API_DEF_H_
#include "VertexSet.h"
#include "Window.h"
#include "Texture.h"
#include "Shader.h"
#include "ShaderFactory.h"

class GraphicsAPI
{
private:

public:
    virtual ~GraphicsAPI() {}

    virtual RenderWindow* getRenderWindow() = 0;
    virtual VertexSet* newVertexSet() = 0;
    virtual const char* getAPISummary() = 0;
    virtual Texture* newTexture() = 0;

    virtual ShaderFactory* getShaderFactory() = 0;

    static GraphicsAPI* getAPIInstance(const char* Name);

    virtual void clearScreen() = 0;

    virtual bool initialize() = 0;
    virtual void setupFor3D(unsigned int width, unsigned int height) = 0;
    virtual void setupFor2D(unsigned int width, unsigned int height) = 0;

    virtual void pushMatrix() = 0;
    virtual void popMatrix() = 0;
    virtual void resetMatrix() = 0;

    virtual void translateMatrix(float x, float y, float z) = 0;
    virtual void rotateMatrix(float x, float y, float z) = 0;

    virtual void enableBlending() = 0;
    virtual void disableBlending() = 0;

    virtual void setWireframeRender(bool val) = 0;

    virtual void clearActiveTexture() = 0;

    virtual void setDepthTesting(bool value) = 0;

    virtual void setupViewport(unsigned int start_x, unsigned int start_y, unsigned int end_x, unsigned int end_y) = 0;

    //Just free (And report?) any errors that have occured within the API so nothing overflows
    virtual void clearErrors() = 0;

    virtual bool setupRenderToTextureTextures(unsigned int w, unsigned int h, Texture* colour, Texture* depth) = 0;
    virtual bool setRenderToTexture(unsigned int width, unsigned int height, Texture* tex, Texture* depth) = 0;
};

#endif //_GRAPHICS_API_DEF_H_
