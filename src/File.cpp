#include "File.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

File::File(const string &name){
	this->length = 0;
	this->name = name;
	this->read = true;
	this->write = true;
}

void File::rename(const string &name) {
    this->name = name;
}

string File::getName() const {
	return this->name;
}

void File::setLength(const int length) {
	this->length = length;
}

int File::getLength() const {
	return this->length;
}

void File::setBlockNumbers(vector<int> blockNrs) {
	this->blockNumbers = blockNrs;
}

vector<int> File::getBlockNumbers() const{
	return this->blockNumbers;
}

void File::setPermission(Permission perm) {
	switch (perm){
	case NONE:
		read = false;
		write = false;
		break;
	case READ:
		read = true;
		write = false;
		break;
	case WRITE:
		read = false;
		write = true;
		break;
	case READWRITE:
		read = true;
		write = true;
		break;
	}
}

bool File::getWritePermission() const {
	return write;
}

bool File::getReadPermission() const {
	return read;
}

File::Permission File::getPermission() const {
	if ((read == false) && (write == false))
		return NONE;
	if ((read == true) && (write == false))
		return READ;
	if ((read == false) && (write == true))
		return WRITE;
	if ((read == true) && (write == true))
		return READWRITE;
}

void File::save(ofstream &file) {
    file.write(reinterpret_cast<const char*>(&read), sizeof(read));
    file.write(reinterpret_cast<const char*>(&write), sizeof(write));
    file.write(reinterpret_cast<const char*>(&length), sizeof(length));
    
    size_t blockLength = blockNumbers.size();
    file.write(reinterpret_cast<const char*>(&blockLength), sizeof(blockLength));
    
    for (int block : blockNumbers) {
        file.write(reinterpret_cast<const char*>(&block), sizeof(block));
    }
}

void File::load(ifstream &file) {
    file.read(reinterpret_cast<char*>(&read), sizeof(read));
    file.read(reinterpret_cast<char*>(&write), sizeof(write));
    file.read(reinterpret_cast<char*>(&length), sizeof(length));
    
    size_t blockLength;
    file.read(reinterpret_cast<char*>(&blockLength), sizeof(blockLength));
    
    for (int i=0; i<blockLength; i++) {
        int temp;
        file.read(reinterpret_cast<char*>(&temp), sizeof(temp));
        blockNumbers.push_back(temp);
    }
}

string File::toString() const {
    stringstream temp;
    temp << "    "
         << (read ? "R" : "-")
         << (write ? "W" : "-")
         << " "
         << setw(10) << length
         << " "
         << name
         << "\n";

	return temp.str();
}
