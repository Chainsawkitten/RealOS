#include "FileSystem.hpp"


FileSystem::FileSystem() {

}

void FileSystem::format() {
	mMemblockDevice.reset();
}

void FileSystem::save(const std::string &saveFile) const{
	std::ofstream file;
	file.open(saveFile, std::ios::out|std::ios::binary);
	int nrOfBlocks = mMemblockDevice.size();
	for (int i = 0; i < nrOfBlocks; i++)
		file.write(mMemblockDevice.readBlock(i).toString().c_str(), 512);
	file.close();
}

void FileSystem::read(const std::string &saveFile) {
	std::ifstream file;
	char buffer[512];
	file.open(saveFile, std::ios::in | std::ios::binary);
	int nrOfBlocks = mMemblockDevice.size();
	for (int i = 0; i < nrOfBlocks; i++){
		file.read(buffer, 512);
		std::string temp(buffer);
		mMemblockDevice.writeBlock(i, temp);
	}
	file.close();
}