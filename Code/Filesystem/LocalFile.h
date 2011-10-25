#ifndef _LOCALFILE_DEF_H_
#define _LOCALFILE_DEF_H_
#include "File.h"

class LocalFile : public File
{
private:
    char* m_localFilePath;
    char* m_localName;

public:

    LocalFile(const char* FullPath);
    ~LocalFile();

    DataInputStream* createInputStream();
    DataOutputStream* createOutputStream();

    bool exists();
    unsigned long length();

    char* getName() { return m_localName; }
};

#endif //_LOCALFILE_DEF_H_
