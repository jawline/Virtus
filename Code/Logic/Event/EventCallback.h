#ifndef _EVENT_CALLBACK_DEF_H_
#define _EVENT_CALLBACK_DEF_H_
#include <vector>

class EventCallback
{
public:
    virtual ~EventCallback() { }
    virtual void triggerCallback(std::vector<int> Event_Data, int Flags) { }
};

#endif //_EVENT_CALLBACK_DEF_H_
