#include "InterfaceLayer.h"
#include "../GRender.h"
#include "../GraphicsAPI/GraphicsAPI.h"
#include <Filesystem/Filesystem.h>

InterfaceLayer::InterfaceLayer()
{

    if ( m_defaultTextRenderer.Load( (File*) Filesystem::evaluateChild(Filesystem::getRoot(), "Data/fonts/default.ttf")) == false );
    {
        printf("InterfaceLayer: Unable to load Data/fonts/default.ttf. Continuing, expect errors\n");
    }

    m_interfaceTex = 0;

    m_interfaceTex = GRenderer::getRenderer()->getTextureMonitor()->getTexture("Data/gui.tga");

    //Create a vertex set used for the quad drawing function calls
    m_quadVertexSet = GRenderer::getRenderer()->getGraphicsAPI()->newVertexSet();

    //Create a base set of vertices to be copied into the vertex set
    float baseVertices[] = { 0, 0, 0,
                             0, 0, 0,
                             0, 0, 0,
                             0, 0, 0
                            };

    float baseTextureCoordinates[] = { 0, 0,
                                       0, 0,
                                       0, 0,
                                       0, 0
                                     };

    float baseColourValues[] = { 1, 1, 1,
                                 1, 1, 1,
                                 1, 1, 1,
                                 1, 1, 1
                                 };

    m_quadVertexSet->setVertices(baseVertices, 4);
    m_quadVertexSet->setTextureCoordinates(baseTextureCoordinates, 4);
    m_quadVertexSet->setColourValues(baseColourValues, ctype_float_rgb, 4);
}

void InterfaceLayer::draw(GRenderer*, int w, int h)
{

    GRenderer::getRenderer()->getGraphicsAPI()->setupFor2D(w, h);

    for (unsigned int i = 0; i < m_interfaceElements.size(); i++)
    {

        m_interfaceElements[i]->draw(this);

    }

}

void InterfaceLayer::drawText(int PointSize, int x, int y, std::string Text, FREngine* Renderer)
{

    if (Renderer == 0)
    {
        m_defaultTextRenderer.Render(PointSize, x, y, Text);
    }
    else
    {
        Renderer->Render(PointSize, x, y, Text);
    }

}

void InterfaceLayer::drawQuad(float x, float y, float w, float h)
{

    float* texCoordinates = m_quadVertexSet->getTextureCoordinates();
    float* vertices = m_quadVertexSet->getVertices();

    //First point
    texCoordinates[0] = 0; texCoordinates[1] = 1;
    vertices[0] = x; vertices[1] = y; vertices[2] = 0;

    //Second point
    texCoordinates[2] = 1; texCoordinates[3] = 1;
    vertices[3] = x + w; vertices[4] = y; vertices[5] = 0;

    //Third point
    texCoordinates[4] = 1; texCoordinates[5] = 0;
    vertices[6] = x + w; vertices[7] = y + h; vertices[8] = 0;

    //Fourth point
    texCoordinates[6] = 0; texCoordinates[7] = 0;
    vertices[9] = x; vertices[10] = y + h; vertices[11] = 0;

    m_quadVertexSet->Draw(0, 4, QUADS);
}

void InterfaceLayer::drawQuadWithUIElement(float x, float y, float w, float h, unsigned int element_x, unsigned int element_y)
{
    if (m_interfaceTex != 0)
        m_interfaceTex->makeActive();

    unsigned int m_uiElementWidth = 64;
    unsigned int m_uiElementHeight = 64;

    float u_start = 0;
    float u_end = 1;

    float v_start = 0;
    float v_end = 1;

    if (m_interfaceTex != 0)
    {
        unsigned int xPos = m_uiElementWidth * element_x;
        unsigned int xEndPos = m_uiElementWidth * (element_x + 1);

        u_start = ((float)xPos) / ((float) m_interfaceTex->getWidth());
        u_end = ((float)xEndPos) / ((float) m_interfaceTex->getHeight());

        unsigned int yPos = m_uiElementHeight * element_y;
        unsigned int yEndPos = m_uiElementHeight * (element_y + 1);

        v_start = ((float)yEndPos) / ((float) m_interfaceTex->getHeight());
        v_end = ((float)yPos) / ((float) m_interfaceTex->getHeight());

    }

    float* texCoordinates = m_quadVertexSet->getTextureCoordinates();
    float* vertices = m_quadVertexSet->getVertices();

    //First point
    texCoordinates[0] = u_start; texCoordinates[1] = v_end;
    vertices[0] = x; vertices[1] = y; vertices[2] = 0;

    //Second point
    texCoordinates[2] = u_end; texCoordinates[3] = v_end;
    vertices[3] = x + w; vertices[4] = y; vertices[5] = 0;

    //Third point
    texCoordinates[4] = u_end; texCoordinates[5] = v_start;
    vertices[6] = x + w; vertices[7] = y + h; vertices[8] = 0;

    //Fourth point
    texCoordinates[6] = u_start; texCoordinates[7] = v_start;
    vertices[9] = x; vertices[10] = y + h; vertices[11] = 0;

    m_quadVertexSet->Draw(0, 4, QUADS);
}

//Message every interface asking if it needs this event - on return true the loop breaks
bool InterfaceLayer::processEvent(Input_Event E)
{
    bool isHandled = false;

    for (unsigned int i = 0; i < m_interfaceElements.size(); i++)
    {

        bool cResult = m_interfaceElements[i]->handleInput(E);

        if (cResult)
        {
            isHandled = true;
            break;
        }

    }

    return isHandled;
}
