#ifndef _LOG_WRITER_DEF_H_
#define _LOG_WRITER_DEF_H_
#include <Filesystem/Filesystem.h>

class LogWriter
{
private:
    DataOutputStream* m_logOutStream;

public:
    LogWriter(File* file);
    ~LogWriter();

    void writeData(const char* Data);

    static File* getPathToLogFile(const char* Name);
};

#endif //_LOG_WRITER_DEF_H_
