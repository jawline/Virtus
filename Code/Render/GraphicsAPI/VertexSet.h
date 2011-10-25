#ifndef _VERTEX_SET_DEF_H_
#define _VERTEX_SET_DEF_H_

enum ColourValueTypes
{
    ctype_none = 0,
    ctype_float_rgb,
    ctype_float_rgba
};

enum PrimitiveTypes
{
    POINTS = 0,
    QUADS,
    QUAD_STRIP,
    TRIANGLES,
    TRIANGLE_STRIP,
    LINES,
    LINE_STRIP
};

class VertexSet
{
private:

public:

        virtual ~VertexSet() {}

        virtual void setVertices(float* vertices, unsigned int number) = 0;
        virtual void setNormals(float* normals, unsigned int number) = 0;
        virtual void setTextureCoordinates(float* tcoords, unsigned int number) = 0;
        virtual void setColourValues(float* cvals, enum ColourValueTypes type, unsigned int points) = 0;
        virtual void setIndices(unsigned int* indices, unsigned int numberPoints) = 0;

        virtual float* getVertices() = 0;
        virtual float* getNormals() = 0;
        virtual float* getTextureCoordinates() = 0;
        virtual float* getColourValues() = 0;
        virtual unsigned int* getIndices() = 0;

        virtual void Draw() = 0;
        virtual void Draw(unsigned int start_vertex, unsigned int num_Vertices, PrimitiveTypes type)  = 0;

        virtual unsigned int getNumVertices() = 0;
        virtual unsigned int getNumNormals() = 0;
        virtual unsigned int getNumTextureCoordinates() = 0;
        virtual unsigned int getNumColourValues() = 0;
        virtual unsigned int getNumIndices() = 0;
};

#endif //_VERTEX_SET_DEF_H_
