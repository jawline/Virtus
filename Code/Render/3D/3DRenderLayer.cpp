#include "3DRenderLayer.h"
#include "../TextureMonitor/TextureMonitor.h"

ThreeDRenderLayer::ThreeDRenderLayer()
{
    m_worldNode = 0;
}

ThreeDRenderLayer::~ThreeDRenderLayer()
{
}

void ThreeDRenderLayer::draw(GRenderer* renderer, int width, int height)
{

    if (renderer->getRenderWireframeValue() == true)
    {
        GRenderer::getRenderer()->getGraphicsAPI()->setWireframeRender(true);
    }

    GRenderer::getRenderer()->getGraphicsAPI()->setupFor3D(width, height);
    GRenderer::getRenderer()->getGraphicsAPI()->resetMatrix();

    if (m_worldNode != 0)
    {
        m_worldNode->draw(&m_worldCamera);
    }

    if (renderer->getRenderWireframeValue() == true)
    {
        GRenderer::getRenderer()->getGraphicsAPI()->setWireframeRender(false);
    }

}



void ThreeDRenderLayer::update(float s)
{
    if (m_worldNode != 0)
    {
        m_worldNode->update(s);
    }
}

void ThreeDRenderLayer::setWorldNode(SceneNode* node)
{
    m_worldNode = node;
}
