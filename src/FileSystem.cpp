#include "FileSystem.hpp"
#include "File.hpp"
#include <cmath>


FileSystem::FileSystem() {
    root = new Directory("/");
	mMemblockDevice = MemBlockDevice(250, 512);
	format();
}

FileSystem::~FileSystem() {
    delete root;
}

void FileSystem::format() {
	mMemblockDevice.reset();
	freeBlockNumbers = std::vector<bool>(mMemblockDevice.size(), true);
    delete root;
    root = new Directory("/");
}

std::string FileSystem::ls(const std::string &path) const {
    Directory* directory = root->getDirectory(path);
    if (directory == nullptr)
        return "Directory does not exist.\n";
    
    return directory->ls();
}

std::string FileSystem::create(const std::string &filePath, const std::string &fileContent) {
	if (fileOrDirectoryExists(filePath))
		return "File or directory already exists.\n";


	int requiredBlocks = ceil(fileContent.length() / (float)mMemblockDevice.getBlockLength());
	std::vector<int> freeBlock = freeBlocks();
	if (requiredBlocks > freeBlock.size())
		return "Not enough free blocks to save string.\n";

    Directory* directory = root->getDirectory(directoryPart(filePath));
	File* file = directory->createFile(filePart(filePath));

	return appendToFile(file, fileContent);
}

std::string FileSystem::mkdir(const std::string &path) {
    if (filePart(path).empty())
        return "Wrong syntax.\n";
    
    Directory* directory = root->getDirectory(directoryPart(path));
    if (directory == nullptr)
        return "Directory " + directoryPart(path) + " does not exist.\n";
    
    if (fileOrDirectoryExists(path))
        return "File or directory with that name already exists.\n";

    directory->createDirectory(filePart(path));
    
    return "Directory created.\n";
}

std::string FileSystem::cat(const std::string &fileName) const {
    Directory* directory = root->getDirectory(directoryPart(fileName));
    if (directory == nullptr)
        return "File does not exist.\n";
    
    File* file = directory->getFile(filePart(fileName));
	if (file == nullptr) {
        return "File does not exist.\n";
	} else if (!file->getReadPermission()) {
		return "File is read protected.\n";
	}
    
    return fileToString(fileName) + "\n";
}

void FileSystem::save(const std::string &saveFile) const {
	std::ofstream file;
	int nrOfBlocks = mMemblockDevice.size();
	int nrOfElements = mMemblockDevice.getBlockLength();
	char* buffer = new char[nrOfElements];
	file.open(saveFile, std::ios::out|std::ios::binary);
	
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
    
	std::ifstream file;
	int nrOfBlocks = mMemblockDevice.size();
	int nrOfElements = mMemblockDevice.getBlockLength();
	char* buffer = new char[nrOfElements];
	file.open(saveFile, std::ios::in | std::ios::binary);
	
	for (int i = 0; i < nrOfBlocks; i++) {
		file.read(buffer, nrOfElements);
		mMemblockDevice.writeBlock(i, buffer);
	}
    
    delete[] buffer;
    
    root->load(file);
    
	file.close();
}

std::string FileSystem::rm(const std::string &path) {
	if (!fileExists(path))
		return "File does not exist.\n";
    
	Directory* directory = root->getDirectory(directoryPart(path));
	File* file = directory->getFile(filePart(path));

	if (!file->getWritePermission())
		return "File is write protected.\n";

	for (int i = 0; i < file->getBlockNumbers().size(); i++)
		freeBlockNumbers[file->getBlockNumbers()[i]] = true;
	
	delete file;
	directory->rm(filePart(path));
    
    return "";
}

std::string FileSystem::copy(const std::string &source, const std::string &dest) {
	if (!fileExists(source))
		return "Can't copy file, file does not exist.\n";
	
	if (fileOrDirectoryExists(dest))
		return "Can't copy file, destination already exists.\n";
	
	Directory* directory = root->getDirectory(directoryPart(source));
	File* sourceFile = directory->getFile(filePart(source));
    if (!sourceFile->getReadPermission())
		return "File is read protected.\n";
    
    Directory* destinationDirectory = root->getDirectory(directoryPart(dest));
    if (destinationDirectory == nullptr)
        return "Destination directory does not exist.\n";
    
	File* destinationFile = destinationDirectory->createFile(filePart(dest));
	std::string contents = fileToString(source);
	return appendToFile(destinationFile, contents);
}

