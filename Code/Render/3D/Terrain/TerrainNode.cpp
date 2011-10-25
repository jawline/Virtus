#include "TerrainNode.h"
#include "../3DRenderLayer.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glext.h>

float terrainOriginalLight = 1.0f;

const unsigned int verticesPerPatch = 129;

TerrainNode::TerrainNode()
{

    m_terrainTextureData = GRenderer::getRenderer()->getTextureMonitor()->getTexture("Data/textures/terrain_1.tga");
    m_terrainLightMap = GRenderer::getRenderer()->getGraphicsAPI()->newTexture();

    m_terrainDataCl = new TerrainData(513);

    m_terrainDataCl->generateUsingSineFaultAlgorythm(40, 10, 1, 30);

    //1 point, luminosity for each terrain point
    m_lightingData = m_terrainDataCl->getLightmapData();

    //3 floats per point for the normals of the terrain
    m_normalData = m_terrainDataCl->getNormalData();

	for(unsigned int i = 0; i < m_terrainDataCl->getTerrainLength(); ++i)
    {
        m_lightingData[i] = 0;
    }

    for (unsigned int i = 0; i < m_terrainDataCl->getTerrainLength() * 3; ++i)
    {
        m_normalData[i] = 0;
    }

    m_lightmapDirty = true;

    m_patchSize = verticesPerPatch;
    m_numPatches = m_terrainDataCl->getTerrainLength() / verticesPerPatch;
    m_patches = new TerrainPatch*[m_numPatches * m_numPatches];

    for (unsigned int zPatch = 0; zPatch < m_numPatches; ++zPatch)
    {

        for (unsigned int xPatch = 0; xPatch < m_numPatches; ++xPatch)
        {

            unsigned int zRowStart = (zPatch * m_patchSize);
            unsigned int xRowStart = (xPatch * m_patchSize);

            float* patchHeights = new float[m_patchSize * m_patchSize];
            float* patchUV = new float[m_patchSize * m_patchSize * 2];

            for (unsigned int z = 0; z < m_patchSize; ++z)
            {

                unsigned int zMulValue = zRowStart + z;

                for (unsigned int x = 0; x < m_patchSize; ++x)
                {

                    patchHeights[(z * m_patchSize) + x] = m_terrainDataCl->getTerrainData()[(zMulValue * m_terrainDataCl->getTerrainLength()) + xRowStart + x];
                    patchUV[((z * m_patchSize) + x) * 2] = m_terrainDataCl->getUVData()[ ((zMulValue * m_terrainDataCl->getTerrainLength()) + xRowStart + x) * 2];
                    patchUV[(((z * m_patchSize) + x) * 2) + 1] = m_terrainDataCl->getUVData()[ (((zMulValue * m_terrainDataCl->getTerrainLength()) + xRowStart + x) * 2) + 1];
                }

            }


            m_patches[(zPatch * m_numPatches) + xPatch] = new TerrainPatch(patchHeights, patchUV, m_patchSize);

            delete[] patchHeights;
            delete[] patchUV;
        }

    }


    File* shdFile = (File*) Filesystem::evaluateChild(Filesystem::getRoot(), "Data/Shaders/Terrain.shader");
    m_terrainShader = GRenderer::getRenderer()->getGraphicsAPI()->getShaderFactory()->loadShader(shdFile);

    if (m_terrainShader != 0)
    {
        m_terrainShader->setNumberOfTextures(2);
        m_terrainShader->setTextureName(0, "baseTexture");
        m_terrainShader->setTextureName(1, "lightMap");
    }

    scaleFactorX = 100;
    scaleFactorY = 100;
    scaleFactorZ = 100;

    m_ambientLight = 1.0f;
}

TerrainNode::~TerrainNode()
{

    delete m_terrainDataCl;
    delete m_terrainLightMap;

    GRenderer::getRenderer()->getTextureMonitor()->releaseTexture(m_terrainTextureData);

}

void TerrainNode::terrainAddVector(float* v, float* o)
{
    v[0] += o[0];
    v[1] += o[1];
    v[2] += o[2];
}

