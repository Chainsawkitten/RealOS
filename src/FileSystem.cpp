#include "FileSystem.hpp"

FileSystem::FileSystem() {
    root = new Directory(nullptr, "/");
    root->addDirectory("test");
    root->addDirectory("test2");
    root->addDirectory("test3");
}

FileSystem::~FileSystem() {
    delete root;
}

void FileSystem::ls() const {
    root->ls();
}
