#include <GL/glew.h>
#include "../../GRender.h"
#include "GLAPI.h"
#include <string.h>

#include GL_HEADER
#include GLU_HEADER

const char* GL2GraphicsAPI::TAG = "GL2 Graphics API";

GL2GraphicsAPI::GL2GraphicsAPI()
{

    apiSummary = 0;

    setAPISummary("OpenGL2 GraphicsAPI");

    m_shaderFactory = new GLShaderFactory();
}


GL2GraphicsAPI::~GL2GraphicsAPI()
{
    m_renderWindow.destroy();

    glDeleteRenderbuffers(1, &renderBufferID);
    glDeleteFramebuffers(1, &frameBufferObjectID);

}

RenderWindow* GL2GraphicsAPI::getRenderWindow()
{
    return &m_renderWindow;
}

void GL2GraphicsAPI::setAPISummary(const char* stringSummary)
{
    if (apiSummary != 0)
        delete[] apiSummary;

    apiSummary = new char[strlen(stringSummary) + 1];
    strcpy(apiSummary, stringSummary);
}

void GL2GraphicsAPI::clearScreen()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool GL2GraphicsAPI::initialize()
{
    //Set the background colour
    glClearColor(0, 0, 0, 1);

    glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

    glClearDepth(1.0f);									// Depth Buffer Setup
    glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do

    glEnable(GL_TEXTURE_2D);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Set the blending function

    GLenum err = glewInit();

    if (err != GLEW_OK)
    {
        GRenderer::getRenderer()->getRenderLog()->writeLine(TAG, "GLEW failed to initialize\n");
        return false;
    }
    else
    {
        GRenderer::getRenderer()->getRenderLog()->writeLine(TAG, "GLEW initialized\n");
    }

    //Create a framebuffer
    glGenFramebuffers(1, &frameBufferObjectID);

    //Create a render buffer
    glGenRenderbuffers(1, &renderBufferID);

    return true;
}

void GL2GraphicsAPI::setupFor3D(unsigned int width, unsigned int height)
{
    glEnable(GL_DEPTH_TEST);							// Enables Depth Testing

    glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0f, ((float)width) / ((float)height), 1.0f, 100000.0f);

    glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
}

//Setup the scene for 2D drawing
void GL2GraphicsAPI::setupFor2D(unsigned int width, unsigned int height)
{
    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho (0, width, height, 0, 0, 1);

    glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

    return;
}

void GL2GraphicsAPI::pushMatrix()
{

    glPushMatrix();

}

void GL2GraphicsAPI::popMatrix()
{

    glPopMatrix();

}

void GL2GraphicsAPI::resetMatrix()
{

    glLoadIdentity();

}


void GL2GraphicsAPI::translateMatrix(float x, float y, float z)
{
    glTranslatef(x, y, z);
}

void GL2GraphicsAPI::rotateMatrix(float x, float y, float z)
{
    glRotatef(x, 1, 0, 0);
    glRotatef(y, 0, 1, 0);
    glRotatef(z, 0, 0, 1);
}

void GL2GraphicsAPI::enableBlending()
{
    glEnable (GL_BLEND);
}

void GL2GraphicsAPI::disableBlending()
{
    glDisable (GL_BLEND);
}

void GL2GraphicsAPI::clearActiveTexture()
{

    glBindTexture(GL_TEXTURE_2D, 0);

}

void GL2GraphicsAPI::setupViewport(unsigned int start_x, unsigned int start_y, unsigned int end_x, unsigned int end_y)
{
    glViewport(start_x, start_y, end_x, end_y);
}

void GL2GraphicsAPI::clearErrors()
{

    unsigned int errorRecv = 0;

    while ( (errorRecv = glGetError()) != 0)
    {
        printf("GL Error %i\n", errorRecv);
    }

}

void GL2GraphicsAPI::setDepthTesting(bool value)
{
    if (value)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);
}

void GL2GraphicsAPI::setWireframeRender(bool val)
{

    if (val == true)
    {
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    }
    else
    {
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    }

}

bool GL2GraphicsAPI::setRenderToTexture(unsigned int width, unsigned int height, Texture* tex, Texture* depth)
{
    if (tex == 0)
    {
        if (frameBufferObjectID != 0)
        {
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glPopAttrib();


        }

        return true;
    }
    else
    {

        //Bind the framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObjectID);

        glBindTexture(GL_TEXTURE_2D, ((GLTexture*)tex)->getTextureID());

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ((GLTexture*)tex)->getTextureID(), 0);

        if (depth == 0)
        {

            glBindRenderbuffer(GL_RENDERBUFFER, renderBufferID);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

            //Attach the render buffer as a depth buffer
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBufferID);

        }
        else
        {

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, ((GLTexture*)depth)->getTextureID(), 0);

        }

        glPushAttrib(GL_VIEWPORT_BIT);
        glViewport(0, 0, width, height);
        glEnable(GL_DEPTH_TEST);
        glClearDepth(1.0f);									// Depth Buffer Setup
        glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            printf("ERR with FBO\n");
            return false;
        }

        return true;
    }

}

bool GL2GraphicsAPI::setupRenderToTextureTextures(unsigned int width, unsigned int height, Texture* colour, Texture* depth)
{
    if (colour != 0)
    {
        GLuint tex = ((GLTexture*)colour)->getTextureID();

        glBindTexture(GL_TEXTURE_2D, tex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    }

    if (depth != 0)
    {
        GLuint tex = ((GLTexture*)depth)->getTextureID();

        glBindTexture(GL_TEXTURE_2D, tex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
    }

    return true;
}
