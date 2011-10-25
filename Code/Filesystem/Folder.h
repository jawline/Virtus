#ifndef _FILESYSTEM_FOLDER_DEF_H_
#define _FILESYSTEM_FOLDER_DEF_H_
#include "Node.h"
#include "File.h"

const int folderId = 2;

class Folder : public FilesystemNode
{
public:

    virtual unsigned int numChildren() = 0;
    virtual FilesystemNode* getChild(unsigned int number) = 0;

    virtual void refresh() = 0;
    virtual File* newChild(const char* Name) = 0;
    virtual Folder* newChildFolder(const char* Name) = 0;
    virtual FilesystemNode* searchForChild(const char* Name, unsigned int nameLength = 0) = 0;

    int getType()
    {
        return folderId;
    }

    virtual ~Folder()
    {
    }

};

#endif //_FILESYSTEM_FOLDER_DEF_H_
