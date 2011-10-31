#ifndef _GL_TEXTURE_DEF_H_
#define _GL_TEXTURE_DEF_H_
#include "../Texture.h"
#include <GL/gl.h>

class GLTexture : public Texture
{
    private:
		static const char* TAG;
        GLuint m_glTextureID;


        unsigned int textureDebugIdentifier;

    public:

        GLTexture();
        ~GLTexture();

        void loadTextureData(InputDataFormat format, unsigned int width, unsigned int height, bool genMips, void* inputData);
        void makeActive();

        GLuint getTextureID() { return m_glTextureID; }
};

#endif //_GL_TEXTURE_DEF_H_
