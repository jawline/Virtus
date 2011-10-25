#ifndef _FPS_CALCULATOR_DEF_H_
#define _FPS_CALCULATOR_DEF_H_

class FPSCalculator
{
private:
    float m_curNumFrames;
    float m_curTime;

    float m_lastAvg;

public:
    FPSCalculator();
    void frame(float seconds);

    long getFPS()
    {
        return m_lastAvg;
    }

};

#endif //_FPS_CALCULATOR_DEF_H_
