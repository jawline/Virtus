#ifndef _TEXTURE_MONITOR_DEF_H_
#define _TEXTURE_MONITOR_DEF_H_
#include <vector>
#include <iostream>
#include "../GraphicsAPI/Texture.h"
#include "TextureLoader.h"
#include "../ResourceMonitor/ResourceMonitor.h"
#include <Filesystem/Filesystem.h>


class TextureMonitor : public ResourceMonitor
{
private:
    TextureLoader m_Loader;

    static const char* TAG;

    void f_freeEntity(void* m_Entity);
    void* f_loadEntity(std::string Name);

public:
    TextureMonitor();
    ~TextureMonitor();

    Texture* getTexture(const char* Filename);
    void releaseTexture(Texture* texture);

};

#endif //_TEXTURE_MONITOR_DEF_H_
