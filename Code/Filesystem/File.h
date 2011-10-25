#ifndef _FILESYSTEM_FILE_DEF_H_
#define _FILESYSTEM_FILE_DEF_H_
#include "Node.h"
#include "Stream.h"

const int fileId = 1;

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

    virtual DataInputStream* createInputStream() = 0;
    virtual DataOutputStream* createOutputStream() = 0;

    virtual bool exists() = 0;
    virtual unsigned long length() = 0;

};

#endif //_FILESYSTEM_FILE_DEF_H_
