#include "File.hpp"
#include <iostream>

using namespace std;

string File::toString() const {
	string tempstring;
	tempstring += "FILE " + this->name + '\n';
	tempstring += "R:" + this->read + '\n';
	tempstring += "W:" + this->write + '\n';
	tempstring += "BlockNrs: " + '\n';
	for (int i = 0; i < blockNumbers.size(); i++){
		tempstring += "B: "+ blockNumbers[i] + '\n';
	}
}