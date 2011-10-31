#ifndef _LOCAL_FOLDER_DEF_H_
#define _LOCAL_FOLDER_DEF_H_
#include "Folder.h"
#include <vector>
#include <string>
using namespace std;

class LocalFolder : public Folder
{
private:
	static const char* TAG;

    string m_localFolderPath;
    string m_localName;

    std::vector<FilesystemNode*> m_entryList;
    void emptyList();

public:

    LocalFolder(string Name, string Path);
    ~LocalFolder();

    unsigned int numChildren();
    FilesystemNode* getChild(unsigned int n);

    File* newChild(string filename);
    Folder* newChildFolder(string Name);

    void refresh();

    string getName() { return m_localName; }
    FilesystemNode* searchForChild(std::string Name);

};

#endif //_LOCAL_FOLDER_DEF_H_
