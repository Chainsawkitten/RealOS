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