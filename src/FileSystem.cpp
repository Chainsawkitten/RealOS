#include "FileSystem.hpp"
#include <iostream>

using namespace std;

FileSystem::FileSystem() {
    root = new Directory(nullptr, "/");
    root->createDirectory("test");
    Directory* test = root->createDirectory("test2");
    test->createDirectory("test3");
}

FileSystem::~FileSystem() {
    delete root;
}

void FileSystem::ls() const {
    root->getDirectory("test2/")->ls();
}
