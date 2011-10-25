#ifndef _GL2_VERTEX_SET_DEF_H_
#define _GL2_VERTEX_SET_DEF_H_
#include "../VertexSet.h"
#include <GL/glew.h>
#include <GL/gl.h>

class GL2VertexSet : public VertexSet
{
    private:

        unsigned int m_vertexSetVBO;

        unsigned int m_numVertices;
        GLfloat* m_vertices;

        unsigned int m_numNormals;
        GLfloat* m_normals;

        unsigned int m_numTexCoords;
        GLfloat* m_texCoords;

        unsigned int m_numIndices;
        GLuint* m_indices;

        unsigned int m_numColourValues;
        enum ColourValueTypes m_colourValuesType;
        GLfloat* m_colourVals;

    public:

        GL2VertexSet();
        ~GL2VertexSet();

        void setVertices(float* vertices, unsigned int points);
        void setNormals(float* normals, unsigned int points);
        void setTextureCoordinates(float* tcoords, unsigned int points);
        void setIndices(unsigned int* indices, unsigned int numberPoints);
        void setColourValues(float* cvals, enum ColourValueTypes type, unsigned int points);
/*
        void updateBuffers();
        void updateBuffers(unsigned int first, unsigned int num);
*/
        void Draw();
        void Draw(unsigned int start_vertex, unsigned int num_Vertices, PrimitiveTypes primitiveType);

        float* getVertices() { return m_vertices; }
        unsigned int getNumVertices() { return m_numVertices; }

        float* getNormals() { return m_normals; }
        unsigned int getNumNormals() { return m_numNormals; }

        float* getTextureCoordinates() { return m_texCoords; }
        unsigned int getNumTextureCoordinates() { return m_numTexCoords; }

        float* getColourValues() { return m_colourVals; }
        unsigned int getNumColourValues() { return m_numColourValues; }

        unsigned int* getIndices() { return m_indices; }
        unsigned int getNumIndices() { return m_numIndices; }
};

#endif //_VERTEX_SET_DEF_H_
