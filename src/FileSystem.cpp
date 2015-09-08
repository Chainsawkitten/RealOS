#include "FileSystem.hpp"
#include "File.hpp"
#include <iostream>
#include <cmath>

using namespace std;

FileSystem::FileSystem() {
    root = new Directory("/");
	mMemblockDevice = MemBlockDevice(250, 512);
	freeBlockNumbers = vector<bool>(mMemblockDevice.size(), true);
}

FileSystem::~FileSystem() {
    delete root;
}

void FileSystem::format() {
	mMemblockDevice.reset();
	freeBlockNumbers = vector<bool>(mMemblockDevice.size(), true);
    delete root;
    root = new Directory("/");
}

void FileSystem::ls(const std::string &path) const {
    Directory* directory = root->getDirectory(path);
    if (directory == nullptr) {
        cout << "Directory does not exist." << endl;
        return;
    }
    
    directory->ls();
}

void FileSystem::create(const std::string &filePath) {
	if (fileOrDirectoryExists(filePath)) {
		cout << "File or directory already exists.";
		return;
	}
	cout << "Enter file contents: \n";
	string fileContent;
	getline(cin, fileContent);

	int requiredBlocks = ceil(fileContent.length() / (float)mMemblockDevice.getBlockLength());
	vector<int> freeBlock = freeBlocks();
	if (requiredBlocks > freeBlock.size()) {
		cout << "Not enough free blocks to save string." << endl;
		return;
	}

    Directory* directory = root->getDirectory(directoryPart(filePath));
	File* file = directory->createFile(filePart(filePath));

	appendToFile(file, fileContent);
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
    
    if (fileOrDirectoryExists(path)) {
        cout << "File or directory with that name already exists." << endl;
		return;
    }

    directory->createDirectory(filePart(path));
    
    cout << "Directory created." << endl;
}

void FileSystem::cat(const std::string &fileName) const {
    Directory* directory = root->getDirectory(directoryPart(fileName));
    if (directory == nullptr) {
        cout << "File does not exist." << endl;
        return;
    }
    File* file = directory->getFile(filePart(fileName));
	if (file == nullptr) {
        cout << "File does not exist." << endl;
        return;
	} else if (!file->getReadPermission()) {
		cout << "File is read protected." << endl;
		return;
	}
    
    cout << fileToString(fileName) << endl;
}

void FileSystem::save(const std::string &saveFile) const {
	ofstream file;
	int nrOfBlocks = mMemblockDevice.size();
	int nrOfElements = mMemblockDevice.getBlockLength();
	char* buffer = new char[nrOfElements];
	file.open(saveFile, ios::out|ios::binary);
	
	for (int i = 0; i < nrOfBlocks; i++) {
		for (int j = 0; j < nrOfElements; j++) {
			buffer[j] = mMemblockDevice.readBlock(i)[j];
		}
		file.write(buffer, nrOfElements);
	}
    
    delete[] buffer;
    
    root->save(file);
    
	file.close();
}

void FileSystem::load(const std::string &saveFile) {
    format();
    
	ifstream file;
	int nrOfBlocks = mMemblockDevice.size();
	int nrOfElements = mMemblockDevice.getBlockLength();
	char* buffer = new char[nrOfElements];
	file.open(saveFile, ios::in | ios::binary);
	
	for (int i = 0; i < nrOfBlocks; i++) {
		file.read(buffer, nrOfElements);
		mMemblockDevice.writeBlock(i, buffer);
	}
    
    delete[] buffer;
    
    root->load(file);
    
	file.close();
}

void FileSystem::rm(const std::string &path) {
	if (!fileExists(path))
		return;
	Directory* directory = root->getDirectory(directoryPart(path));
	File* file = directory->getFile(filePart(path));

	if (!file->getWritePermission()) {
		cout << "File is write protected." << endl;
		return;
	}

	for (int i = 0; i < file->getBlockNumbers().size(); i++)
		freeBlockNumbers[file->getBlockNumbers()[i]] = true;
	
	delete file;
	directory->rm(filePart(path));
}