std::string FileSystem::append(const std::string &source, const std::string &destination) {
	if (fileExists(destination) && fileExists(source)) {
		Directory* destinationDirectory = root->getDirectory(directoryPart(destination));
		Directory* sourceDirectory = root->getDirectory(directoryPart(source));
		File* destinationFile = destinationDirectory->getFile(filePart(destination));
		File* sourceFile = sourceDirectory->getFile(filePart(source));

		if (sourceFile->getReadPermission() && destinationFile->getWritePermission()) {
			std::string appendstring = fileToString(source);
			return appendToFile(destinationFile, appendstring);
		} else {
			return "Files did not have proper read/write permission.\n";
		}
	}
	
    return "File(s) were not found.\n";
}

std::string FileSystem::rename(const std::string &source, const std::string &newName) {
    if (newName.empty())
        return "New name can't be empty.\n";
    
    if (!fileExists(source))
        return "Can't rename file, source does not exist.\n";
    
    if (fileOrDirectoryExists(newName))
        return "Destination already exists.\n";
    
    Directory* sourceDirectory = root->getDirectory(directoryPart(source));
    Directory* destinationDirectory = root->getDirectory(directoryPart(newName));
    
    if (destinationDirectory == nullptr)
        return "Destination directory does not exist.\n";
    
    File* file = sourceDirectory->getFile(filePart(source));
    if (!file->getWritePermission())
        return "File is write protected.\n";
    
    sourceDirectory->rm(filePart(source));
    file->rename(newName);
    destinationDirectory->addFile(file);
    
    return "";
}

std::string FileSystem::chmod(const std::string &path, int permission) {
	Directory* directory = root->getDirectory(directoryPart(path));
    if (directory == nullptr)
        return "File does not exist.\n";
    
	File* file = directory->getFile(filePart(path));
    if (file == nullptr)
        return "File does not exist.\n";
    
	if (permission > 3 || permission < 0)
		return "Invalid permission level. Valid permission levels:\n"
               "0 = NONE.\n"
               "1 = READ only.\n"
               "2 = WRITE only.\n"
               "3 = READ and WRITE permission.\n";
    
	file->setPermission(File::Permission(permission));
    
    return "";
}

bool FileSystem::directoryExists(const std::string &path) {
    return root->getDirectory(path) != nullptr;
}

std::string FileSystem::fileToString(const std::string &path) const {
	Directory* directory = root->getDirectory(directoryPart(path));
	File* file = directory->getFile(filePart(path));
	std::vector<int> tempNrs = file->getBlockNumbers();
	std::string contents;
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

std::string FileSystem::appendToFile(File* file, std::string contents) {
	std::vector<int> freeBlock = freeBlocks();
	std::vector<int> usedBlockNumbers;
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
        
		if (requiredBlocks > freeBlock.size())
			return "Not enough free blocks.\n";
		
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
    
    return "";
}

std::vector<int> FileSystem::freeBlocks() const{
	std::vector<int> blockList;
	for (int i = 0; i < freeBlockNumbers.size(); i++){
		if (freeBlockNumbers[i] == true)
			blockList.push_back(i);
	}
	return blockList;
}

bool FileSystem::fileOrDirectoryExists(const std::string &path) const {
	Directory* directory = root->getDirectory(directoryPart(path));
	if (directory->getDirectory(filePart(path)) != nullptr)
		return true;

	if (directory->getFile(filePart(path)) != nullptr)
		return true;
    
	return false;
}

bool FileSystem::fileExists(const std::string &path) const {
	Directory* directory = root->getDirectory(directoryPart(path));
    if (directory == nullptr)
        return false;
    
	return (directory->getFile(filePart(path)) != nullptr);
}

std::string FileSystem::directoryPart(const std::string &path) {
    size_t pos = path.find_last_of('/');
    if (pos == std::string::npos)
        return "";
    
    return path.substr(0, pos);
}

std::string FileSystem::filePart(const std::string &path) {
    size_t pos = path.find_last_of('/');
    if (pos == std::string::npos)
        return path;
    
    if (path.length() < pos + 2)
        return "";
    
    return path.substr(pos + 1);
}
