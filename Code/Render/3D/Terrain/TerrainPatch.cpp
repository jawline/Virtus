#include "TerrainPatch.h"
#include "../../GRender.h"

VertexSet* TerrainPatch::generateDrawLOD(float* heightData, float* UV, unsigned int patchSize, unsigned int stride, unsigned int* lodPatchSize)
{

    unsigned int numPointsPerRow = ((patchSize - 1) / stride) + 1;

    //4 because there are 4 vertices per point
    float* verticesTemp = new float[(numPointsPerRow - 1) * numPointsPerRow * 2 * 3];
    float* tempUVs = new float[(numPointsPerRow - 1) * numPointsPerRow * 2 * 2];

    for (unsigned int i = 0; i < (numPointsPerRow - 1) * numPointsPerRow * 2 * 3; ++i)
    {
        verticesTemp[i] = 0;
    }

    unsigned int nextVertexStart = 0;
    unsigned int nextUVStart = 0;

    for (unsigned int z = 0; z < patchSize - 1; z += stride)
    {

        for (unsigned int x = 0; x < patchSize; x += stride)
        {

            unsigned int uvDataStartPos = ((z * patchSize) + x) * 2;
            unsigned int heightDataStart = ( ( z * patchSize) + x);

            verticesTemp[nextVertexStart] = x;
            verticesTemp[nextVertexStart + 1] = heightData[heightDataStart];
            verticesTemp[nextVertexStart + 2] = z;

            tempUVs[nextUVStart] = UV[uvDataStartPos];
            tempUVs[nextUVStart + 1] = UV[uvDataStartPos + 1];

            nextUVStart += 2;

            nextVertexStart += 3;

            heightDataStart = ( ( (z + stride) * patchSize) + x);

            verticesTemp[nextVertexStart] = x;
            verticesTemp[nextVertexStart + 1] = heightData[heightDataStart];
            verticesTemp[nextVertexStart + 2] = z + stride;

            nextVertexStart += 3;

            uvDataStartPos = (((z + stride) * patchSize) + x) * 2;
            tempUVs[nextUVStart] = UV[uvDataStartPos];
            tempUVs[nextUVStart + 1] = UV[uvDataStartPos + 1];

            nextUVStart += 2;

        }

    }


    VertexSet* returnSet = GRenderer::getRenderer()->getGraphicsAPI()->newVertexSet();

    returnSet->setVertices(verticesTemp, (patchSize - 1) * patchSize * 2);
    returnSet->setTextureCoordinates(tempUVs, (patchSize - 1) * patchSize * 2);

    delete[] verticesTemp;
    delete[] tempUVs;

    *lodPatchSize = numPointsPerRow;

    return returnSet;
}

TerrainPatch::TerrainPatch(float* heightData, float* UV, unsigned int patchSize)
{
    printf("Allocating patch VSet\n");

    m_numLods = 0;

    unsigned int stride = 1;

    while (1)
    {
        m_numLods++;

        stride = stride * 2;
        if (stride == (patchSize - 1)) break;

    }

    printf("Num LODs %i\n", m_numLods);

    m_patchVSetLods = new VertexSet*[m_numLods];
    m_lodNumVs = new unsigned int[m_numLods];

    stride = 1;

    for (unsigned int i = 0; i < m_numLods; ++i)
    {
        m_patchVSetLods[i] = generateDrawLOD(heightData, UV, patchSize, stride, &m_lodNumVs[i]);
        stride = stride * 2;
    }

/*
    m_patchSize = patchSize;

    unsigned int newPatchSize = 0;
    m_patchVSet = generateDrawLOD(heightData, UV, patchSize, 32, &newPatchSize);
    m_patchSize = newPatchSize; */
}

TerrainPatch::~TerrainPatch()
{
    //delete m_patchVSet;
    for (unsigned int i = 0; i < m_numLods; ++i)
    {
        delete m_patchVSetLods[i];
    }

    delete[] m_patchVSetLods;
    delete[] m_lodNumVs;
}

void TerrainPatch::draw(unsigned int lod)
{
    if (lod >= m_numLods) lod = m_numLods - 1;
    if (lod < m_numLods)
    {
        VertexSet* vSet = m_patchVSetLods[lod];
        unsigned int rInc = m_lodNumVs[lod];
        unsigned int cStart = 0;

        for (unsigned int i = 0; i < rInc - 1; ++i)
        {
            vSet->Draw(cStart, rInc * 2, TRIANGLE_STRIP);
            cStart += rInc * 2;
        }
    }

    /*
    unsigned int cStart = 0;

    for (unsigned int i = 0; i < m_patchSize - 1; ++i)
    {
        m_patchVSet->Draw(cStart, m_patchSize * 2, TRIANGLE_STRIP);
        cStart += m_patchSize * 2;
    } */
}
