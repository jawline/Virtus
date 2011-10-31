#include "LogWriter.h"
#include <stdio.h>
#include <string.h>

LogWriter::LogWriter(File* file)
{
    if (file == 0) return;
    else
    {
        printf("LogWriter passed invalid file\n");
    }

    m_logOutStream = file->createOutputStream();

    if (m_logOutStream != 0)
    {
        if ( m_logOutStream->open(true) == false)
        {
            printf("Unable to open output stream for file %s\n", file->getName().c_str());
        }
    }
    else
    {
        printf("LogWriter unable to create output stream for file %s\n", file->getName().c_str());
    }

}

LogWriter::~LogWriter()
{
    if (m_logOutStream != 0)
    {

        delete m_logOutStream;
        m_logOutStream = 0;

    }
}

void LogWriter::writeData(const char* Data)
{

    if (m_logOutStream != 0)
    {

        if (m_logOutStream->isOpen() == true)
        {

            m_logOutStream->write(Data, strlen(Data));
            m_logOutStream->flush();

        }

    }

}

File* LogWriter::getPathToLogFile(const char* Name)
{
    Folder* folder = static_cast<Folder*>(Filesystem::evaluateChild(Filesystem::getRoot(), "Logs"));

    if (folder == 0) return 0;
    if (folder->getType() != folderId) return 0;

    File* searchForFile = static_cast< File * >( folder->searchForChild(Name) );

    if (searchForFile != 0)
    {

        return searchForFile;

    }
    else
    {

        return folder->newChild(Name);

    }

}
