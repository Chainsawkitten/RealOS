#include "File.hpp"
#include <iostream>

using namespace std;

File::File(const std::string &name){
	this->length = 0;
	this->name = name;
}

File::~File(){

}

string File::toString() const {
	string tempstring;
	tempstring += "FILE: " + this->name + '\n';
	tempstring += "R:" + this->read + '\n';
	tempstring += "W:" + this->write + '\n';
	tempstring += "Length:" + this->length + 'n';
	tempstring += "BlockNrs: " + '\n';
	for (int i = 0; i < blockNumbers.size(); i++){
		tempstring += "B: "+ blockNumbers[i] + '\n';
	}
	return tempstring;
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