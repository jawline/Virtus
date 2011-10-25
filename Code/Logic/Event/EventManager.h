#ifndef _EVENT_MANAGER_DEF_H_
#define _EVENT_MANAGER_DEF_H_
#include "EventCallback.h"
#include <vector>

struct event_callback_struct
{
    std::vector<EventCallback*>* m_eventList;
};

/**
 * @brief Manages and stores mapping between events
 *
 */
class EventManager
{
private:
    unsigned int m_numEvents;
    event_callback_struct* m_eventCallbacks;

    void freeEvents();

public:
    EventManager();
    ~EventManager();

    /**
     * @brief Set the number of possible events (From 0 to number)
     *
     */

    void setNumberOfEvents(unsigned int number);

    /**
     * @brief Unregister the specified callback from the event
     *
     */

    void unregisterEventCallback(unsigned int event, EventCallback* Callback);

    /**
     * @brief Register the specified callback to the event
     *
     */

    void registerEventCallback(unsigned int event, EventCallback * Callback);

    /**
     * @brief Notify the event manager that the specified event has occured
     *
     */
    void triggerEvent(unsigned int event,std::vector<int> F, int Flags);
};

#endif //_EVENT_MANAGER_DEF_H_
