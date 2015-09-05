#include "File.hpp"
#include <iostream>

using namespace std;

File::File(const std::string &name){
	this->length = 0;
	this->name = name;
	this->read = true;
	this->write = true;
}

File::~File(){

}

string File::toString() const {
    /// @todo Permissions
    /// @todo File size

	int filesize = length / 8;
	return "      " + name + "\n";
}

string File::getName() const {
	return this->name;
}

int File::getLength() const {
	return this->length;
}

void File::setLength(const int length) {
	this->length = length;
}


vector<int> File::getBlockNumbers() const{
	return this->blockNumbers;
}

void File::setBlockNumbers(std::vector<int> blockNrs){
	this->blockNumbers = blockNrs;
}

bool File::getWritePermission(){
	return write;
}

bool File::getReadPermission() const{
	return read;
}

void File::setPermission(Permission perm){
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

Permission File::getPermission() const{
	if ((read == false) && (write == false))
		return NONE;
	if ((read == true) && (write == false))
		return READ;
	if ((read == false) && (write == true))
		return WRITE;
	if ((read == true) && (write == true))
		return READWRITE;
}