#include "FileSystem.hpp"
#include "File.hpp"
#include <iostream>

using namespace std;


FileSystem::FileSystem() {
    root = new Directory(nullptr, "/");
    root->createDirectory("test");
    Directory* test = root->createDirectory("test2");
    test->createDirectory("test3");
	freeBlockNumbers = vector<bool>(mMemblockDevice.size(), true);
}

FileSystem::~FileSystem() {
    delete root;
}

void FileSystem::ls() const {
    root->getDirectory("test2/")->ls();
}

void FileSystem::format() {
	mMemblockDevice.reset();
	freeBlockNumbers = vector<bool>(mMemblockDevice.size(), true);
}

void FileSystem::save(const std::string &saveFile) const{
	std::ofstream file;
	char buffer[512];
	int nrOfBlocks = mMemblockDevice.size();
	file.open(saveFile, std::ios::out|std::ios::binary);
	
	for (int i = 0; i < nrOfBlocks; i++){
		for (int j = 0; j < 512; j++){
			buffer[j] = mMemblockDevice.readBlock(i)[j];
		}
		file.write(buffer, 512);
	}
	file.close();
}

void FileSystem::load(const std::string &saveFile) {
	std::ifstream file;
	char buffer[512];
	int nrOfBlocks = mMemblockDevice.size();
	file.open(saveFile, std::ios::in | std::ios::binary);
	
	for (int i = 0; i < nrOfBlocks; i++){
		file.read(buffer, 512);
		mMemblockDevice.writeBlock(i, buffer);
	}
	file.close();
}

void FileSystem::create(const std::string &filePath){
	cout << "Enter something to put into the file: \n";
	string fileContent;
	cin >> fileContent;
	root->getDirectory(filePath);
}


void FileSystem::cat(std::string &fileName) const{
	if (root->getFile(fileName) == nullptr){
		cout << "404: File not found!\n";
	}
	int fileLength = root->getFile(fileName)->getLength();
	vector<int> tempNrs = root->getFile(fileName)->getBlockNumbers();
	//Read blocks and print characters until there is no more characters left or we have read a whole block (and should start to read a new block).
	for (int i = 0; i < tempNrs.size(); i++){
		for (int j = 0; j < fileLength || j < 512; j++){
			cout << mMemblockDevice[tempNrs[i]][j];
		}
		fileLength - 512;
	}
}