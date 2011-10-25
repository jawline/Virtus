#ifndef _LOG_WRITER_DEF_H_
#define _LOG_WRITER_DEF_H_
#include <Filesystem/Filesystem.h>

/**
 * @brief Convenience class for a log
 *
 */

class LogWriter
{
private:

    DataOutputStream* m_logOutStream;

public:

    /**
     * @brief Creates a instance of a log bound to a specific FileSystem File
     */
    LogWriter(File* file);

    /**
     * @brief Destroy the log. Flushes any buffers and deinitializes all
     */
    ~LogWriter();

    /**
     * @brief Write the specified line to the log
     */

    void writeData(const char* Data);

    /**
     * @brief Convert the string name of the log (I.e. Render.Log) to a internal filesystem node (Creating it if it doesn't already exist)
     */

    static File* getPathToLogFile(const char* Name);
};

#endif //_LOG_WRITER_DEF_H_
