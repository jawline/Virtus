#include "Plane.h"
#include "../GRender.h"

Plane::Plane(float sx, float sz, float ex, float ez)
{
    printf("PLANE %f %f %f %f\n", sx, sz, ex, ez);
    m_planeVSet = GRenderer::getRenderer()->getGraphicsAPI()->newVertexSet();

    float vertices[] = {
                            sx, 0, sz,
                            sx, 0, ez,
                            ex, 0, ez,
                            ex, 0, sz
                       };

    float colors[] =
    {
        0, 0, 1, 1,
        0, 0, 1, 1,
        0, 0, 1, 1,
        0, 0, 1, 1
    };

    m_planeVSet->setColourValues(colors, ctype_float_rgba, 4);
    m_planeVSet->setVertices(vertices, 4);
}

Plane::~Plane()
{

}

void Plane::draw(Camera* cam)
{
    GRenderer::getRenderer()->getGraphicsAPI()->pushMatrix();
    GRenderer::getRenderer()->getGraphicsAPI()->clearActiveTexture();

        m_planeVSet->Draw(0, 4, QUADS);

    GRenderer::getRenderer()->getGraphicsAPI()->popMatrix();
}