void FileSystem::copy(const std::string &source, const std::string &dest) {
	if (!fileExists(source)) {
		cout << "Can't copy file, file does not exist." << endl;
		return;
	}
	if (fileOrDirectoryExists(dest)) {
		cout << "Can't copy file, destination already exists." << endl;
	}
	Directory* directory = root->getDirectory(directoryPart(source));
	File* sourceFile = directory->getFile(filePart(source));
    if (!sourceFile->getReadPermission()) {
		cout << "File is read protected." << endl;
        return;
    }
    
    Directory* destinationDirectory = root->getDirectory(directoryPart(dest));
    if (destinationDirectory == nullptr) {
        cout << "Destination directory does not exist." << endl;
        return;
    }
    
	File* destinationFile = destinationDirectory->createFile(filePart(dest));
	string contents = fileToString(source);
	appendToFile(destinationFile, contents);
}

void FileSystem::append(const std::string &source, const std::string &destination) {
	if (fileExists(destination) && fileExists(source)) {
		Directory* destinationDirectory = root->getDirectory(directoryPart(destination));
		Directory* sourceDirectory = root->getDirectory(directoryPart(source));
		File* destinationFile = destinationDirectory->getFile(filePart(destination));
		File* sourceFile = sourceDirectory->getFile(filePart(source));

		if (sourceFile->getReadPermission() && destinationFile->getWritePermission()) {
			string appendString = fileToString(source);
			cout << appendString << " will be added." << endl;
			appendToFile(destinationFile, appendString);
		} else {
			cout << "Files did not have proper read/write permission." << endl;
		}
	} else {
		cout << "File(s) were not found." << endl;
	}
}

void FileSystem::rename(const string &source, const string &newName) {
    if (newName.empty()) {
        cout << "New name can't be empty." << endl;
        return;
    }
    
    if (!fileExists(source)) {
        cout << "Can't rename file, source does not exist." << endl;
        return;
    }
    
    if (fileOrDirectoryExists(newName)) {
        cout << "Destination already exists." << endl;
        return;
    }
    
    Directory* sourceDirectory = root->getDirectory(directoryPart(source));
    Directory* destinationDirectory = root->getDirectory(directoryPart(newName));
    
    if (destinationDirectory == nullptr) {
        cout << "Destination directory does not exist." << endl;
        return;
    }
    
    File* file = sourceDirectory->getFile(filePart(source));
    if (!file->getWritePermission()) {
        cout << "File is write protected." << endl;
        return;
    }
    
    sourceDirectory->rm(filePart(source));
    file->rename(newName);
    destinationDirectory->addFile(file);
}

void FileSystem::chmod(const std::string &path, int permission) {
	Directory* directory = root->getDirectory(directoryPart(path));
    if (directory == nullptr) {
        cout << "File does not exist." << endl;
        return;
    }
    
	File* file = directory->getFile(filePart(path));
    if (file == nullptr) {
        cout << "File does not exist." << endl;
        return;
    }
    
	if (permission > 4 || permission < 0){
		cout << "Invalid permission level. Valid permission levels:\n";
		cout << "0 = NONE.\n";
		cout << "1 = READ only.\n";
		cout << "2 = WRITE only.\n";
		cout << "3 = READ and WRITE permission.\n";
        return;
	}
    
	file->setPermission(File::Permission(permission));
}

bool FileSystem::directoryExists(const string &path) {
    return root->getDirectory(path) != nullptr;
}

string FileSystem::fileToString(const std::string &path) const {
	Directory* directory = root->getDirectory(directoryPart(path));
	File* file = directory->getFile(filePart(path));
	vector<int> tempNrs = file->getBlockNumbers();
	string contents;
	int bufferPos = 0;
    
	// Read blocks and print characters until there is no more characters left or we have read a whole block (and should start to read a new block).
	for (int i = 0; i < tempNrs.size(); i++){
		while ((bufferPos < (mMemblockDevice.getBlockLength())) && (mMemblockDevice.readBlock(tempNrs[i])[bufferPos] != '\0')){
			contents += mMemblockDevice.readBlock(tempNrs[i])[bufferPos];
			bufferPos++;
		}
		bufferPos = 0;
	}
    
	return contents;
}

