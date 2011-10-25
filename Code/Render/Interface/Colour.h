#ifndef _COLOUR_DEF_H_
#define _COLOUR_DEF_H_

class Colour
{
public:

    Colour()
    {
        r = 0;
        g = 0;
        b = 0;
    }

    Colour(float rr, float gg, float bb)
    {
        Colour::r = rr;
        Colour::g = gg;
        Colour::b = bb;
    }

    float r, g, b;
};

#endif //_COLOUR_DEF_H_
