#ifndef _TERRAIN_DATA_DEF_H_
#define _TERRAIN_DATA_DEF_H_

class TerrainData
{
private:

    float* m_terrainData;
    float* m_uvData;
    float* m_lightmapData;
    float* m_normalData;

    unsigned int m_terrainLength;

public:

    TerrainData(unsigned int l_terrainLength);
    ~TerrainData();


    float* getTerrainData();
    float* getUVData();
    float* getLightmapData();
    float* getNormalData();

    unsigned int getTerrainLength() { return m_terrainLength; }

    void generateUsingSineFaultAlgorythm(unsigned int l_numIterations, float distortionStart, float distortionMin, float sineCurveMagnitude);
    void recalculateUV();
};

#endif //_TERRAIN_DATA_DEF_H_
