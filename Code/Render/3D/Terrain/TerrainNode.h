#ifndef _TERRAIN_NODE_DEF_H_
#define _TERRAIN_NODE_DEF_H_
#include "../SceneNode.h"
#include "../../GRender.h"
#include "../../GraphicsAPI/VertexSet.h"
#include "../../TextureMonitor/TextureMonitor.h"
#include <Render/GraphicsAPI/Shader.h>
#include <Filesystem/File.h>

#include "TerrainData.h"
#include "TerrainPatch.h"

const unsigned int slopeLighting = 1;
const unsigned int positionalLighting = 2;
const unsigned int heightBasedLightmap = 3;

class TerrainNode : public SceneNode
{
private:

    unsigned int m_patchSize;
    unsigned int m_numPatches;
    TerrainPatch** m_patches;

    float scaleFactorX;
    float scaleFactorY;
    float scaleFactorZ;

    float m_ambientLight;

    Shader* m_terrainShader;
    float* m_lightingData;
    float* m_normalData;

    bool m_lightmapDirty;

    TerrainData* m_terrainDataCl;

    Texture* m_terrainTextureData;
    Texture* m_terrainLightMap;

    void recalculateLightMaps();
    void recalculateTerrainCoordinates();

    float getBrightnessAtPoint(unsigned int x, unsigned int z);

    void terrainCrossProduct(unsigned int x1, unsigned int z1, unsigned int x2, unsigned int z2, unsigned int x3, unsigned int z3, float* auxNormal);
    void terrainNormalize(float *v);
    void terrainAddVector(float* v, float* o);


public:
    TerrainNode();
    ~TerrainNode();

    void update(float seconds);

    //Draws the terrain
    void draw(Camera* cam);

};

#endif //_TERRAIN_NODE_DEF_H_
