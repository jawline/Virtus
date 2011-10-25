#ifndef _SCENE_PLANE_DEF_H_
#define _SCENE_PLANE_DEF_H_
#include "SceneNode.h"
#include "../GraphicsAPI/VertexSet.h"

class Plane : public SceneNode
{
private:
    VertexSet* m_planeVSet;

public:

    Plane(float sx, float sy, float ex, float ey);
    ~Plane();

    void draw(Camera* cam);

};


#endif //_SCENE_PLANE_DEF_H_