void FileSystem::appendToFile(File* file, string contents) {
	vector<int> freeBlock = freeBlocks();
	vector<int> usedBlockNumbers;
	int bufferPos = 0;												// Keeps track of where we are in the buffer
	int freeBlockPos = 0;											// Keeps track of what free block number we are using
	int requiredBlocks = 0;											// Keeps track of how many blocks we may need to allocate
	char* buffer = new char[mMemblockDevice.getBlockLength()];		// Stores what we want to write to the block

	if (file->getBlockNumbers().size() > 0) {
		//The file we are appending to is not a new file.
		//Recalculate file length
		int tempLength = file->getLength() + contents.length();
		//Calculate how many new blocks we need, if any.
		requiredBlocks = ceil(tempLength / (float)mMemblockDevice.getBlockLength()) - file->getBlockNumbers().size();
		if (requiredBlocks > freeBlock.size()) {
			cout << "Not enough free blocks.\n";
			return;
		}
		file->setLength(tempLength);
		// Null out buffer.
		for (int i = 0; i < mMemblockDevice.getBlockLength(); i++)
			buffer[i] = '\0';
		// Read the last block of the file into the buffer.
		char c;
		while ((bufferPos < ((float)mMemblockDevice.getBlockLength())) && ((c = mMemblockDevice.readBlock(file->getBlockNumbers().back())[bufferPos]) != '\0')) {
			buffer[bufferPos] = c;
			bufferPos++;
		}
		usedBlockNumbers = file->getBlockNumbers();
		usedBlockNumbers.pop_back();
		// Insert the files last block number at the start of freeBlock,
		// indicating that if we need a new block then we should write the buffer
		// to the files last block.
		freeBlock.insert(freeBlock.begin(), file->getBlockNumbers().back());
		if (bufferPos == (mMemblockDevice.getBlockLength())) { // If pos is greater than block length, we need to write to next block.
			mMemblockDevice.writeBlock(freeBlock[freeBlockPos], buffer);
			freeBlockNumbers[freeBlock[freeBlockPos]] = false;
			usedBlockNumbers.push_back(freeBlock[freeBlockPos]);
			bufferPos = 0;
			freeBlockPos++;
		}
	} else {
		requiredBlocks = ceil(contents.length() / (float)mMemblockDevice.getBlockLength());
		file->setLength(contents.length());
		for (int i = 0; i < mMemblockDevice.getBlockLength(); i++)
			buffer[i] = '\0';
		bufferPos = 0;
		
	}

	for (int j = 0; j < contents.length(); j++) {
		buffer[bufferPos] = contents[j];
		bufferPos++;
		if (j == (contents.length()-1)) { // If we have reached the end of the string and should write the contents to block.
			mMemblockDevice.writeBlock(freeBlock[freeBlockPos], buffer);
			freeBlockNumbers[freeBlock[freeBlockPos]] = false;
			usedBlockNumbers.push_back(freeBlock[freeBlockPos]);
		} else if (bufferPos == (mMemblockDevice.getBlockLength())) { // If pos is greater than block length, we need to write to next block.
			mMemblockDevice.writeBlock(freeBlock[freeBlockPos], buffer);
			freeBlockNumbers[freeBlock[freeBlockPos]] = false;
			usedBlockNumbers.push_back(freeBlock[freeBlockPos]);
			for (int i = 0; i < mMemblockDevice.getBlockLength(); i++)
				buffer[i] = '\0';
			bufferPos = 0;
			freeBlockPos++;
		}
	}
    
    delete[] buffer;
	freeBlocks();
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

bool FileSystem::fileOrDirectoryExists(const string &path) const {
	Directory* directory = root->getDirectory(directoryPart(path));
	if (directory->getDirectory(filePart(path)) != nullptr) {
		cout << "Directory with that name already exists." << endl;
		return true;
	}

	if (directory->getFile(filePart(path)) != nullptr) {
		cout << "File with same name already exists." << endl;
		return true;
	}
	return false;
}

bool FileSystem::fileExists(const string &path) const {
	Directory* directory = root->getDirectory(directoryPart(path));
    if (directory == nullptr)
        return false;
    
	return (directory->getFile(filePart(path)) != nullptr);
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
