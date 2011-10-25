#ifndef _FPS_CALCULATOR_DEF_H_
#define _FPS_CALCULATOR_DEF_H_

/**
 * @brief Class to calculate how many frames are occurring per second
 */

class FPSCalculator
{
private:
    float m_curNumFrames;
    float m_curTime;

    float m_lastAvg;

public:
    FPSCalculator();

    /**
     * @brief Notify that a frame has occured and how long it took
     */
    void frame(float seconds);

    /**
     * @brief Return the last calculated FPS
     */
    long getFPS()
    {
        return m_lastAvg;
    }

};

#endif //_FPS_CALCULATOR_DEF_H_