void TerrainNode::terrainNormalize(float *v) {

	double d;

	d = sqrt((v[0]*v[0]) + (v[1]*v[1]) + (v[2]*v[2]));

	v[0] = v[0] / d;
	v[1] = v[1] / d;
	v[2] = v[2] / d;
}

void TerrainNode::terrainCrossProduct(unsigned int x1, unsigned int z1, unsigned int x2, unsigned int z2, unsigned int x3, unsigned int z3, float* auxNormal)
{
    unsigned int l_terrainLength = m_terrainDataCl->getTerrainLength();

    float* m_terrainData = m_terrainDataCl->getTerrainData();

    float v1[3],v2[3];

	v1[0] = (x2-x1);
	v1[1] = -m_terrainData[z1 * l_terrainLength + x1]
			+ m_terrainData[z2 * l_terrainLength + x2];
	v1[2] = (z2-z1);


	v2[0] = (x3-x1);
	v2[1] = -m_terrainData[z1 * l_terrainLength + x1]
			+ m_terrainData[z3 * l_terrainLength + x3];
	v2[2] = (z3-z1);

	auxNormal[2] = v1[0] * v2[1] - v1[1] * v2[0];
	auxNormal[0] = v1[1] * v2[2] - v1[2] * v2[1];
	auxNormal[1] = v1[2] * v2[0] - v1[0] * v2[2];

    return;
}


float TerrainNode::getBrightnessAtPoint(unsigned int x, unsigned int z)
{

    return m_lightingData[(z * m_terrainDataCl->getTerrainLength()) + x];

}


void TerrainNode::update(float seconds)
{
}

void TerrainNode::recalculateLightMaps()
{
    unsigned int l_terrainLength = m_terrainDataCl->getTerrainLength();

    unsigned int light_type = slopeLighting;
    float softness = 5.0f;
    float max_Light = 0.95f;

    Vector3f lightVector;

    lightVector.setX(20);
    lightVector.setY(0);
    lightVector.setZ(0);

    float* m_terrainData = m_terrainDataCl->getTerrainData();

    //If its a height based light map just cycle through setting the colours to the value of the lightmap
    if (light_type == heightBasedLightmap)
    {

        float maxHeight = -10000.0f;
        float minHeight = 10000.0f;

        for (unsigned int i = 0; i < l_terrainLength * l_terrainLength; i++)
        {
            if (m_terrainData[i] > maxHeight)
            {
                maxHeight = m_terrainData[i];
            }

            if (m_terrainData[i] < minHeight)
            {
                minHeight = m_terrainData[i];
            }
        }

        float range = maxHeight - minHeight;
        printf("Range: %f\n", range);

        for (unsigned int x = 0; x < l_terrainLength; ++x)
        {

            for (unsigned int z = 0; z < l_terrainLength; ++z)
            {


                float value = m_terrainData[(z * l_terrainLength) + x];
                float result = value + range;
                result = result / range;

                m_lightingData[(z * l_terrainLength) + x] = result;

            }

        }

    }
    else if (light_type == slopeLighting)
    {

        lightVector = lightVector.normalize();

        for (unsigned int x = 0; x < l_terrainLength; ++x)
        {

            for (unsigned int z = 0; z < l_terrainLength; ++z)
            {
                float tmp_ShadeValue = 0.0f;

                //This double checks that the value is within the boundries of the array
                if (z >= lightVector.getZ() && x >= lightVector.getX())
                {
                    tmp_ShadeValue = 1.0f - ( m_terrainData[(unsigned int) (((z - lightVector.getZ()) * l_terrainLength) + (x - lightVector.getX()))] - m_terrainData[(unsigned int) ((z * l_terrainLength) + x)]) / softness;
                }
                else
                {

                    //If its not then just make is very bright
                    tmp_ShadeValue = 1.0f;

                }

                if (tmp_ShadeValue > max_Light)
                {
                    tmp_ShadeValue = max_Light;
                }

                m_lightingData[(z * l_terrainLength) + x] = (m_ambientLight * tmp_ShadeValue);
            }

        }
    }
    else if (light_type == positionalLighting)
    {

        for (unsigned int x = 0; x < l_terrainLength; ++x)
        {

            for (unsigned int z = 0; z < l_terrainLength; ++z)
            {

                Vector3f vectorBetweenPointAndLight;

/*
                vectorBetweenPointAndLight.x = lightVector.x - (x + m_position.x);
                vectorBetweenPointAndLight.y = m_terrainData[(z * m_terrainLength) + x];
                vectorBetweenPointAndLight.z = lightVector.z - (z + m_position.z);
*/
                vectorBetweenPointAndLight = vectorBetweenPointAndLight.normalize();


                m_lightingData[(z * l_terrainLength) + x] = m_ambientLight;

            }

        }

    }


    //the -1 causes a little strech buut ensures the texture is a pow two (Terrain = pow2 + 1)

    printf("Terrain: Building texture from data\n");
    m_terrainLightMap->loadTextureData(input_data_float_luminance, l_terrainLength - 1, l_terrainLength - 1, true, m_lightingData);
    printf("Terrain: Done\n");



}

