#include "FileSystem.hpp"
#include "File.hpp"
#include <iostream>

using namespace std;

FileSystem::FileSystem() {
    root = new Directory("/");
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
    delete root;
    root = new Directory("/");
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
	File* file = root->createFile(filePart(filePath));
	cout << "Enter file contents: \n";
	string fileContent;
	cin >> fileContent;
	appendToFile(file, fileContent);
}

void FileSystem::append(const std::string &source, const std::string &app){
}

void FileSystem::appendToFile(File* file, string contents){
	vector<int> freeBlock = freeBlocks();
	vector<int> usedBlockNumbers;
	int bufferPos = 0;		//Keeps track of where we are in the buffer
	int freeBlockPos = 0;	//Keeps track of what free block number we are using
	int requiredBlocks = 0;	//Keeps track of how many blocks we need to allocate
	char buffer[512];		//Stores what we want to write to the block

	if (file->getBlockNumbers().size() > 1){
		//The file we are appending to is not a new file.
		//Recalculate file length
		int tempLength = file->getLength() + contents.length();
		//Calculate how many new blocks we need, if any.
		//If four total blocks required and 2 already allocated, required blocks will be 2.
		requiredBlocks = ceil(tempLength / 512.f) - file->getBlockNumbers().size();
		if (requiredBlocks > freeBlock.size()){
			cout << "Not enough free blocks.\n";
			return;
		}
		
		file->setLength(tempLength);

		//Null out buffer.
		for (int i = 0; i < 512; i++)
			buffer[i] = '\0';
		//read the last block of the file into the buffer
		//Size will never exceed 512.
		string tempString = mMemblockDevice.readBlock(file->getBlockNumbers().back()).toString();
		bufferPos = tempString.length();
		for (int i = 0; i < tempString.length(); i++){
			buffer[i] = tempString[i];
		}
	} else {
		requiredBlocks = ceil(contents.length() / 512.f);
		//Check to see if there are enough free blocks.
		if (requiredBlocks > freeBlock.size()){
			cout << "Not enough free blocks.\n";
			return;
		}
		file->setLength(contents.length());
		for (int i = 0; i < 512; i++)
			buffer[i] = '\0';
		bufferPos = 0;
		
	}
	
	for (int j = 0; j < file->getLength(); j++){
		if (bufferPos > 512) { //If pos is greater than 512, we need to write to next block.
			mMemblockDevice.writeBlock(freeBlock[freeBlockPos], buffer);
			freeBlockNumbers[freeBlock[freeBlockPos]] = false;
			usedBlockNumbers.push_back(freeBlock[freeBlockPos]);
			//Null out buffer.
			for (int i = 0; i < 512; i++)
				buffer[i] = '\0';
			bufferPos = 0;
			freeBlockPos++;
		}
		
		buffer[bufferPos] = contents[j];
		bufferPos++;
		
		if (bufferPos == contents.length()){	//If we have reached the end of the string and should write the contents to block.
			mMemblockDevice.writeBlock(freeBlock[freeBlockPos], buffer);
			freeBlockNumbers[freeBlock[freeBlockPos]] = false;
			usedBlockNumbers.push_back(freeBlock[freeBlockPos]);
		}
	}
	file->setBlockNumbers(usedBlockNumbers);
}

vector<int> FileSystem::freeBlocks() const{
	vector<int> blockList;
	for (int i = 0; i < freeBlockNumbers.size(); i++){
		if (freeBlockNumbers[i] == true)
			blockList.push_back(i);
	}
	return blockList;
}



void FileSystem::mkdir(const string &path) {
    if (filePart(path).empty()) {
        cout << "Wrong syntax." << endl;
        return;
    }
    
    Directory* directory = root->getDirectory(directoryPart(path));
    if (directory == nullptr) {
        cout << "Directory " << directoryPart(path) << " does not exist." << endl;
        return;
    }
    
    if (directory->getDirectory(filePart(path)) != nullptr) {
        cout << "Directory with that name already exists." << endl;
        return;
    }
    
    if (directory->getFile(filePart(path)) != nullptr) {
        cout << "File with same name already exists." << endl;
        return;
    }
    
    directory->createDirectory(filePart(path));
    
    cout << "Directory created." << endl;
}

void FileSystem::cat(std::string &fileName) const{
    Directory* directory = root->getDirectory(directoryPart(fileName));
    if (directory == nullptr) {
        cout << "File does not exist.\n" << endl;
        return;
    }
    
    File* file = directory->getFile(filePart(fileName));
	if (file == nullptr){
        cout << "File does not exist.\n" << endl;
        return;
	}
    
	//int fileLength = file->getLength();
	vector<int> tempNrs = file->getBlockNumbers();
	char buffer[512];
	int bufferPos = 0;
	//Read blocks and print characters until there is no more characters left or we have read a whole block (and should start to read a new block).
	for (int i = 0; i < tempNrs.size(); i++){
		while ( (bufferPos < 511) && (mMemblockDevice.readBlock(tempNrs[i])[bufferPos] != '\0') ){
			cout << mMemblockDevice.readBlock(tempNrs[i])[bufferPos];
			bufferPos++;
		}
		bufferPos = 0;
	}
	cout << '\n';
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
