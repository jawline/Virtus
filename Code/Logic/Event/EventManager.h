#ifndef _EVENT_MANAGER_DEF_H_
#define _EVENT_MANAGER_DEF_H_
#include "EventCallback.h"
#include <vector>

struct event_callback_struct
{
    std::vector<EventCallback*>* m_eventList;
};

class EventManager
{
private:
    unsigned int m_numEvents;
    event_callback_struct* m_eventCallbacks;

    void freeEvents();

public:
    EventManager();
    ~EventManager();

    void setNumberOfEvents(unsigned int number);

    void unregisterEventCallback(unsigned int event, EventCallback* Callback);
    void registerEventCallback(unsigned int event, EventCallback * Callback);

    void triggerEvent(unsigned int event,std::vector<int> F, int Flags);
};

#endif //_EVENT_MANAGER_DEF_H_
