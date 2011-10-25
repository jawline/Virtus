#include "LocalInputStream.h"

LocalDataInputStream::LocalDataInputStream(const char* filePath)
{

    m_localDataFilePath = new char[strlen(filePath) + 1];
    strncpy(m_localDataFilePath, filePath, strlen(filePath) + 1);

    m_fileInputStream = 0;

}

LocalDataInputStream::~LocalDataInputStream()
{

    if (isOpen())
    {
        close();
    }

    if (m_localDataFilePath != 0)
    {

        delete[] m_localDataFilePath;
        m_localDataFilePath = 0;

    }

}

bool LocalDataInputStream::read(void* target, unsigned long amount)
{
    if (isOpen() == false)
    {

        return false;

    }

    if (fread(target, amount, 1, m_fileInputStream) != 1)
    {
        return false;
    }

    return true;
}

void LocalDataInputStream::close()
{

    if (isOpen())
    {

        fclose(m_fileInputStream);
        m_fileInputStream = 0;

    }

}

bool LocalDataInputStream::open(bool textMode)
{

    if (isOpen()) return false;
    if (m_localDataFilePath == 0) return false;

    if (textMode)
        m_fileInputStream = fopen(m_localDataFilePath, "r");
    else
        m_fileInputStream = fopen(m_localDataFilePath, "rb");

    if (m_fileInputStream != 0)
    {
        return true;
    }

    return false;
}

bool LocalDataInputStream::isOpen()
{

    if (m_fileInputStream != 0)
    {

        return true;

    }

    return false;
}

bool LocalDataInputStream::skip(unsigned long bytes)
{

    //Check the file is open
    if (isOpen() == false) return false;

    if ( fseek( m_fileInputStream, bytes, SEEK_CUR) != 0)
    {
        //Error seeking, return a bad result
        return false;
    }

    //Otherwise it seeked fine
    return true;
}

bool LocalDataInputStream::rewind(unsigned long bytes)
{
    //Check if the file is open
    if (isOpen() == false) return false;

    if ( fseek( m_fileInputStream, -bytes, SEEK_CUR) != 0)
    {
        return false;
    }

    return true;
}
