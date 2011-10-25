#ifndef _TEST_SCENE_NODE_DEF_H_
#define _TEST_SCENE_NODE_DEF_H_

#include "SceneNode.h"
#include "../GraphicsAPI/VertexSet.h"
#include "../GraphicsAPI/Shader.h"
#include "../TextureMonitor/TextureMonitor.h"

class TestNode : public SceneNode
{
private:
    Shader* nodeShader;
    VertexSet* nodeVertexSet;
    Texture* nodeTexture;

    char* nodeName;

public:
    TestNode();
    ~TestNode();

    void update(float seconds);
    void draw(Camera* cam);

    bool load(const char* Filename);

};

#endif //_TEST_SCENE_NODE_DEF_H_
