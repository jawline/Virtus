#include "TextureMonitor.h"
#include "../GRender.h"

TextureMonitor::TextureMonitor()
{
    printf("Texture Monitor initialized\n");
}

TextureMonitor::~TextureMonitor()
{
    printf("Destroying texture monitor\n");

    for (unsigned int i = 0; i < m_resourceList.size(); ++i)
    {
        f_freeEntity(m_resourceList[i].m_Entity);
    }

    m_resourceList.clear();
}


void TextureMonitor::f_freeEntity(void* m_Entity)
{
    Texture* texture = (Texture*) m_Entity;
    delete texture;

    return;
}

void* TextureMonitor::f_loadEntity(std::string Name)
{
    printf("Load entity called\n");
    Texture* newTexture = GRenderer::getRenderer()->getGraphicsAPI()->newTexture();

    if (newTexture == 0)
    {
        char Buffer[512];
        sprintf(Buffer, "Unable to load texture %s due to being unable to create a Texture object\n", Name.c_str());
        GRenderer::getRenderer()->getRenderLog()->writeToLogFile(Buffer);
        return 0;
    }

    FilesystemNode* node = Filesystem::evaluateChild(Filesystem::getRoot(), Name.c_str());

    if (node == 0)
    {
        char Buffer[512];
        sprintf(Buffer, "Unable to load texture %s due to being unable to find specified node in filesystem\n", Name.c_str());
        GRenderer::getRenderer()->getRenderLog()->writeToLogFile(Buffer);
        return 0;
    }

    if (node->getType() != fileId)
    {
        char Buffer[512];
        sprintf(Buffer, "Unable to load texture %s due to node not being marked as a file in the filesystem\n", Name.c_str());
        GRenderer::getRenderer()->getRenderLog()->writeToLogFile(Buffer);
        return 0;
    }

    File* castToFile = static_cast<File*> (node);

    DataInputStream* newStream = castToFile->createInputStream();
    newStream->open(false);

    if (m_Loader.load(newTexture, newStream) == false)
    {

        char Buffer[512];
        sprintf(Buffer, "Unable to load texture %s due to being unable to load from file path\n", Name.c_str());
        GRenderer::getRenderer()->getRenderLog()->writeToLogFile(Buffer);

        delete newTexture;

        //Delete the input stream once its been used
        delete newStream;

        return 0;
    }
    else
    {

        char Buffer[512];
        sprintf(Buffer, "Succesfully loaded texture %s\n", Name.c_str());
        GRenderer::getRenderer()->getRenderLog()->writeToLogFile(Buffer);

        f_addEntity( Name, (void*) newTexture );

        //Delete the input stream once its been used
        delete newStream;

        return (void*)newTexture;
    }
}

Texture* TextureMonitor::getTexture(const char* Filename)
{
    return (Texture*) rmGetResource(Filename);
}

void TextureMonitor::releaseTexture(Texture* Texture)
{
    rmReleaseResource((void*)Texture);
}
