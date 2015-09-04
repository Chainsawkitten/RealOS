#include "Directory.hpp"
#include <iostream>

using namespace std;

Directory::Directory(const std::string &name) {
    this->name = name;
}

Directory::~Directory() {
    for (auto directory : directories) {
        delete directory.second;
    }
}

Directory* Directory::createDirectory(const string &name) {
    directories[name] = new Directory(name);
    
    return directories[name];
}

void Directory::createFile(const File &file){
	File* tempFile = new File(file);
	files[tempFile->getName()] = tempFile;
	return;
}

Directory* Directory::getDirectory(const string &path) {
    if (path.empty())
        return this;
    
    string dir;
    string remaining;
    size_t pos = path.find('/');
    if (pos == string::npos) {
        dir = path;
        remaining = "";
    } else {
        dir = path.substr(0, pos);
        remaining = path.substr(pos + 1);
    }
    
    if (directories.find(dir) != directories.end())
        return directories[dir]->getDirectory(remaining);
    
    return nullptr;
}

File* Directory::getFile(const string &name) {
    if (files.find(name) != files.end())
        return files[name];

	return nullptr;
}

void Directory::ls() const {
    for (auto directory : directories) {
        cout << directory.second->toString();
    }
}

string Directory::toString() const {
    return "DIR   " + name + "\n";
}
