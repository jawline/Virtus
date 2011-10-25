#ifndef _INPUT_EVENT_DEF_H_
#define _INPUT_EVENT_DEF_H_
#include <vector>

typedef enum
{
    input_event_start = 0,
    input_event_keypressed,
    input_event_keyreleased,
    input_event_button_one,
    input_event_button_two,
    input_event_window,
    input_event_end
} input_event_ids;

#define KEY_UPPERCASE 0x1

struct Input_Event
{
    int Event_ID;
    int Event_Flags;

    std::vector<int> Event_Data;
};

#endif //_INPUT_EVENT_DEF_H_
