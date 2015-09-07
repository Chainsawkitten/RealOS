#include "MemBlockDevice.hpp"
#include <stdexcept>

MemBlockDevice::MemBlockDevice(int nrOfBlocks) {
    if (nrOfBlocks > 0)
        this->nrOfBlocks = nrOfBlocks;
    else
        this->nrOfBlocks = 250;
	nrOfElements = 512;
    memBlocks = new Block[this->nrOfBlocks];
}

MemBlockDevice::MemBlockDevice(int nrOfBlocks, int nrOfElements) {
    if (nrOfBlocks > 0)
		this->nrOfBlocks = nrOfBlocks;
	else
		this->nrOfBlocks = 250;
	this->nrOfElements = nrOfElements;
	memBlocks = new Block[this->nrOfBlocks];
	for (int i = 0; i < nrOfBlocks; i++){
		memBlocks[i] = Block(nrOfElements);
	}
}

MemBlockDevice::MemBlockDevice(const MemBlockDevice &other) {
    nrOfBlocks = other.nrOfBlocks;
    memBlocks = new Block[nrOfBlocks];
	nrOfElements = other.nrOfElements;
    for (int i = 0; i < nrOfBlocks; ++i)
        memBlocks[i] = other.memBlocks[i];
}

MemBlockDevice::~MemBlockDevice() {
    delete[] memBlocks;
}

MemBlockDevice& MemBlockDevice::operator=(const MemBlockDevice &other) {
    delete[] memBlocks;
    nrOfBlocks = other.nrOfBlocks;
    memBlocks = new Block[nrOfBlocks];

    for (int i = 0; i < nrOfBlocks; ++i)
        memBlocks[i] = other.memBlocks[i];

    return *this;
}

Block& MemBlockDevice::operator[](int index) const {
    if (index < 0 || index >= nrOfBlocks) {
        throw std::out_of_range("Illegal access\n");
    } else {
        return memBlocks[index];
    }
}

int MemBlockDevice::writeBlock(int blockNr, const std::vector<char> &vec) {
    int output = -1; // Assume blockNr out-of-range

    if (blockNr < nrOfBlocks && blockNr >= 0) {
        /* -2 = vec and block dont have same dimensions */
        /* 1 = success */
        output = memBlocks[blockNr].writeBlock(vec);
    }
    return output;
}

int MemBlockDevice::writeBlock(int blockNr, const std::string &strBlock) {
    int output = -1; // Assume blockNr out-of-range

    if (blockNr < nrOfBlocks && blockNr >= 0) {
        /* -2 = str-length and block dont have same dimensions */
        /* 1 = success */
        output = memBlocks[blockNr].writeBlock(strBlock);
    }
    return output;
}

int MemBlockDevice::writeBlock(int blockNr, const char cArr[]) {
    int output = -1; // Assume blockNr out-of-range
    if (blockNr < nrOfBlocks && blockNr >= 0) {
        output = 1;
        // Underlying function writeBlock cannot check array-dimension.
        memBlocks[blockNr].writeBlock(cArr);
    }
    return output;
}

Block MemBlockDevice::readBlock(int blockNr) const {
    if (blockNr < 0 || blockNr >= nrOfBlocks) {
        throw std::out_of_range("Block out of range");
    } else {
        Block a(memBlocks[blockNr]);
        return a;
    }
}

void MemBlockDevice::reset() {
    for (int i = 0; i < nrOfBlocks; ++i) {
        memBlocks[i].reset('0');
    }
}

int MemBlockDevice::getBlockLength() const{
	return memBlocks->size();
}

int MemBlockDevice::size() const {
    return nrOfBlocks;
}
