#ifndef _GAME_ENGINE_DEF_H_
#define _GAME_ENGINE_DEF_H_
#include "./Render/GRender.h"
#include <Logic/Event/EventManager.h>
#include <Input/InputEvent.h>
#include <Input/Postbox.h>
#include <Input/Keys.h>
#include <Core/FPSCalculator.h>
#include <API/Application.h>
#include <Configure/ConfigurationInstance.h>
#include <HIVE/Hive.h>
#include <Filesystem/Filesystem.h>
#include <Filesystem/LocalFolder.h>
#include <LogWriter/LogWriter.h>
#include <Core/Clock.h>

typedef struct
{

    std::vector <unsigned int> Events;

} EventBinding;

typedef struct
{

    EventBinding* Keys;

} InputBinding;

class GEngine
{
private:
	static const char* TAG;

    bool initialize();
    void uninitialize();
    void initializeInputBindings();
    void freeInputBindings();

    FPSCalculator m_fpsCalculator;

    Clock m_clock;

    EventManager* m_eventManager;

    Postbox<Input_Event> m_inputPostbox;
    InputBinding* m_inputBindings;

    GApplication* m_currentApplication;

    ConfigurationInstance* m_engineCfg;

    BeeHive* m_threadHive;

    LogWriter* m_engineLog;

    void f_sleep();

    bool m_GameActive;
    bool m_physicsActive;
    bool m_GamePaused;

    bool m_errorState;
    const char* m_errorDetails;

    unsigned int m_canvasWidth;
    unsigned int m_canvasHeight;


    void setErrorDetails(const char* str);

    GEngine();

public:
    ~GEngine();

    //Bind a event to a event ID and key (For example, keypressed A or mouseclicked LEFT)
    void bindEvent(unsigned int Eve, input_event_ids, key_ids);
    void sendEventManagerMessage(Input_Event iEvent);
    void processEvent(Input_Event event);

    bool isInErrorState() { return m_errorState; }
    const char* errorDetails() { return m_errorDetails; }

    int gameLoop();

    void killEngine()
    {

        m_GameActive = false;

    }

    EventManager* getEventManager()
    {

        return m_eventManager;

    }

    FPSCalculator* getFpsCalculator()
    {

        return &m_fpsCalculator;

    }

    LogWriter* getLog()
    {
        return m_engineLog;
    }

    void pauseEngine()
    {

        m_GamePaused = true;

    }

    void unpauseEngine()
    {

        m_GamePaused = false;

    }

    void togglePaused()
    {

        m_GamePaused = !m_GamePaused;

    }

    void pausePhysics()
    {

        m_physicsActive = false;

    }

    void unpausePhysics()
    {

        m_physicsActive = true;

    }

    void togglePhysics()
    {

        m_physicsActive = !m_physicsActive;

    }

    bool loadApplication(GApplication* App);
    bool unloadApplication();

    bool applicationLoaded()
    {

        if (m_currentApplication)
        {

            return true;

        }

        return false;
    }

    unsigned int getCanvasWidth()
    {
        return m_canvasWidth;
    }
    unsigned int getCanvasHeight()
    {
        return m_canvasHeight;
    }

    void doFrameUpdate(float seconds);

    static GEngine* getInstance();
};

#endif //_GAME_ENGINE_DEF_H_
