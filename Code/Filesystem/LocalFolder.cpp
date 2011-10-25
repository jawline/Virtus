#include "LocalFolder.h"
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include "LocalFile.h"


LocalFolder::LocalFolder(const char* Name, const char* Path)
{
    m_localFolderPath = 0;
    m_localName = 0;

    if (Path != 0 && Name != 0)
    {

        //Allocate the memory for the path
        m_localFolderPath = new char[ strlen(Path) + 1 ];

        //Copy the path
        strncpy(m_localFolderPath, Path, strlen(Path) + 1);

        //Allocate the name
        m_localName = new char[strlen(Name) + 1];

        //Copy the name
        strncpy(m_localName, Name, strlen(Name) + 1);

    }

}

LocalFolder::~LocalFolder()
{

    //Empty the list of child nodes
    emptyList();

    //If the path is non null delete it
    if (m_localFolderPath != 0)
    {

        delete[] m_localFolderPath;
        m_localFolderPath = 0;

    }

    //If the name is non-null delete it
    if (m_localName != 0)
    {
        delete[] m_localName;
        m_localName = 0;
    }

}

unsigned int LocalFolder::numChildren()
{

    return m_entryList.size();

}

FilesystemNode* LocalFolder::getChild(unsigned int n)
{
    if (m_entryList.size() > n)
        return m_entryList[n];
    else
        return 0;
}

File* LocalFolder::newChild(const char* Name)
{
    //If the name is invalid return 0
    if (Name == 0) return 0;

    //If it already exists bottle out return 0
    if (searchForChild(Name) != 0) return 0;

    //Generate the file path
    char* fullPath = new char[strlen(m_localFolderPath) + strlen(Name) + 1];

    strcpy(fullPath, m_localFolderPath);
    strcat(fullPath, Name);

    File* newFile = new LocalFile(fullPath);
    m_entryList.push_back( newFile );

    delete[] fullPath;

    return newFile;
}

Folder* LocalFolder::newChildFolder(const char* Name)
{

    //If the name is invalid return 0
    if (Name == 0) return 0;

    //If it already exists bottle out return 0
    if (searchForChild(Name) != 0) return 0;

    Folder* returnFolder= 0;

    //Generate the file path
    char* fullPath = new char[strlen(m_localFolderPath) + strlen(Name) + 2];

    strcpy(fullPath, m_localFolderPath);
    strcat(fullPath, Name);
    strcat(fullPath, "/");

    if (mkdir(fullPath, 0) == 0)
    {

        returnFolder = new LocalFolder(Name, fullPath);
        returnFolder->refresh();

        m_entryList.push_back(returnFolder);
    }

    delete[] fullPath;

    return returnFolder;
}

void LocalFolder::refresh()
{
    #warning May not be compatable with Windows 32
    emptyList();

    DIR* dir = opendir(m_localFolderPath);
    struct dirent* nextEntry;

    if (dir != 0)
    {

        while (true)
        {

            nextEntry = readdir(dir);

            if (nextEntry != 0)
            {
                //Check it isn't the current or parent indicators
                if (strcmp(nextEntry->d_name, ".") == 0)
                {

                }
                else if (strcmp(nextEntry->d_name, "..") == 0)
                {

                }
                else
                {
                    //if its not process it

                    if (nextEntry->d_type == DT_DIR)
                    {

                        //Directory

                        char* fullPath = new char[strlen(m_localFolderPath) + strlen(nextEntry->d_name) + 2];

                        strcpy(fullPath, m_localFolderPath);
                        strcat(fullPath, nextEntry->d_name);
                        strcat(fullPath, "/");

                        Folder* subFolder = new LocalFolder(nextEntry->d_name, fullPath);

                        subFolder->refresh();

                        m_entryList.push_back( subFolder );

                        delete[] fullPath;
                    }
                    else
                    {

                        //File
                        char* fullPath = new char[strlen(m_localFolderPath) + strlen(nextEntry->d_name) + 1];

                        strcpy(fullPath, m_localFolderPath);
                        strcat(fullPath, nextEntry->d_name);

                        m_entryList.push_back( new LocalFile(fullPath) );

                        delete[] fullPath;
                    }

                }

            }
            else
            {

                break;

            }

        }


        closedir(dir);
        dir = 0;
    }

}

void LocalFolder::emptyList()
{

    for (unsigned int i = 0; i < m_entryList.size(); ++i)
    {
        delete m_entryList[i];
    }

    m_entryList.clear();

}

FilesystemNode* LocalFolder::searchForChild(const char* Name, unsigned int nameLength)
{

    for (unsigned int i = 0; i < m_entryList.size(); ++i)
    {

        //If the nameLength is unset and the Name is non null use a strlen function for the comparison length
        if (nameLength == 0 && Name != 0)
        {
            nameLength = strlen(Name);
        }

        if (strncmp(m_entryList[i]->getName(), Name, nameLength) == 0)
        {

            return m_entryList[i];

        }

    }

    return 0;
}
