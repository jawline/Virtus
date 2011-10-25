#ifndef _VOXEL_OBJECT_DEF_H_
#define _VOXEL_OBJECT_DEF_H_
#include "SceneNode.h"

class Element
{
public:

    Element* children[8];
};

class VoxelObject : public SceneNode
{
private:


public:
    VoxelObject();
    ~VoxelObject();

    void draw(Camera* cam);
};

#endif //_VOXEL_OBJECT_DEF_H_
