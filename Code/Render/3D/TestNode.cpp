#include "TestNode.h"
#include "../GRender.h"
#include <GL/gl.h>
#include <math.h>

static float Verts[36] =
{
    0, 1, 0,
    -1, -1, 1,
    1, -1, 1,

    0, 1, 0,
    1, -1, 1,
    1, -1, -1,

    0, 1, 0,
    1, -1, -1,
    -1, -1, -1,

    0, 1, 0,
    -1, -1, -1,
    -1, -1, 1
};

static float TexCoords[24] =
{
    0, 0,
    0, 1,
    1, 1,

    0, 0,
    0, 1,
    1, 1,

    0, 0,
    0, 1,
    1, 1,

    0, 0,
    0, 1,
    1, 1,
};

static float Normals[36] =
{
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,

    4, 2, 0,
    4, 2, 0,
    4, 2, 0,

    0, 2, -4,
    0, 2, -4,
    0, 2, -4,

    -4, 0, 4,
    -4, 0, 4,
    -4, 0, 4
};


TestNode::TestNode()
{
    nodeVertexSet = GRenderer::getRenderer()->getGraphicsAPI()->newVertexSet();
    nodeVertexSet->setVertices(Verts, 12);
    nodeVertexSet->setTextureCoordinates(TexCoords, 12);
    nodeVertexSet->setNormals(Normals, 12);

    nodeTexture = GRenderer::getRenderer()->getTextureMonitor()->getTexture("Data/backgrounds/background_2.tga");

    //TODO: Check before typecast
    File* shdFile = (File*) Filesystem::evaluateChild(Filesystem::getRoot(), "Data/Shaders/Test.shader");
    nodeShader = GRenderer::getRenderer()->getGraphicsAPI()->getShaderFactory()->loadShader(shdFile);

    printf("Compiling shader\n");

    if (nodeShader != 0)
    {

        nodeShader->setNumberOfTextures(1);
        nodeShader->setTextureName(0, "texture");

    }

    nodeName = 0;

}

TestNode::~TestNode()
{

    if (nodeName != 0)
    {
        delete[] nodeName;
        nodeName = 0;
    }

    GRenderer::getRenderer()->getTextureMonitor()->releaseTexture(nodeTexture);
    delete nodeShader;
}

float LightAmbient[]= { 0.7f, 0.7f, 0.7f, 1.0f }; 				// Ambient Light Values ( NEW )
float LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };				 // Diffuse Light Values ( NEW )
float LightPosition[]= { 0.0f, 20.0f, 0.0f, 1.0f };				 // Light Position ( NEW )


void TestNode::draw(Camera* cam)
{

    if (nodeShader != 0)
    {

        nodeShader->activateShader();
        nodeShader->setTexture(0, nodeTexture);

    }

    GRenderer::getRenderer()->getGraphicsAPI()->pushMatrix();

    glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);				// Setup The Ambient Light
    glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);				// Setup The Diffuse Light
    glLightfv(GL_LIGHT0, GL_POSITION,LightPosition);			// Position The Light

    GRenderer::getRenderer()->getGraphicsAPI()->translateMatrix(m_position.getX(), m_position.getY(), m_position.getZ());
    GRenderer::getRenderer()->getGraphicsAPI()->rotateMatrix(m_rotation.getX(), m_rotation.getY(), m_rotation.getZ());

    glScalef(10.0f, 10.0f, 10.0f);

        nodeVertexSet->Draw();

    glScalef(0.1f, 0.1f, 0.1f);

    GRenderer::getRenderer()->getGraphicsAPI()->popMatrix();

    if (nodeShader != 0)
        nodeShader->deactivateShader();

}

void TestNode::update(float seconds)
{
 //   m_rotation.setZ(m_rotation.getZ() + (seconds * 10));
}

static float vectorLength(float* vector)
{
    return sqrtf((vector[0] * vector[0]) + (vector[1] * vector[1]) * (vector[2] * vector[2]));
}

static void vectorNormalize(float* vector)
{
    float length = vectorLength(vector);
    if (length == 0) length = 1;

    vector[0] = vector[0] / length;
    vector[1] = vector[1] / length;
    vector[2] = vector[2] / length;
}

static void vectorBetween(float* start, float* other, float* result)
{
    result[0] = other[0] - start[0];
    result[1] = other[1] - start[1];
    result[2] = other[2] - start[2];
}

static void vectorCross(float* first, float* other, float* result)
{

    result[0] = (first[1] * other[2]) - (first[2] * other[1]);
    result[1] = (first[2] * other[0]) - (first[0] * other[2]);
    result[2] = (first[0] * other[1]) - (first[1] * other[0]);

}

