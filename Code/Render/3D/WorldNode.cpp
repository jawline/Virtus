#include "WorldNode.h"
#include "../GRender.h"
#include <math.h>
#include "./3DRenderLayer.h"

const unsigned int phi_stride = 10;
const unsigned int theta_stride = 10;

inline float getXPoint(float phi, float theta, float radius)
{
    return radius * sin ( phi * (M_PI / 180.0f) ) * cosf ( theta * (M_PI / 180.0f));
}

inline float getYPoint(float phi, float theta, float radius)
{
    return radius * sin ( phi * (M_PI / 180.0f) ) * sinf ( theta * (M_PI / 180.0f));
}

inline float getZPoint(float phi, float theta, float radius)
{
    return radius * cos ( phi * (M_PI / 180.0f));
}

inline float getUPoint(float phi, float theta, float r)
{
    return sin(theta * (M_PI / 180.0f)) * cos(phi * (M_PI / 180.0f));
}

inline float getVPoint(float phi, float theta, float r)
{
    return sin(theta * (M_PI / 180.0f)) * sin(phi * (M_PI / 180.0f));
}


inline unsigned int addToVertices(float phi, float theta, float r, float* Vertices, unsigned int currentVPtr)
{

    Vertices[currentVPtr] = getXPoint(phi, theta, r);
    currentVPtr++;

    Vertices[currentVPtr] = getYPoint(phi, theta, r);
    currentVPtr++;

    Vertices[currentVPtr] = getZPoint(phi, theta, r);
    currentVPtr++;

    return currentVPtr;
}

inline unsigned int addToUV(float phi, float theta, float r, float* uvPtr, unsigned int currentUVP)
{

    //float x = getXPoint(phi, theta, r);
    float y = getYPoint(phi, theta, r);
    //float z = getZPoint(phi, theta, r);

    float uComponent = phi / 360.0f;
//    float vComponent = theta / 180.0f;

    uvPtr[currentUVP] = uComponent;
    currentUVP++;

    uvPtr[currentUVP] = 1 - (y / r);
    currentUVP++;

    return currentUVP;
}

WorldNode::WorldNode(const char* Viewfile, float radius)
{

    printf("Created Skydome\n");

    m_skydomeVSet = GRenderer::getRenderer()->getGraphicsAPI()->newVertexSet();

    m_radius = radius;

    m_panoramicDomeTexture = GRenderer::getRenderer()->getTextureMonitor()->getTexture(Viewfile);

    f_calculateVertices();
}

WorldNode::~WorldNode()
{
    GRenderer::getRenderer()->getTextureMonitor()->releaseTexture(m_panoramicDomeTexture);
    delete m_skydomeVSet;
    printf("Destroyed Skydome\n");
}

void WorldNode::f_calculateVertices()
{
    float* m_skydomeVertices, *m_skydomeUV;

    m_skydomeVertices = new float[ (180 / theta_stride) * (180 / phi_stride) * 4 * 3];
    m_skydomeUV = new float[ (180 / theta_stride) * (180 / phi_stride) * 4 * 2];

    unsigned int vPtr = 0;
    unsigned int uvPtr = 0;

    for (unsigned int phi = 0; phi <= 180 - theta_stride; phi += phi_stride)
    {

            for (unsigned int theta = 0; theta <= 180 - theta_stride; theta += theta_stride)
            {
                vPtr = addToVertices(phi, theta, m_radius, m_skydomeVertices, vPtr);
                uvPtr = addToUV(phi, theta, m_radius, m_skydomeUV, uvPtr);

                vPtr = addToVertices(phi + phi_stride, theta, m_radius, m_skydomeVertices, vPtr);
                uvPtr = addToUV(phi + phi_stride, theta, m_radius, m_skydomeUV, uvPtr);

                vPtr = addToVertices(phi + phi_stride, theta + theta_stride, m_radius, m_skydomeVertices, vPtr);
                uvPtr = addToUV(phi + phi_stride, theta + theta_stride, m_radius, m_skydomeUV, uvPtr);

                vPtr = addToVertices(phi, theta + theta_stride, m_radius, m_skydomeVertices, vPtr);
                uvPtr = addToUV(phi, theta + theta_stride, m_radius, m_skydomeUV, uvPtr);
            }
    }


    m_skydomeVSet->setVertices( m_skydomeVertices, (180 / theta_stride) * (180 / phi_stride) * 4);
    m_skydomeVSet->setTextureCoordinates( m_skydomeUV, (180 / theta_stride) * (180 / phi_stride) * 4);

    delete[] m_skydomeVertices;
    delete[] m_skydomeUV;

}

void WorldNode::update(float seconds)
{
    SceneNode::update(seconds);
}

void WorldNode::draw(Camera* activeCam)
{
    float* rMatrix = 0;
    float* pMatrix = 0;

    GRenderer::getRenderer()->getGraphicsAPI()->pushMatrix();

        GRenderer::getRenderer()->getGraphicsAPI()->resetMatrix();

        GRenderer::getRenderer()->getGraphicsAPI()->setDepthTesting(false);


        rMatrix = activeCam->getRotation().getMatrix();

        GRenderer::getRenderer()->getGraphicsAPI()->rotateMatrix(rMatrix[0], rMatrix[1], rMatrix[2]);

        if (m_panoramicDomeTexture != 0)
            m_panoramicDomeTexture->makeActive();

        m_skydomeVSet->Draw(0, (180 / theta_stride) * (180 / phi_stride) * 4, QUADS);

        GRenderer::getRenderer()->getGraphicsAPI()->setDepthTesting(true);


    GRenderer::getRenderer()->getGraphicsAPI()->popMatrix();

    GRenderer::getRenderer()->getGraphicsAPI()->pushMatrix();

        rMatrix = activeCam->getRotation().getMatrix();

        GRenderer::getRenderer()->getGraphicsAPI()->rotateMatrix(rMatrix[0], rMatrix[1], rMatrix[2]);

        pMatrix = activeCam->getPosition().getMatrix();

        GRenderer::getRenderer()->getGraphicsAPI()->translateMatrix(pMatrix[0], pMatrix[1], pMatrix[2]);

        SceneNode::draw(activeCam);

    GRenderer::getRenderer()->getGraphicsAPI()->popMatrix();
}
