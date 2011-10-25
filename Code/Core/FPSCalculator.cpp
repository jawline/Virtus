#include "FPSCalculator.h"
#include <stdio.h>

FPSCalculator::FPSCalculator()
{
    m_curTime = 0;
    m_curNumFrames = 0;
    m_lastAvg = 0;
}

void FPSCalculator::frame(float seconds)
{
    m_curNumFrames++;
    m_curTime += seconds;

    if (m_curTime >= 0.05)
    {
        m_lastAvg = ((float)m_curNumFrames) / m_curTime;
        m_curNumFrames = 0;
        m_curTime = 0;
    }
}
