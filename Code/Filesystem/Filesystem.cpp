#include "Filesystem.h"
#include <string.h>
#include <stdio.h>
#include "LocalFolder.h"

FilesystemNode* Filesystem::evaluateChild(Folder* Parent, const char* targetNode)
{
    const char* firstSlash = strchr( targetNode , '/');

    if (firstSlash == 0)
    {
        //Pass it to the folder internal search
        return Parent->searchForChild( targetNode );
    }
    else
    {
        size_t entryLength = firstSlash - targetNode;

        FilesystemNode* childNode = Parent->searchForChild( targetNode , entryLength);

        if (childNode != 0)
        {

            if (childNode->getType() != folderId)
            {

                //Not a folder, can't be searched
                return 0;

            }
            else
            {

                Folder* childFolder = static_cast<Folder*>(childNode);
                return Filesystem::evaluateChild(childFolder, firstSlash + 1);

            }
        }
        else
        {
            return 0;
        }

    }
}

FilesystemNode* Filesystem::search(const char* TargetFile) {
    return Filesystem::evaluateChild( Filesystem::getRoot(), TargetFile );
}

DataInputStream* Filesystem::getFileInputStream(const char* targetFile) {

    //Find the node
    FilesystemNode* node = Filesystem::search(targetFile);

    //If it failed return null
    if (!node) {

        return 0;
    }

    //Check the file type
    if (node->getType() != fileId) {
        return 0;
    }

    //Convert it to a file
    File* toFileNode = static_cast<File*>(node);

    return toFileNode->createInputStream();
}

DataOutputStream* Filesystem::getFileOutputStream(const char* targetFile) {

    //Find the node
    FilesystemNode* node = Filesystem::search(targetFile);

    //If it failed return null
    if (!node) {

        return 0;
    }

    //Check the file type
    if (node->getType() != fileId) {
        return 0;
    }

    //Convert it to a file
    File* toFileNode = static_cast<File*>(node);

    return toFileNode->createOutputStream();
}

static Folder* g_filesystemRoot = 0;

Folder* Filesystem::getRoot()
{
    if (g_filesystemRoot == 0)
    {
        g_filesystemRoot = new LocalFolder("Root", "./");
        g_filesystemRoot->refresh();
    }

    return g_filesystemRoot;
}
