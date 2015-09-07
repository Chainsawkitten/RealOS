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
    
    for (auto file : files) {
        delete file.second;
    }
}

Directory* Directory::createDirectory(const string &name) {
    directories[name] = new Directory(name);
    
    return directories[name];
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

File* Directory::createFile(const std::string &name) {
	File* newFile = new File(name);

	files[name] = newFile;

	return newFile;
}

void Directory::addFile(File* file) {
    files[file->getName()] = file;
}

File* Directory::getFile(const string &name) const {
    if (files.find(name) != files.end())
        return files[name];

	return nullptr;
}

void Directory::ls() const {
    for (auto directory : directories) {
        cout << directory.second->toString();
    }
    
    for (auto file : files) {
        cout << file.second->toString();
    }
}

void Directory::rm(const string &name){
	files.erase(name);
}

string Directory::toString() const {
    return "DIR               " + name + "\n";
}

void Directory::save(ofstream &file) const {
    // Save files
    size_t length = files.size();
    file.write(reinterpret_cast<const char*>(&length), sizeof(length));
    for (auto f : files) {
        length = f.first.length();
        file.write(reinterpret_cast<const char*>(&length), sizeof(length));
        file.write(f.first.c_str(), length);
        
        f.second->save(file);
    }
    
    // Save directories
    length = directories.size();
    file.write(reinterpret_cast<const char*>(&length), sizeof(length));
    for (auto d : directories) {
        length = d.first.length();
        file.write(reinterpret_cast<const char*>(&length), sizeof(length));
        file.write(d.first.c_str(), length);
        
        d.second->save(file);
    }
}

void Directory::load(ifstream &file) {
    // Load files
    size_t length;
    file.read(reinterpret_cast<char*>(&length), sizeof(length));
    for (size_t i=0; i<length; i++) {
        size_t nameLength;
        file.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
        char* buffer = new char[nameLength+1];
        file.read(buffer, nameLength);
        buffer[nameLength] = '\0';
        File* f = new File(buffer);
        files[buffer] = f;
        delete[] buffer;
        
        f->load(file);
    }
    
    // Load directories
    file.read(reinterpret_cast<char*>(&length), sizeof(length));
    for (size_t i=0; i<length; i++) {
        size_t nameLength;
        file.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
        char* buffer = new char[nameLength+1];
        file.read(buffer, nameLength);
        buffer[nameLength] = '\0';
        Directory* d = new Directory(buffer);
        directories[buffer] = d;
        delete[] buffer;
        
        d->load(file);
    }
}
