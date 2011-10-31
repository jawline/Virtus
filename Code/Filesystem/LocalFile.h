#ifndef _LOCALFILE_DEF_H_
#define _LOCALFILE_DEF_H_
#include "File.h"
#include <string>
using namespace std;

class LocalFile : public File
{
private:
    string m_localFilePath;
    string m_localName;

public:

    LocalFile(string FullPath);
    ~LocalFile();

    DataInputStream* createInputStream();
    DataOutputStream* createOutputStream();

    bool exists();
    unsigned long length();

    string getName();
};

#endif //_LOCALFILE_DEF_H_
