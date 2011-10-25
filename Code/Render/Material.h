#ifndef _RENDER_ENGINE_MATERIAL_
#define _RENDER_ENGINE_MATERIAL_
#include "GraphicsAPI/Texture.h"
#include "GraphicsAPI/Shader.h"

class Material
{
private:
    Shader* materialShader;

    char** textureNames;
    Texture* textures;

    Material();


private:
    ~Material();

    static Material* loadMaterial(FILE* input);

    void apply();
};

#endif //_RENDER_ENGINE_MATERIAL_
