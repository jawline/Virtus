#ifndef _RENDER_DEF_H_
#define _RENDER_DEF_H_

#include <vector>
#include "LayerIDs.h"
#include "GRenderLayer.h"
#include "./TextureMonitor/TextureMonitor.h"
#include "./GraphicsAPI/Window.h"
#include "./GraphicsAPI/GraphicsAPI.h"
#include <LogWriter/LogWriter.h>

class GSpriteLayer;
class InterfaceLayer;
class ThreeDRenderLayer;

/**
 * @brief The core renderer class - Singleton
 *
 */
class GRenderer
{
private:
	static const char* TAG;

    GraphicsAPI* m_renderAPI;

    GRenderLayer* m_renderLayers[Layers_Max];
    LogWriter* m_gameRenderLog;

    TextureMonitor* m_textureMonitor;
    bool m_shouldRenderWireframe;

    void setLayer(int ID, GRenderLayer * Layer)
    {

        if ((ID < Layers_Start) | (ID > Layers_Max)) return;

        m_renderLayers[ID] = Layer;

    }

    GRenderer();

    void begin(int wx, int wy);
    void end(int wx, int wy);
public:

    ~GRenderer();

    bool initialize(unsigned int w, unsigned int h, const char* renderAPI);
    void uninitialize();

    void update(float seconds);

    void renderScene(int wx, int wy);

    GraphicsAPI* getGraphicsAPI()
    {
        return m_renderAPI;
    }


    GRenderLayer* getLayer(int ID)
    {
        if ((ID < Layers_Start) | (ID > Layers_Max)) return 0;

        return m_renderLayers[ID];
    }

    GSpriteLayer* getSpriteLayer()
    {
        return (GSpriteLayer*) m_renderLayers[Layer_Sprites];
    }

    InterfaceLayer* getInterfaceManager()
    {
        return (InterfaceLayer*) m_renderLayers[Layer_Interface];
    }

    ThreeDRenderLayer* get3DLayer()
    {
        return (ThreeDRenderLayer*) m_renderLayers[Layer_3D];
    }

    TextureMonitor* getTextureMonitor()
    {
        return m_textureMonitor;
    }

    LogWriter* getRenderLog()
    {
        return m_gameRenderLog;
    }

    RenderWindow* getWindow()
    {
        if (m_renderAPI != 0)
            return m_renderAPI->getRenderWindow();

        return 0;
    }

    void setRenderWireframeValue(bool value);
    bool getRenderWireframeValue();

    static GRenderer* getRenderer();
};

#endif //_RENDER_DEF_H_
