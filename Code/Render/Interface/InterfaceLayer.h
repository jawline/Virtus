#ifndef _INTERFACE_LAYER_DEF_H_
#define _INTERFACE_LAYER_DEF_H_
#include "../GRenderLayer.h"
#include "../LayerIDs.h"
#include "../Fonts/FREngine.h"
#include <Input/InputEvent.h>
#include "IElement.h"
#include "Colour.h"
#include "../TextureMonitor/TextureMonitor.h"
#include "../GraphicsAPI/VertexSet.h"

class InterfaceLayer : public GRenderLayer
{
private:
    FREngine m_defaultTextRenderer;
    std::vector<IElement*> m_interfaceElements;
    Texture* m_interfaceTex;

    //Vertex set used to store the vertices for the UI quad drawing
    VertexSet* m_quadVertexSet;

public:

    InterfaceLayer();

    void draw(GRenderer*, int width, int height);

    int getID()
    {
        return Layer_Interface;
    }

    void AddElement(IElement* E)
    {
        m_interfaceElements.push_back(E);
    }

    void RemoveElement(IElement* E)
    {

        for (unsigned int i = 0; i < m_interfaceElements.size(); i++)
        {

            if (m_interfaceElements[i] == E)
            {

                m_interfaceElements.erase(m_interfaceElements.begin() + i);

            }

        }

    }

    //Drawing functions - Public - Utalized by InterfaceElements to draw complex objects abstractly
    void drawText(int PointSize, int x, int y, std::string Text, FREngine* Renderer);

    void drawQuad(float x, float y, float w, float h);
    void drawQuadWithUIElement(float x, float y, float w, float h, unsigned int element_x, unsigned int element_y);

    bool processEvent(Input_Event E);

    FREngine* getDefaultTextRenderer()
    {
        return &m_defaultTextRenderer;
    }
};

#endif //_INTERFACE_LAYER_DEF_H_
