#ifndef _MODELVIEWER_APPLICATION_DEF_H_
#define _MODELVIEWER_APPLICATION_DEF_H_
#include "../API/Application.h"
#include <Core/GEngine.h>
#include "EventHandlers.h"

#include "../Render/3D/Terrain/TerrainNode.h"
#include "../Render/3D/TestNode.h"
#include "UserInterface.h"

class ModelViewer : public GApplication
{

private:
    bool m_Forward;
    bool m_Backward;
    bool m_Left;
    bool m_Right;
    bool m_Up;
    bool m_Down;

    SceneNode* m_worldNode;

    PressedEventHandler* ForwardHandle;
    ReleasedEventHandler* ForwardReleaseHandle;

    PressedEventHandler* LeftHandle;
    ReleasedEventHandler* LeftReleaseHandle;

    PressedEventHandler* RightHandle;
    ReleasedEventHandler* RightReleaseHandle;

    PressedEventHandler* BackwardHandle;
    ReleasedEventHandler* BackwardReleaseHandle;

    PressedEventHandler* UpHandle;
    ReleasedEventHandler* UpReleaseHandle;

    PressedEventHandler* DownHandle;
    ReleasedEventHandler* DownReleaseHandle;

    CloseHandler* windowCloseHandler;

    ToggleWireframeHandler* m_toggleWireframeHandler;

    IElement* m_viewerInterface;
    IElement* m_wireframeButton;

    GEngine* m_gameEngine;

public:
    bool onLoad(GEngine* Engine);
    bool onUnload();

    void updateApplication(float seconds);
};

#endif //_MODELVIEWER_APPLICATION_DEF_H_
