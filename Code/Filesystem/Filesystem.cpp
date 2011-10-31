#include "Filesystem.h"
#include <string.h>
#include <stdio.h>
#include "LocalFolder.h"

FilesystemNode* Filesystem::evaluateChild(Folder* parent, string path) {

	/*
	 * General plan (Evaluating a path such as ./Data/Folder/ or ./Data/Folder/Stuff.txt)
	 *
	 * Check if the path is empty, if it is exit
	 * Find the location of the first forward slash
	 * Get the strings on the left and right of the forward slash (E.g. Data and File/Dog/Tootoo.avi)
	 * Get the strings to the left and right of the forward slash
	 * Find the node named by the left side
	 * Recurse, using the node on the left, eventually the path will have no slashes left in it and be passed to the node->search function a few steps above
	 */

	//Check the path isn't empty
	if (path.empty()) {
		return 0;
	}

	//Find the location of the first forward slash
	size_t firstForwardSlash = path.find_first_of('/');

	//If there are no forward slashes in the path
	if (firstForwardSlash == string::npos) {

		//Pass it to the folder internal search
		return parent->searchForChild(path);

	}

	//Get the part of the string before the forward slash
	string leftHandSide = path.substr(0, firstForwardSlash);

	//Try and find that as a node
	FilesystemNode* childNode = parent->searchForChild(leftHandSide);

	//If its not found or isn't a folder
	if ( /* IF NOT */ ! ( childNode != 0 && childNode->getType() == folderId) ) {
		//Return failure
		return 0;
	}

	//Find the right hand side
	string rightHandSide = path.substr(firstForwardSlash + 1);

	//If there is nothing on the right hand side it is something like ./Data/Log/, in this case just return the parent node
	if ( rightHandSide.empty() ) {
		return childNode;
	}

	//Recurse, using a cast of childNode as a folder and the substring of path right of the first forward slash
	return Filesystem::evaluateChild( static_cast<Folder*> (childNode), path.substr(firstForwardSlash + 1));
}

FilesystemNode* Filesystem::search(std::string TargetFile) {

	return Filesystem::evaluateChild(Filesystem::getRoot(), TargetFile);

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
	File* toFileNode = static_cast<File*> (node);

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
	File* toFileNode = static_cast<File*> (node);

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
