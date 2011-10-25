#include "LocalFile.h"
#include <stdio.h>
#include <string.h>
#include "LocalOutputStream.h"
#include "LocalInputStream.h"
#include <sys/stat.h>

LocalFile::LocalFile(const char* Path)
{
    m_localName = 0;
    m_localFilePath = 0;

    if (Path != 0)
    {

        m_localFilePath = new char[strlen(Path) + 1];
        strncpy(m_localFilePath, Path, strlen(Path) + 1);

        const char* lastSlash = strrchr(Path, '/');

        if (lastSlash == 0)
        {
            lastSlash = Path;
        }
        else
        {
            //Increment by 1 to remove the / from the string
            lastSlash++;
        }

        m_localName = new char[ strlen(lastSlash) + 1 ];
        strncpy(m_localName, lastSlash, strlen(lastSlash) + 1);

    }
}

LocalFile::~LocalFile()
{

    if (m_localFilePath != 0)
    {

        delete[] m_localFilePath;
        m_localFilePath = 0;

    }

    if (m_localName != 0)
    {
        delete[] m_localName;
        m_localName = 0;
    }

    return;
}

DataInputStream* LocalFile::createInputStream()
{
    return new LocalDataInputStream(m_localFilePath);
}

DataOutputStream* LocalFile::createOutputStream()
{
    return new LocalDataOutputStream(m_localFilePath);
}

bool LocalFile::exists()
{

  struct stat fileInfo;

  //If stat returns 0 something by that name exists
  if (stat(m_localFilePath, &fileInfo) == 0)
    return true;

  return false;
}

unsigned long LocalFile::length()
{

  struct stat fileInfo;

  //If stat returns 0 something by that name exists
  if (stat(m_localFilePath, &fileInfo) == 0)
  {

        return fileInfo.st_size;

  }

  return 0;
}
