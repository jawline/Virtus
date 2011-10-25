#ifndef _COLOR_DEF_H_
#define _COLOR_DEF_H_

/**
 * @brief A color within the game engine
 */

class Color
{
public:

    Color()
    {
        r = 0;
        g = 0;
        b = 0;
    }

    Color(float rr, float gg, float bb)
    {
        Color::r = rr;
        Color::g = gg;
        Color::b = bb;
    }

    float r, g, b;
};

#endif //_COLOUR_DEF_H_
