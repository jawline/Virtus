#ifndef _FILESYSTEM_FILE_DEF_H_
#define _FILESYSTEM_FILE_DEF_H_
#include "Node.h"
#include "Stream.h"

const int fileId = 1;

/**
 * @brief The abstraction of a file within the filesystem
 *
 */

class File : public FilesystemNode
{
private:

public:

    int getType()
    {
        return fileId;
    }

    virtual ~File()
    {
    }

    /**
     * @brief Create a input stream to that file
     *
     */

    virtual DataInputStream* createInputStream() = 0;

    /**
     * @brief Create a output stream to that file
     *
     */

    virtual DataOutputStream* createOutputStream() = 0;

    /**
     * @brief Returns true if the file actually exists, or will be created when a output stream is found
     *
     */

    virtual bool exists() = 0;

    /**
     * @brief Return the length of the file
     *
     */
    virtual unsigned long length() = 0;

};

#endif //_FILESYSTEM_FILE_DEF_H_
