#include "LocalFile.h"
#include <stdio.h>
#include <string.h>
#include "LocalOutputStream.h"
#include "LocalInputStream.h"
#include <sys/stat.h>

LocalFile::LocalFile(string Path) {

	m_localFilePath = Path;

	size_t lastSlash = Path.find_last_of("/");

	if (lastSlash == string::npos) {

		lastSlash = 0;

	} else {

		//Increment by 1 to remove the / from the string
		lastSlash++;

	}

	//Get the part of the path after the last forward slash (I.e. the file name)
	m_localName = Path.substr(lastSlash);

	printf("m_localName %s\n", m_localName.c_str());
}

LocalFile::~LocalFile() {
	return;
}

DataInputStream* LocalFile::createInputStream() {
	return new LocalDataInputStream(m_localFilePath);
}

DataOutputStream* LocalFile::createOutputStream() {
	return new LocalDataOutputStream(m_localFilePath);
}

bool LocalFile::exists() {

	struct stat fileInfo;

	//If stat returns 0 something by that name exists
	if (stat(m_localFilePath.c_str(), &fileInfo) == 0)
		return true;

	return false;
}

unsigned long LocalFile::length() {

	struct stat fileInfo;

	//If stat returns 0 something by that name exists
	if (stat(m_localFilePath.c_str(), &fileInfo) == 0) {

		return fileInfo.st_size;

	}

	return 0;
}

string LocalFile::getName() {
	return m_localName;
}
