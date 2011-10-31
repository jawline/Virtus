#include "Filesystem.h"
#include <string.h>
#include <stdio.h>
#include "LocalFolder.h"

FilesystemNode* Filesystem::evaluateChild(Folder* Parent, string targetNode)
{
    size_t firstSlash = targetNode.find_first_of("/");

    if (firstSlash == string::npos)
    {

        //Pass it to the folder internal search
        return Parent->searchForChild( targetNode.c_str() );

    }
    else
    {

    	//Get the part of the string after the slash
        string splitPath = targetNode.substr(0, firstSlash);

        FilesystemNode* childNode = Parent->searchForChild( splitPath.c_str() );

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
                return Filesystem::evaluateChild(childFolder, targetNode.substr(firstSlash + 1, targetNode.length() - (firstSlash + 1)) );

            }
        }
        else
        {
            return 0;
        }

    }
}

FilesystemNode* Filesystem::search(std::string TargetFile) {

    return Filesystem::evaluateChild( Filesystem::getRoot(), TargetFile );

}

DataInputStream* Filesystem::getFileInputStream(std::string targetFile) {

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

DataOutputStream* Filesystem::getFileOutputStream(std::string targetFile) {

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

Folder* Filesystem::getRoot() {

    if (g_filesystemRoot == 0) {

        g_filesystemRoot = new LocalFolder("Root", "./");
        g_filesystemRoot->refresh();
    }

    return g_filesystemRoot;
}
