#include "VertexSet.h"
#include <GL/glew.h>
#include <stdio.h>

GL2VertexSet::GL2VertexSet()
{

    m_numVertices = m_numNormals = m_numTexCoords = 0;
    m_vertices = m_texCoords = m_normals = 0;

    m_vertexSetVBO = 0;

    m_numIndices = 0;
    m_indices = 0;
    m_colourVals = 0;
    m_numColourValues = 0;
    m_colourValuesType = ctype_none;

    glGenBuffers(1, &m_vertexSetVBO);
}

GL2VertexSet::~GL2VertexSet()
{

    if (m_vertices != 0)
    {
        delete[] m_vertices;
    }

    if (m_normals != 0)
    {
        delete[] m_normals;
    }

    if (m_texCoords != 0)
    {
        delete[] m_texCoords;
    }

    if (m_indices != 0)
    {
        delete[] m_indices;
    }

    if (m_colourVals != 0)
    {
        delete[] m_colourVals;
    }
}

void GL2VertexSet::setColourValues(float* cvals, enum ColourValueTypes type, unsigned int points)
{
    if (m_colourVals != 0) delete[] m_colourVals;

    unsigned int arrSize = 0;

    if (type == ctype_float_rgb)
    {
            arrSize = points * 3;
    }
    else if (type == ctype_float_rgba)
    {
            arrSize = points * 4;
    }
    else
    {
        m_colourVals = 0;
        m_colourValuesType = ctype_none;
        m_numColourValues = 0;
        return;
    }

    m_colourVals = new GLfloat[arrSize];
    m_colourValuesType = type;
    m_numColourValues = points;

    for (unsigned int i = 0; i < arrSize; ++i)
    {
        m_colourVals[i] = cvals[i];
    }

    return;
}

void GL2VertexSet::setIndices(unsigned int* indices, unsigned int number)
{
    if (m_indices != 0) delete[] m_vertices;

    m_indices = new GLuint[number];

    for (unsigned int i = 0; i < number; ++i)
    {
        m_indices[i] = indices[i];
    }

    m_numIndices = number;
}

void GL2VertexSet::setVertices(float* vertices, unsigned int number)
{
    if (m_vertices != 0) delete[] m_vertices;

    m_vertices = new GLfloat[number * 3];

    for (unsigned int i = 0; i < number * 3; ++i)
    {
        m_vertices[i] = vertices[i];
    }

    m_numVertices = number;
}

void GL2VertexSet::setNormals(float* normals, unsigned int number)
{
    if (m_normals != 0) delete[] m_normals;

    m_normals = new GLfloat[number * 3];

    for (unsigned int i = 0; i < number * 3; ++i)
    {
        m_normals[i] = normals[i];
    }

    m_numNormals = number;
}

void GL2VertexSet::setTextureCoordinates(float* tcoords, unsigned int number)
{
    if (m_texCoords != 0) delete[] m_texCoords;

    m_texCoords = new GLfloat[number * 2];

    for (unsigned int i = 0; i < number * 2; ++i)
    {
        m_texCoords[i] = tcoords[i];
    }

    m_numTexCoords = number;
}

void GL2VertexSet::Draw()
{
    if (m_vertices == 0) return;

    if (m_indices != 0)
    {
        Draw(0, m_numIndices, TRIANGLES);
    }
    else
    {
        Draw(0, m_numVertices, TRIANGLES);
    }
}

void GL2VertexSet::Draw(unsigned int start, unsigned int number, PrimitiveTypes primitiveType)
{
    if (m_vertices == 0) return;

    GLenum apiPrimType = 0;

    switch (primitiveType)
    {
        case QUADS:
            apiPrimType = GL_QUADS;
            break;

        case QUAD_STRIP:
            apiPrimType = GL_QUAD_STRIP;
            break;

        case TRIANGLES:
            apiPrimType = GL_TRIANGLES;
            break;

        case TRIANGLE_STRIP:
            apiPrimType = GL_TRIANGLE_STRIP;
            break;

        case LINES:
            apiPrimType = GL_LINES;
            break;

        case LINE_STRIP:
            apiPrimType = GL_LINE_STRIP;
            break;

        case POINTS:
            apiPrimType = GL_POINTS;
            break;
    }

    if (m_vertices != 0)
    {
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, m_vertices);
    }

    if (m_texCoords != 0)
    {

        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, 0, m_texCoords);
    }

    if (m_normals != 0)
    {
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_FLOAT, 0, m_normals);
    }

    if (m_colourVals != 0)
    {
        glEnableClientState(GL_COLOR_ARRAY);

        if (m_colourValuesType == ctype_float_rgb)

            glColorPointer(3, GL_FLOAT, 0, m_colourVals);
        else
            glColorPointer(4, GL_FLOAT, 0, m_colourVals);
    }


    if (m_indices != 0)
    {
       glDrawRangeElements(apiPrimType, start, start + number, number, GL_UNSIGNED_INT, m_indices);
    }
    else
    {
        glDrawArrays(apiPrimType, start, number);
    }

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
}
/*
void VertexSet::updateBuffers() {

    glBindBuffer(GL_ARRAY_BUFFER, &m_vertexSetVBO);

    if (m_vertices != 0) {
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void VertexSet::updateBuffers(unsigned int first, unsigned int num) {

}
*/
