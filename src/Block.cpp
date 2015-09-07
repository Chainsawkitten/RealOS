#include "Block.hpp"
#include <stdexcept>

using namespace std;

Block::Block(int size) {
    /* Setting size */
    if (size > 0)
        nrOfElements = size;
    else
        nrOfElements = 512;

    block = new char[nrOfElements];

    // Sets everything to 0
    reset();
}

Block::Block(const Block &other) {
    nrOfElements = other.nrOfElements;
    block = new char[nrOfElements];
    for (int i = 0; i < nrOfElements; ++i)
        block[i] = other.block[i];
}

Block::~Block() {
    delete[] block;
}

Block &Block::operator =(const Block &other) {
    delete[] block;
    nrOfElements = other.nrOfElements;
    block = new char[nrOfElements];
    for (int i = 0; i < nrOfElements; ++i)
        this->block[i] = other.block[i];
    return *this;
}

char Block::operator[](int index) const {
    if (index < 0 || index >= nrOfElements) {
        throw out_of_range("Illegal access\n");
    } else {
        return block[index];
    }
}

void Block::reset(char c) {
    for (int i = 0; i < nrOfElements; ++i)
        block[i] = c;
}

int Block::size() const {
    return nrOfElements;
}

Block Block::readBlock() const {
    return Block(*this);
}

int Block::writeBlock(const string &strBlock) {
    int output = -2;    // Assume out of range
    if (strBlock.size() == (unsigned long)nrOfElements) {
        for (int i = 0; i < nrOfElements; ++i) {
            block[i] = strBlock[i];
			cout << i << block[i] << '\n';
        }
        output = 1;
    }

    return output;
}

int Block::writeBlock(const vector<char> &vec) {
    int output = -2; // Assume not the same dimension
    if (vec.size() == (unsigned long)nrOfElements) {
        for (unsigned long int i = 0; i < vec.size(); ++i) {
           block[i] = vec[i];
        }
        output = 1;
    }
    return output;
}

void Block::writeBlock(const char cArr[]) {
    for (int i = 0; i < nrOfElements; ++i) {
        block[i] = cArr[i];
    }
}

string Block::toString() const {
    string output;
    output.resize(nrOfElements);
    for (int i = 0; i < nrOfElements; ++i)
        output += block[i];
    return output;
}