void TerrainNode::draw(Camera* cam)
{

    ThreeDRenderLayer* ly = (ThreeDRenderLayer*) GRenderer::getRenderer()->getLayer(Layer_3D);

    if (m_lightmapDirty)
    {
        recalculateLightMaps();
        m_lightmapDirty = false;

        printf("Terrain: Recalculated lightmap\n");
    }

    GRenderer::getRenderer()->getGraphicsAPI()->pushMatrix();

        GRenderer::getRenderer()->getGraphicsAPI()->translateMatrix(m_position.getX(), m_position.getY(), m_position.getZ());
        GRenderer::getRenderer()->getGraphicsAPI()->rotateMatrix(m_rotation.getX(), m_rotation.getY(), m_rotation.getZ());

        if (m_terrainShader != 0)
        {

            m_terrainShader->setTexture(0, m_terrainTextureData);
            m_terrainShader->setTexture(1, m_terrainLightMap);

            m_terrainShader->activateShader();
        }

        float changeDistance = 150 * ((scaleFactorX + scaleFactorZ) / 2.0f);

        float cameraCurrentX = ly->getActiveCamera()->getPosition().getX();
        float cameraCurrentY = ly->getActiveCamera()->getPosition().getY();
        float cameraCurrentZ = ly->getActiveCamera()->getPosition().getZ();

        glScalef(scaleFactorX, scaleFactorY, scaleFactorZ);

        for (unsigned int zPatch = 0; zPatch < m_numPatches; ++zPatch)
        {

            for (unsigned int xPatch = 0; xPatch < m_numPatches; ++xPatch)
            {

                GRenderer::getRenderer()->getGraphicsAPI()->pushMatrix();
                    GRenderer::getRenderer()->getGraphicsAPI()->translateMatrix(xPatch * m_patchSize, 0, zPatch * m_patchSize);

                unsigned int patchCenterX = ((xPatch * m_patchSize) + m_patchSize / 2) * scaleFactorX;
                unsigned int patchCenterY = 0;
                unsigned int patchCenterZ = ((zPatch * m_patchSize) + m_patchSize / 2) * scaleFactorZ;

                float xDist = (patchCenterX + cameraCurrentX);
                float yDist = (patchCenterY + cameraCurrentY);
                float zDist = (patchCenterZ + cameraCurrentZ);

                float distanceFromCameraSquared = powf(xDist, 2) + powf(yDist, 2) + powf(zDist, 2);

                float d = sqrtf(distanceFromCameraSquared);

                unsigned int distanceOverChangeD = d / changeDistance;

                m_patches[(zPatch * m_numPatches) + xPatch]->draw(distanceOverChangeD);


                GRenderer::getRenderer()->getGraphicsAPI()->popMatrix();

            }

        }

        glScalef( 1.0f / scaleFactorX, 1.0f / scaleFactorY, 1.0f / scaleFactorZ);

        if (m_terrainShader != 0)
        {

            m_terrainShader->deactivateShader();

        }

    GRenderer::getRenderer()->getGraphicsAPI()->popMatrix();

}
