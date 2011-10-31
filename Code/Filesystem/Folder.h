#ifndef _FILESYSTEM_FOLDER_DEF_H_
#define _FILESYSTEM_FOLDER_DEF_H_
#include "Node.h"
#include "File.h"

const int folderId = 2;

/**
 * @brief The abstraction of a folder within the filesystem
 *
 */
class Folder: public FilesystemNode {
public:

	virtual unsigned int numChildren() = 0;
	virtual FilesystemNode* getChild(unsigned int number) = 0;

	virtual void refresh() = 0;
	virtual File* newChild(string name) = 0;
	virtual Folder* newChildFolder(string name) = 0;
	virtual FilesystemNode* searchForChild(string name) = 0;

	int getType() {
		return folderId;
	}

	virtual ~Folder() {
	}

};

#endif //_FILESYSTEM_FOLDER_DEF_H_
