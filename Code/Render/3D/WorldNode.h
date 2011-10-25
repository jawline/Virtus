#ifndef _3D_SKYDOME_DEF_H_
#define _3D_SKYDOME_DEF_H_
#include "SceneNode.h"
#include "../TextureMonitor/TextureMonitor.h"
#include "../GraphicsAPI/VertexSet.h"

class WorldNode : public SceneNode
{

private:

    VertexSet* m_skydomeVSet;
    Texture* m_panoramicDomeTexture;

    float m_radius;


    void f_calculateVertices();

public:
    WorldNode(const char* SkydomeFile, float r);
    ~WorldNode();

    void update(float seconds);
    void draw(Camera* cam);
};

#endif //_3D_SKYDOME_DEF_H_
