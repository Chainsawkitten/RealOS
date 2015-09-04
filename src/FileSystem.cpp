#include "FileSystem.hpp"
#include "File.hpp"
#include <iostream>

using namespace std;

FileSystem::FileSystem() {
    root = new Directory("/");
    root->createDirectory("test");
    Directory* test = root->createDirectory("test2");
    test->createDirectory("test3");
	freeBlockNumbers = vector<bool>(mMemblockDevice.size(), true);
}

FileSystem::~FileSystem() {
    delete root;
}

void FileSystem::ls(const std::string &path) const {
    Directory* directory = root->getDirectory(path);
    if (directory == nullptr) {
        cout << "Directory does not exist." << endl;
        return;
    }
    
    directory->ls();
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
    Directory* directory = root->getDirectory(directoryPart(fileName));
    if (directory == nullptr) {
        cout << "File does not exist." << endl;
        return;
    }
    
    File* file = directory->getFile(filePart(fileName));
	if (file == nullptr){
        cout << "File does not exist." << endl;
        return;
	}
    
	int fileLength = file->getLength();
	vector<int> tempNrs = file->getBlockNumbers();
	//Read blocks and print characters until there is no more characters left or we have read a whole block (and should start to read a new block).
	for (int i = 0; i < tempNrs.size(); i++){
		for (int j = 0; j < fileLength || j < 512; j++){
			cout << mMemblockDevice[tempNrs[i]][j];
		}
		fileLength - 512;
	}
}

bool FileSystem::directoryExists(const string &path) {
    return root->getDirectory(path) != nullptr;
}

string FileSystem::directoryPart(const string &path) {
    size_t pos = path.find_last_of('/');
    if (pos == string::npos)
        return "";
    
    return path.substr(0, pos);
}

string FileSystem::filePart(const string &path) {
    size_t pos = path.find_last_of('/');
    if (pos == string::npos)
        return path;
    
    if (path.length() < pos + 2)
        return "";
    
    return path.substr(pos + 1);
}
