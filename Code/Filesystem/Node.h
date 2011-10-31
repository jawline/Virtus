#ifndef _FILESYSTEM_NODE_DEF_H_
#define _FILESYSTEM_NODE_DEF_H_
#include <string>
using namespace std;

class FilesystemNode
{
public:

    virtual ~FilesystemNode() {}
    virtual int getType() = 0;

    virtual string getName() = 0;

};

#endif //_FILESYSTEM_NODE_DEF_H_
