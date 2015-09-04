#include "Directory.hpp"
#include <iostream>

using namespace std;

Directory::Directory(const std::string &name) {
    this->name = name;
}

Directory::~Directory() {
    for (auto directory : contents) {
        delete directory.second;
    }
}

Directory* Directory::createDirectory(const string &name) {
    contents[name] = new Directory(name);
    
    return contents[name];
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
    
    if (contents.find(dir) != contents.end()) {
        return contents[dir]->getDirectory(remaining);
    }
    
    return nullptr;
}

File* Directory::getFile(const string &path) {
	string dir;
	string remaining;
	size_t pos = path.find('/');
	if (pos == string::npos){
		dir = path;
		for (int i = 0; i < files.size(); i++) {
			if (files[i].getName() == path)
				return &files[i];
		}
	}
	else {
		dir = path.substr(0, pos);
		remaining = path.substr(pos + 1);
	}
	if (contents.find(dir) != contents.end()) {
		return contents[dir]->getFile(remaining);
	}

	return nullptr;
}

void Directory::ls() const {
    for (auto directory : contents) {
        cout << directory.second->toString();
    }
}

string Directory::toString() const {
    return "DIR   " + name + "\n";
}
