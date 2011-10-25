#ifndef _KEY_IDS_DEF_H_
#define _KEY_IDS_DEF_H_

typedef enum
{
    KEYS_START = 0,

    KEY_A,
    KEY_B,
    KEY_C,
    KEY_D,
    KEY_E,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_I,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_M,
    KEY_N,
    KEY_O,
    KEY_P,
    KEY_Q,
    KEY_R,
    KEY_S,
    KEY_T,
    KEY_U,
    KEY_V,
    KEY_W,
    KEY_X,
    KEY_Y,
    KEY_Z,

    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    KEY_0,

    KEY_RETURN,
    KEY_BACKSPACE,
    KEY_SPACE,

    KEY_UP,
    KEY_DOWN,
    KEY_LEFT,
    KEY_RIGHT,

    /* Mouse events are passed as keys */
    MOUSE_DOWN,
    MOUSE_UP,
    MOUSE_MOTION,

    /* Window events are passed the same way */
    WINDOW_KILL_REQUEST,

    KEYS_END
} key_ids;

#endif //_KEY_IDS_DEF_H_
