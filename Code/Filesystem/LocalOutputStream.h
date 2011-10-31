#ifndef _LOCALOUTPUT_STREAM_DEF_H_
#define _LOCALOUTPUT_STREAM_DEF_H_
#include "Stream.h"
#include <stdio.h>
#include <string>
using namespace std;

class LocalDataOutputStream : public DataOutputStream
{
private:

    string m_filePath;
    FILE* m_fileOutputStream;

public:

    LocalDataOutputStream(string FilePath);
    ~LocalDataOutputStream();

    bool isOpen();
    bool open( bool textMode );
    void close();
    bool write(const void* Data, unsigned long len);
    void flush();

};

#endif //_LOCALOUTPUT_STREAM_DEF_H_
