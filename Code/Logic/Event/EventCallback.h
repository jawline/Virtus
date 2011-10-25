#ifndef _EVENT_CALLBACK_DEF_H_
#define _EVENT_CALLBACK_DEF_H_
#include <vector>

/**
 * @brief Callback for a event
 */

class EventCallback
{

public:
    virtual ~EventCallback() { }

    /**
     * @brief Called when a event this callback is bound too is triggered
     */
    virtual void triggerCallback(std::vector<int> Event_Data, int Flags) = 0;
};

#endif //_EVENT_CALLBACK_DEF_H_
