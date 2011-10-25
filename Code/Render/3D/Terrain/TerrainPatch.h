#ifndef _TERRAIN_PATCH_DEF_H_
#define _TERRAIN_PATCH_DEF_H_
#include "../../GraphicsAPI/VertexSet.h"

class TerrainPatch
{

private:

    VertexSet** m_patchVSetLods;
    unsigned int* m_lodNumVs;
    unsigned int m_numLods;

    float m_patchSize;

    VertexSet* generateDrawLOD(float* heights, float* UV, unsigned int patchSize, unsigned int stride, unsigned int* lodPatchSize);

public:
    TerrainPatch(float* Heights, float* UV, unsigned int patchSize);
    ~TerrainPatch();

    void draw(unsigned int lod);

};

#endif //_TERRAIN_PATCH_DEF_H_