static void vectorCopy(float* out, unsigned int outStart, float* in, unsigned int inStart)
{
    out[outStart] = in[inStart];
    out[outStart + 1] = in[inStart + 1];
    out[outStart + 2] = in[inStart + 2];
}

static void vectorAdd(float* out, unsigned int start, float* in)
{
    out[start] += in[0];
    out[start + 1] += in[1];
    out[start + 2] += in[2];
}

const unsigned int mainChunk = 0x4d4d;
const unsigned int editorChunk = 0x3d3d;
const unsigned int editObjectChunk = 0x4000;
const unsigned int objectTriangleMesh = 0x4100;
const unsigned int objectVertexList = 0x4110;
const unsigned int objectFaceList = 0x4120;
const unsigned int objectUVList = 0x4140;


bool TestNode::load(const char* Filename)
{
    FILE* fin = fopen(Filename, "rb");

    if (fin == 0)
    {
        printf("Could not open %s for reading in binary mode\n", Filename);
        return false;
    }

    bool result = true;

    float* vertices = 0;
    float* normals = 0;
    unsigned int numVertices = 0;

    float* uvCoordinates = 0;
    unsigned int numUVCoordinates = 0;

    unsigned int* indices = 0;
    unsigned int numPolygons = 0;

    unsigned short chunkID = 0;
    unsigned int chunkLength = 0;

    unsigned char tempChar = 0;

    unsigned short faceFlags = 0;

    while (true)
    {

        fread(&chunkID, 2, 1, fin);
        fread(&chunkLength, 4, 1, fin);

        if (feof(fin) || ferror(fin))
        {
            printf("End of file or read error\n");
            break;
        }

        switch (chunkID)
        {
            case mainChunk:
            {
                printf("Main chunk, ID: 0x%x Length 0x%x\n", chunkID, chunkLength);
                break;
            };

            case editorChunk:
            {
                printf("Editor chunk, ID: 0x%x Length 0x%x\n", chunkID, chunkLength);
                break;
            };

            case editObjectChunk:
            {

                printf("Edit object chunk, ID: 0x%x Length 0x%x\n", chunkID, chunkLength);

                //Store the start of the name so we can seek back to it
                unsigned long currentPosition = ftell(fin);
                unsigned int nameLength = 0;

                while (true)
                {
                    fread(&tempChar, 1, 1, fin);

                    if (feof(fin) || ferror(fin))
                    {
                        //Shouldn't end here, Flag a error and run
                        result = false;
                        break;
                    }

                    if (tempChar == '\0')
                        break;

                    nameLength++;
                }

                //Break out of the switch if result is bad
                if (result == false) break;

                //Otherwise allocate space for  the name, seek to the start and read it again

                printf("Name length %i\n", nameLength);

                //Add one byte for a null terminator
                char* nameData = new char[nameLength + 1];

                fseek(fin, currentPosition, SEEK_SET);
                fread(nameData, 1, nameLength + 1, fin);

                printf("Read name %s\n", nameData);

                nodeName = nameData;

                break;
            };

            case objectTriangleMesh:
            {
                printf("Triangle mesh node ID: %i length %i\n", chunkID, chunkLength);
                break;
            };

            case objectVertexList:
            {
                printf("Vertex list ID %i length %i\n", chunkID, chunkLength);

                numVertices = 0;
                fread(&numVertices, 2, 1, fin);

                printf("Num vertices %i\n", numVertices);

                vertices = new float[numVertices * 3];
                memset(vertices, 0, sizeof(float) * numVertices * 3);

                if (fread(vertices, sizeof(float), numVertices * 3, fin) != numVertices * 3)
                {
                    printf("Unable to read vertices\n");
                    result = false;
                    break;
                }

                break;
            };

            case objectFaceList:
            {
                printf("Face list ID: %i length %i\n", chunkID, chunkLength);

                numPolygons= 0;
                fread(&numPolygons, 2, 1, fin);

                printf("Num indices %i\n", numPolygons * 3);

                indices = new unsigned int[numPolygons * 3];
                memset(indices, 0, sizeof(int) * numPolygons * 3);

                short tempIndices[3];
                unsigned int cIndPtr = 0;


                for (unsigned int i = 0; i < numPolygons; i++)
                {
                    memset(tempIndices, 0, sizeof(short) * 3);

                    if (fread(tempIndices, 2, 3, fin) != 3)
                    {
                        result = false;
                        break;
                    }

                    if (fread(&faceFlags, 2, 1, fin) != 1)
                    {
                        result = false;
                        break;
                    }

                    indices[cIndPtr] = tempIndices[0];
                    cIndPtr++;
                    indices[cIndPtr] = tempIndices[1];
                    cIndPtr++;
                    indices[cIndPtr] = tempIndices[2];
                    cIndPtr++;
                }

                break;
            };

            case objectUVList:
            {
                printf("UV list ID: %i length %i\n", chunkID, chunkLength);

                fread(&numUVCoordinates, 2, 1, fin);

                uvCoordinates = new float[numUVCoordinates * 2];

                if (fread(uvCoordinates, sizeof(float), numUVCoordinates * 2, fin) != numUVCoordinates * 2)
                {
                    printf("Unable to read UV coordinates\n");
                    result = false;
                }

                break;
            };


            default:
            {
                printf("Unhandled chunk ID: 0x%x Length: 0x%x skipping\n", chunkID, chunkLength);

                //Skip length - 6 (6 is the size of the header) bytes
                fseek(fin, chunkLength - 6, SEEK_CUR);

                break;
            };

        };

        if (result == false)
        {
            //If result is false, then a error occured in the switch statement so bottle out
            break;
        }
    }

    /* COMPUTE THE NORMALS */
    if (vertices != 0)
    {
        normals = new float[numVertices * 3];
        memset(normals, 0, sizeof(float) * numVertices * 3);
        unsigned int* numConnections = new unsigned int[numVertices];

        for (unsigned int i = 0; i < numVertices; ++i)
        {
            numConnections[i] = 0;
        }

        for (unsigned int i = 0; i < numVertices; ++i)
        {
            normals[i] = 0;
        }

        float vec1[3];
        float vec2[3];
        float vec3[3];

        float vecb1[3];
        float vecb2[3];

        float calculatedNormal[3];

        for (unsigned int i = 0; i < numPolygons * 3; i += 3)
        {
            unsigned int vertexOne = indices[i];
            unsigned int vertexTwo = indices[i + 1];
            unsigned int vertexThree = indices[i + 2];

            //printf("0x%x 0x%x 0x%x\n", vectorOnePointer, vectorTwoPointer, vectorThreePointer);
            vectorCopy(vec1, 0, vertices, vertexOne * 3);
            vectorCopy(vec2, 0, vertices, vertexTwo * 3);
            vectorCopy(vec3, 0, vertices, vertexThree * 3);

            //Get the vector from vec1 to vec2 (Vec2 - Vec1)
            vectorBetween(vec1, vec2, vecb1);

            //Get the vector from vec1 to vec3 (Vec3 - Vec1)
            vectorBetween(vec1, vec3, vecb2);

            //Normalize the results
            vectorNormalize(vecb2);
            vectorNormalize(vecb1);

            //Get the cross product of the two vectors
            vectorCross(vecb1, vecb2, calculatedNormal);

            //Normalize it
            vectorNormalize(calculatedNormal);

            numConnections[ vertexOne ] += 1;
            numConnections[ vertexTwo ] += 1;
            numConnections[ vertexThree ] += 1;

            //printf("between %f %f %f\n%f %f %f\nnormal %f %f %f\n", vecb1[0], vecb1[1], vecb1[2], vecb2[0], vecb2[1], vecb2[2], normal[0], normal[1], normal[2]);

            //Add it to each vertex normal shared by this poly
            vectorAdd(normals, vertexOne * 3, calculatedNormal);
            vectorAdd(normals, vertexTwo * 3, calculatedNormal);
            vectorAdd(normals, vertexThree * 3, calculatedNormal);
        }

        for (unsigned int i = 0; i < numVertices; ++i)
        {

            if (numConnections[i] > 0)
            {
                normals[(i * 3) ] = normals[( i * 3)] / numConnections[i];
                normals[(i * 3) + 1] = normals[( i * 3) + 1] / numConnections[i];
                normals[(i * 3) + 2] = normals[( i * 3) + 2] / numConnections[i];
            }

        }

        delete[] numConnections;
    }

    if (result == true)
    {
        nodeVertexSet->setIndices(indices, numPolygons * 3);
        nodeVertexSet->setVertices(vertices, numVertices);
        nodeVertexSet->setNormals(normals, numVertices);
        nodeVertexSet->setTextureCoordinates(uvCoordinates, numUVCoordinates);
    }

    delete[] indices;
    delete[] normals;
    delete[] vertices;
    delete[] uvCoordinates;

    fclose(fin);
    return result;
}
