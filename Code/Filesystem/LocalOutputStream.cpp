#include "LocalOutputStream.h"

LocalDataOutputStream::LocalDataOutputStream(const char* Path)
{

    //Allocate memory and copy the file path
    m_filePath = new char[strlen(Path) + 1];
    strncpy(m_filePath, Path, strlen(Path) + 1);

    //Null the fileOutputStream pointer
    m_fileOutputStream = 0;
}

LocalDataOutputStream::~LocalDataOutputStream()
{

    //If the stream is still open
    if (isOpen() == true)
    {
        //Close it
        close();

    }

    //If there is a valid file path
    if (m_filePath != 0)
    {

        //Free up the memory allocated to store it
        delete[] m_filePath;
        m_filePath = 0;

    }

}

//Returns true if the output stream is already open
bool LocalDataOutputStream::isOpen()
{

    //If there is a fileOutputStream
    if (m_fileOutputStream != 0)
    {
        //return true, the stream is open
         return true;

    }

    //Otherwise return false
    return false;
}

//Attempt to open a output stream for the dataoutputstream class
bool LocalDataOutputStream::open( bool textMode )
{

    //If the file path is null return false
    if (m_filePath == 0) return false;

    //if the stream is already open return false
    if (isOpen() == true) return false;

    //Attempt to open the file
    if (textMode == true)
    {
        printf("Opening in text mode\n");

        m_fileOutputStream = fopen(m_filePath, "w");

    }
    else
    {
        printf("Open in binary write mode\n");

        m_fileOutputStream = fopen(m_filePath, "wb");

    }

    //If the open was good then return true
    if (m_fileOutputStream != 0) return true;

    //Otherwise return false
    return false;
}

void LocalDataOutputStream::close()
{

    if (isOpen() == true)
    {

        fclose(m_fileOutputStream);
        m_fileOutputStream = 0;

    }

}

//Attempt to write specified data to the stream
bool LocalDataOutputStream::write(const void* Data, unsigned long size)
{

    if (isOpen() == true)
    {

        if (fwrite(Data, size, 1, m_fileOutputStream) != 1)
        {
            return false;
        }

        return true;

    }
    else
    {

        return false;

    }

}

void LocalDataOutputStream::flush()
{
    if (isOpen() == true)
        fflush(m_fileOutputStream);
}
