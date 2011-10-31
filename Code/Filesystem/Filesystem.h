#ifndef _VIRTUAL_FILESYSTEM_HEADER_DEF_H_
#define _VIRTUAL_FILESYSTEM_HEADER_DEF_H_
#include "Folder.h"
#include "File.h"
#include "Node.h"

#include <string>
using namespace std;

/**
 * Filesystem stores the root of the filesystem and the utility functions to access input and output streams
 */

class Filesystem
{
private:

public:

    /**
     * Search for the child node from the specified root
     * @param Folder* Parent the parent/root of the search (Usually Filesystem::getRoot())
     * @param const char* TargetFile the file
     * @return FilesystemNode* The node found, or null if it's not found
     */

    static FilesystemNode* evaluateChild(Folder* Target, string targetFile);

    /**
     * Search for a child node in the filesystem
     * @param TargetPath
     * @return FilesystemNode* The node found, or null if it's not found
     */

     static FilesystemNode* search(std::string targetFile);

    /**
     * Open a input stream to the specified file
     * @param Folder* searchRoot The root of the search
     * @param const char* TargetFile the file to open a input stream to
     * @return DataInputStream* The input stream to the file - has not been opened
     */

    static DataInputStream* getFileInputStream(std::string targetFile);

    /**
     * Open a output stream to the specified file
     * @param Folder* searchRoot The root of the search
     * @param const char* TargetFile the file to open a output stream to
     * @return DataInputStream* The output stream to the file - has not been opened
     */

    static DataOutputStream* getFileOutputStream(std::string targetFile);

    /**
     * Get the root of the filesystem
     * @return Folder* root of the FS
     */

    static Folder* getRoot();
};

#endif //_VIRTUAL_FILESYSTEM_HEADER_DEF_H_
