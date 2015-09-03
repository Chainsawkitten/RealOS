#include "Directory.hpp"
#include <iostream>

using namespace std;

Directory::Directory(const Directory* parent, const std::string &name) {
    this->parent = parent;
    this->name = name;
}

Directory::~Directory() {
    for (Directory* directory : contents) {
        delete directory;
    }
}

void Directory::addDirectory(const string &name) {
    contents.push_back(new Directory(this, name));
}

void Directory::ls() const {
    for (Directory* directory : contents) {
        cout << directory->toString();
    }
}

string Directory::toString() const {
    return "DIR   " + name + "\n";
}
