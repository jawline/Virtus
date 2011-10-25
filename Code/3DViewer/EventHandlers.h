#ifndef _VIEWER_EVENT_HANDLERS_DEF_H_
#define _VIEWER_EVENT_HANDLERS_DEF_H_
#include <Logic/Event/EventCallback.h>

class CloseHandler : public EventCallback
{
private:

public:

    CloseHandler()
    {

    }

    void triggerCallback(std::vector<int> I, int Flags)
    {
        printf("EventHandlers: Close request recieved\n");

        if (I.size() != 1)
            return;

        if (I[0] != WINDOW_KILL_REQUEST)
            return;

        GEngine::getInstance()->killEngine();
    }
};

class PressedEventHandler : public EventCallback
{
private:
    bool* m_toManip;

public:
    PressedEventHandler(bool* flag)
    {
        m_toManip = flag;
    }

    void triggerCallback(std::vector<int> I, int Flags)
    {

        *m_toManip = true;

    }

};

class ReleasedEventHandler : public EventCallback
{
private:
    bool* m_toManip;

public:
    ReleasedEventHandler(bool* flag)
    {
        m_toManip = flag;
    }

    void triggerCallback(std::vector<int> I, int Flags)
    {

        *m_toManip = false;

    }

};
class ToggleWireframeHandler : public EventCallback
{

    void triggerCallback(std::vector<int> I, int Flags)
    {

        //Get current value
        bool cWF = GRenderer::getRenderer()->getRenderWireframeValue();

        //Swap it
        cWF = !cWF;

        //Set it
        GRenderer::getRenderer()->setRenderWireframeValue(cWF);
    }
};

#endif //_VIEWER_EVENT_HANDLERS_DEF_H_
