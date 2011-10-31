#include "LocalOutputStream.h"

LocalDataOutputStream::LocalDataOutputStream(string Path) {

	//Allocate memory and copy the file path
	m_filePath = Path;

	//Null the fileOutputStream pointer
	m_fileOutputStream = 0;
}

LocalDataOutputStream::~LocalDataOutputStream() {

	//If the stream is still open
	if (isOpen() == true) {
		//Close it
		close();

	}

}

//Returns true if the output stream is already open
bool LocalDataOutputStream::isOpen() {

	//If there is a fileOutputStream
	if (m_fileOutputStream != 0) {
		//return true, the stream is open
		return true;

	}

	//Otherwise return false
	return false;
}

//Attempt to open a output stream for the dataoutputstream class
bool LocalDataOutputStream::open(bool textMode) {

	//If the file path is empty return false
	if (m_filePath.length() == 0) {
		return false;
	}

	//if the stream is already open return false
	if (isOpen() == true) {
		return false;
	}

	//Attempt to open the file
	if (textMode == true) {

		printf("Opening in text mode\n");

		m_fileOutputStream = fopen(m_filePath.c_str(), "w");

	} else {
		printf("Open in binary write mode\n");

		m_fileOutputStream = fopen(m_filePath.c_str(), "wb");

	}

	//If the open was good then return true
	if (m_fileOutputStream != 0)
		return true;

	//Otherwise return false
	return false;
}

void LocalDataOutputStream::close() {

	if (isOpen() == true) {

		fclose(m_fileOutputStream);
		m_fileOutputStream = 0;

	}

}

//Attempt to write specified data to the stream
bool LocalDataOutputStream::write(const void* Data, unsigned long size) {

	if (isOpen() == true) {

		if (fwrite(Data, size, 1, m_fileOutputStream) != 1) {
			return false;
		}

		return true;

	} else {

		return false;

	}

}

void LocalDataOutputStream::flush() {

	if (isOpen() == true) {
		fflush(m_fileOutputStream);
	}

}
