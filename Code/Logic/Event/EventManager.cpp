#include "EventManager.h"
#include <stdio.h>

EventManager::EventManager()
{
    m_numEvents = 0;
    m_eventCallbacks = 0;
}

EventManager::~EventManager()
{
    freeEvents();
}

void EventManager::freeEvents()
{

    if (m_eventCallbacks != 0)
    {

        for (unsigned int i = 0; i < m_numEvents; ++i)
        {

            delete m_eventCallbacks[i].m_eventList;

        }

        delete[] m_eventCallbacks;

    }

    m_eventCallbacks = 0;
    m_numEvents = 0;

}

void EventManager::setNumberOfEvents(unsigned int num)
{

    freeEvents();

    m_eventCallbacks = new event_callback_struct[num];

    for (unsigned int i = 0; i < num; i++)
        EventManager::m_eventCallbacks[i].m_eventList = new std::vector<EventCallback*>;


    m_numEvents = num;
}

void EventManager::unregisterEventCallback(unsigned int event, EventCallback* Cb)
{
    if (event >= m_numEvents) return;

    for (unsigned int i = 0; i < m_eventCallbacks[event].m_eventList->size(); i++)
    {
        if (m_eventCallbacks[event].m_eventList->at(i) == Cb)
        {
            m_eventCallbacks[event].m_eventList->erase(m_eventCallbacks[event].m_eventList->begin() + i);
            return;
        }
    }
}

void EventManager::registerEventCallback(unsigned int event, EventCallback * Callback)
{
    if (event >= m_numEvents) return;

    m_eventCallbacks[event].m_eventList->push_back(Callback);

}

void EventManager::triggerEvent(unsigned int event, std::vector<int> EData, int Flags)
{
    if (event >= m_numEvents) return;

    size_t loop_len = m_eventCallbacks[event].m_eventList->size();

    for (unsigned int i = 0; i < loop_len; i++)
    {
        m_eventCallbacks[event].m_eventList->at(i)->triggerCallback(EData, Flags);
    }
}
