#include "LocalInputStream.h"

LocalDataInputStream::LocalDataInputStream(string filePath) {
	m_localDataFilePath = filePath;

	m_fileInputStream = 0;

}

LocalDataInputStream::~LocalDataInputStream() {

	if (isOpen()) {
		close();
	}

}

bool LocalDataInputStream::read(void* target, unsigned long amount) {
	if (isOpen() == false) {

		return false;

	}

	if (fread(target, amount, 1, m_fileInputStream) != 1) {
		return false;
	}

	return true;
}

void LocalDataInputStream::close() {

	if (isOpen()) {

		fclose(m_fileInputStream);
		m_fileInputStream = 0;

	}

}

bool LocalDataInputStream::open(bool textMode) {

	//Check if it is already open
	if (isOpen()) {
		return false;
	}

	//Check the path length > 0
	if (m_localDataFilePath.length() == 0) {
		return false;
	}

	if (textMode) {
		m_fileInputStream = fopen(m_localDataFilePath.c_str(), "r");
	} else {
		m_fileInputStream = fopen(m_localDataFilePath.c_str(), "rb");
	}

	if (m_fileInputStream != 0) {
		return true;
	}

	return false;
}

bool LocalDataInputStream::isOpen() {

	return (m_fileInputStream != 0);

}

bool LocalDataInputStream::skip(unsigned long bytes) {

	//Check the file is open
	if (isOpen() == false)
		return false;

	if (fseek(m_fileInputStream, bytes, SEEK_CUR) != 0) {
		//Error seeking, return a bad result
		return false;
	}

	//Otherwise it seeked fine
	return true;
}

bool LocalDataInputStream::rewind(unsigned long bytes) {
	//Check if the file is open
	if (isOpen() == false)
		return false;

	if (fseek(m_fileInputStream, -bytes, SEEK_CUR) != 0) {
		return false;
	}

	return true;
}
