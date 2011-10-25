#ifndef _LOCAL_FOLDER_DEF_H_
#define _LOCAL_FOLDER_DEF_H_
#include "Folder.h"
#include <vector>

class LocalFolder : public Folder
{
private:
    char* m_localFolderPath;
    char* m_localName;

    std::vector<FilesystemNode*> m_entryList;
    void emptyList();

public:

    LocalFolder(const char* Name, const char* Path);
    ~LocalFolder();

    unsigned int numChildren();
    FilesystemNode* getChild(unsigned int n);

    File* newChild(const char* filename);
    Folder* newChildFolder(const char* Name);

    void refresh();

    char* getName() { return m_localName; }
    FilesystemNode* searchForChild(const char* Name, unsigned int nameLength = 0);

};

#endif //_LOCAL_FOLDER_DEF_H_
