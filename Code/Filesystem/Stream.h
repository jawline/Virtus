#ifndef _DATA_STREAM_DEF_H_
#define _DATA_STREAM_DEF_H_

class DataInputStream
{

private:

public:

    virtual ~DataInputStream() {}

    virtual bool isOpen() = 0;
    virtual bool open( bool textMode ) = 0;
    virtual void close() = 0;
    virtual bool read(void* targetLocation, unsigned long len) = 0;

    virtual bool skip(unsigned long bytes) = 0;
    virtual bool rewind(unsigned long bytes) = 0;
};

class DataOutputStream
{
private:
public:

    virtual ~DataOutputStream() {}

    virtual bool isOpen() = 0;
    virtual bool open( bool textMode ) = 0;
    virtual void close() = 0;
    virtual bool write(const void* Data, unsigned long len) = 0;
    virtual void flush() = 0;
};

#endif //_DATA_STREAM_DEF_H_
