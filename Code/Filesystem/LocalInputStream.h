#ifndef _LOCAL_DATA_INPUT_STREAM_DEF_H_
#define _LOCAL_DATA_INPUT_STREAM_DEF_H_
#include <stdio.h>
#include <string>
using namespace std;
#include "Stream.h"

class LocalDataInputStream : public DataInputStream
{
private:
    string m_localDataFilePath;
    FILE* m_fileInputStream;

public:

    LocalDataInputStream(string filePath);
    ~LocalDataInputStream();

    bool read(void* target, unsigned long amount);
    void close();
    bool open(bool textMode);
    bool isOpen();

    bool skip(unsigned long bytes);
    bool rewind(unsigned long bytes);

};

#endif //_LOCAL_DATA_INPUT_STREAM_DEF_H_
