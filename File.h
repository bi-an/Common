#pragma once
#include <string>
#include <fstream>
#include "Error.h"

using namespace std;

template<typename T>
class File {
public:
	File() {

	}
	File(string filename) {
		m_filename = filename;
	}
	File operator=(File file) {
		m_filename = file.m_filename;
		return *this;
	}
	File(const File& file) {
		*this = file;
	}
	~File() {

	}
	inline virtual bool NewFile() {
		return true;
	}
	inline virtual bool NewFile(string filename) {
		m_filename = filename;
		ofstream outfile;
		try {
			outfile.open(filename);
		}
		catch (Exception& e) {
			outfile.close();
			return false;
		}
		outfile.close();
		return true;
	}
	virtual bool OpenFile() {
		return true;
	}
	virtual bool OpenFile(string filename) {
		return true;
	}
	bool ReadFile() = 0;
	bool ReadFile(string filename) = 0;
	bool WriteFile() = 0;
	bool WriteFile(string filename) = 0;
private:
	string m_filename;
};

