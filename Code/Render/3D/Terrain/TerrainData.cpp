#include "TerrainData.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

TerrainData::TerrainData(unsigned int l_terrainLength)
{

    printf("TerrainData patch width %i height %i made\n", l_terrainLength, l_terrainLength);

    //Set the terrains widths and heights in the class
    m_terrainLength = l_terrainLength;

    //Allocate memory for the terrain data
    m_terrainData = new float[m_terrainLength * m_terrainLength];

    //Allocate memory for the UV data
    m_uvData = new float[m_terrainLength * m_terrainLength * 2];

    //Allocate memory for lighting data
    m_lightmapData = new float[m_terrainLength * m_terrainLength];

    //Allocate memory for the normal data
    m_normalData = new float[m_terrainLength * m_terrainLength * 3];

    //Reset the terrain data to 0's
    for (unsigned int l_dataIterator = 0; l_dataIterator < m_terrainLength * m_terrainLength; ++l_dataIterator)
    {
        m_terrainData[l_dataIterator] = 0;
    }

    //Reset the UV data to 0
    for (unsigned int l_dataIterator = 0; l_dataIterator < m_terrainLength * m_terrainLength; ++l_dataIterator)
    {
        m_uvData[l_dataIterator] = 0;
    }

    //Reset the lighting data to 0
    for (unsigned int l_dataIterator = 0; l_dataIterator < m_terrainLength * m_terrainLength; ++l_dataIterator)
    {
        m_lightmapData[l_dataIterator] = 0;
    }

    //Reset the normal data to 0
    for (unsigned int l_dataIterator = 0; l_dataIterator < m_terrainLength * m_terrainLength; ++l_dataIterator)
    {
        m_normalData[l_dataIterator] = 0;
    }

    recalculateUV();
}

TerrainData::~TerrainData()
{

    //Free the terrain data
    delete[] m_terrainData;

    //Free the UV data
    delete[] m_uvData;

    //Free the lightmap data
    delete[] m_lightmapData;

    //Free the normal data
    delete[] m_normalData;
}

float* TerrainData::getTerrainData()
{
    //Return the terrain height data
    return m_terrainData;
}


float* TerrainData::getUVData()
{
    //Return the UV data
    return m_uvData;
}

float* TerrainData::getLightmapData()
{
    //Return lightmap data
    return m_lightmapData;
}


float* TerrainData::getNormalData()
{
    //Return normal data
    return m_normalData;
}

void TerrainData::generateUsingSineFaultAlgorythm(unsigned int l_numIterations, float l_distortionStart, float l_distortionMin, float l_sineCurveMagnitude)
{

    //Used in several of the functions so might as well calculate it here
    float l_halfLength = m_terrainLength / 2;
    float l_halfHeight = m_terrainLength / 2;

    //Perform the iterations
    for (unsigned int l_currentIteration = 0; l_currentIteration < l_numIterations; l_currentIteration++)
    {

        //The displacement value
        float l_displacementValue = 0;


        //The displacement decreases every iteration twards d_min

        //If the current iteration is the last iteration
        if (l_currentIteration == l_numIterations)
        {
            l_displacementValue = l_distortionMin;
        }
        else
        {
            float currentIter = ((float) l_currentIteration) / l_numIterations;
            l_displacementValue = l_distortionStart + currentIter * ( l_distortionStart - l_distortionMin);
        }

        float a, b, c, w, d;

        //Generate the line to displace across
        d = sqrt((l_halfLength * l_halfLength) + (l_halfHeight * l_halfHeight));

        w = rand();

        a = cos(w);
        b = sin(w);

        c = ((float)rand() / RAND_MAX) * 2*d  - d;

        //For each point in the terrain
        for (unsigned int z = 0; z < m_terrainLength; ++z)
        {

            for (unsigned int x = 0; x < m_terrainLength; ++x)
            {

                //Calculate the PD and divide it by the length of the sine slope
                float pd = ( ((z - l_halfHeight) * a) + (( x - l_halfLength) * b) + c) / l_sineCurveMagnitude;

                //Set max and min values
                if (pd > 1.57) pd = 1.57;
                else if (pd < 0) pd = 0;

                //Apply it to the terrain data
                m_terrainData[(z * m_terrainLength) + x] += (- l_displacementValue / 2) + (sin(pd) * l_displacementValue);

            }

        }

    }


    float lowestPoint = 100000.0f;

    for (unsigned int i = 0; i < m_terrainLength * m_terrainLength; ++i)
    {
        if (m_terrainData[i] < lowestPoint)
        {
            lowestPoint =m_terrainData[i];
        }
    }

    float addValue = (-lowestPoint);

    for (unsigned int i = 0; i < m_terrainLength * m_terrainLength; ++i)
    {
        m_terrainData[i] += addValue;
    }

}

void TerrainData::recalculateUV()
{

	unsigned int incrementCount, x, z, tuCount, tvCount;
	float incrementValue, tuCoordinate, tvCoordinate;

    //How much to increment by each count
    incrementValue = 1.0f / m_terrainLength;

    //How many times to increment
    incrementCount = m_terrainLength;

    //Set a default values for tu and tv
    tuCoordinate = 0;
    tvCoordinate = 0;

    //Reset counters
    tuCount = 0;
    tvCount = 0;

    for (z = 0; z < m_terrainLength; z++)
    {

        for (x = 0; x < m_terrainLength; x++)
        {

            m_uvData[((m_terrainLength * z) + x) * 2] = tuCoordinate;
            m_uvData[(((m_terrainLength * z) + x) * 2) + 1] = tvCoordinate;

            //increment the tu
            tuCoordinate += incrementValue;

            //Increment the count
            tuCount++;

            //If tuCount == incrementCount
            if (tuCount == incrementCount)
            {
                //Reset them both
                tuCoordinate = 0;
                tuCount = 0;
            }

        }

        // Decement the tvCoordinate by the increment value
		tvCoordinate += incrementValue;

		//Increment the tvCount
		tvCount++;

		// Check if at the top of the texture and if so then start at the bottom again.
		if(tvCount == incrementCount)
		{
			tvCoordinate = 0.0f;
			tvCount = 0;
		}

    }
}
