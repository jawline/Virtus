#include "LocalFolder.h"
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include "LocalFile.h"

LocalFolder::LocalFolder(string name, string path) {
	m_localFolderPath = path;
	m_localName = name;
}

LocalFolder::~LocalFolder() {

	//Empty the list of child nodes
	emptyList();

}

unsigned int LocalFolder::numChildren() {

	return m_entryList.size();

}

FilesystemNode* LocalFolder::getChild(unsigned int n) {
	if (m_entryList.size() > n)
		return m_entryList[n];
	else
		return 0;
}

File* LocalFolder::newChild(string Name) {

	//If the name is invalid return 0
	if (Name.empty()) {
		return 0;
	}

	//If it already exists bottle out return 0
	if (searchForChild(Name) != 0) {
		return 0;
	}

	//Generate the file path
	string fullPath = m_localFolderPath + Name;

	File* newFile = new LocalFile(fullPath);
	m_entryList.push_back(newFile);

	return newFile;
}

Folder* LocalFolder::newChildFolder(string Name) {

	//If the name is invalid return 0
	if (Name.empty())
		return 0;

	//If it already exists bottle out return 0
	if (searchForChild(Name) != 0)
		return 0;

	Folder* returnFolder = 0;

	//Generate the file path
	string fullPath = m_localFolderPath + Name + "/";

	if (mkdir(fullPath.c_str(), 0) == 0) {

		returnFolder = new LocalFolder(Name, fullPath);
		returnFolder->refresh();

		m_entryList.push_back(returnFolder);
	}

	return returnFolder;
}

void LocalFolder::refresh() {
#warning May not be compatable with Windows 32
	emptyList();

	DIR* dir = opendir(m_localFolderPath.c_str());
	struct dirent* nextEntry;

	if (dir != 0) {

		while (true) {

			nextEntry = readdir(dir);

			if (nextEntry != 0) {
				//Check it isn't the current or parent indicators
				if (strcmp(nextEntry->d_name, ".") == 0) {
					//Do nothing for this dir
				} else if (strcmp(nextEntry->d_name, "..") == 0) {
					//Do nothing for the dir the level above
				} else {
					//if its not process it

					if (nextEntry->d_type == DT_DIR) {

						//Directory
						string fullPath = m_localFolderPath + nextEntry->d_name
								+ "/";

						Folder* subFolder = new LocalFolder(nextEntry->d_name,
								fullPath);

						subFolder->refresh();

						m_entryList.push_back(subFolder);

					} else {

						//File
						string fullPath = m_localFolderPath + nextEntry->d_name;

						m_entryList.push_back(new LocalFile(fullPath));
					}

				}

			} else {

				break;

			}

		}

		closedir(dir);
		dir = 0;
	}

}

void LocalFolder::emptyList() {

	for (unsigned int i = 0; i < m_entryList.size(); ++i) {
		delete m_entryList[i];
	}

	m_entryList.clear();

}

FilesystemNode* LocalFolder::searchForChild(std::string name) {

	printf("Search for %s\n", name.c_str());

	for (unsigned int i = 0; i < m_entryList.size(); ++i) {

		if ( name.compare(m_entryList[i]->getName()) == 0) {

			printf("Found\n");

			return m_entryList[i];

		}

	}

	printf("ERROR: Child not found\n");

	return 0;
}
