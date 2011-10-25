#ifndef _RENDER_LOG_DEF_H_
#define _RENDER_LOG_DEF_H_
#include <stdio.h>

class RenderLog
{
private:
    FILE* m_logfileHandle;



public:
    RenderLog(const char* outputFile);
    ~RenderLog();

    void writeToLogFile(const char* line);
};

#endif //_RENDER_LOG_DEF_H_
