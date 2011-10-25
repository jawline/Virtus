#ifndef _3D_RENDER_LAYER_DEF_H_
#define _3D_RENDER_LAYER_DEF_H_
#include "../GRenderLayer.h"
#include "../GRender.h"
#include "SceneNode.h"
#include "Camera.h"
#include <vector>

class ThreeDRenderLayer : public GRenderLayer
{

private:

    Camera m_worldCamera;
    SceneNode* m_worldNode;

public:

    ThreeDRenderLayer();
    ~ThreeDRenderLayer();

    void draw(GRenderer* renderer, int width, int height);
    void update(float seconds);

    ///Set the active world node, pass NULL if you don't want a world to be rendered
    void setWorldNode(SceneNode* worldNode);

    Camera* getActiveCamera()
    {

        return &m_worldCamera;

    }


};

#endif //_3D_RENDER_LAYER_DEF_H_
