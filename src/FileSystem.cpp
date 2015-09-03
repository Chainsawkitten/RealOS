#include "FileSystem.hpp"


FileSystem::FileSystem() {

}

void FileSystem::format() {
	mMemblockDevice.reset();
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