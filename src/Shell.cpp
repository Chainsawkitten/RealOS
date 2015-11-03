#include "Shell.hpp"
#include <iostream>
#include <sstream>
#include "FileSystem.hpp"

const int MAXCOMMANDS = 8;
const int NUMAVAILABLECOMMANDS = 16;

std::string availableCommands[NUMAVAILABLECOMMANDS] = {
	"quit", "format", "ls", "create", "cat", "save", "load",
	"rm", "copy", "append", "rename", "mkdir", "cd", "pwd", "help", "chmod"
};

Shell::Shell(const std::string &user) {
	this->user = user;
	currentDir = "/";
}

bool Shell::getCommand() {
	std::string userCommand, commandArr[MAXCOMMANDS];

	std::cout << user << ":" << currentDir << "$ ";
	std::getline(std::cin, userCommand);
	int nrOfCommands = parseCommandString(userCommand, commandArr);
	if (nrOfCommands > 0) {
		int cIndex = findCommand(commandArr[0]);
		std::string fileContent;
		switch (cIndex) {

		case 0: // quit
			std::cout << "Exiting" << std::endl;
			return false;
			break;
		case 1: // format
			fileSystem.format();
            currentDir = "/";
			break;
		case 2: // ls
			std::cout << "Listing directory" << std::endl;
            
            if (nrOfCommands < 2)
                fileSystem.ls(absolutePath(currentDir));
            else
                fileSystem.ls(absolutePath(commandArr[1]));

			break;
		case 3: // create	
			std::cout << "Enter file contents: \n";
			std::getline(std::cin, fileContent);
			fileSystem.create(absolutePath(commandArr[1]), fileContent);
			break;
		case 4: // cat
			fileSystem.cat(absolutePath(commandArr[1]));
			break;
		case 5: // save
			fileSystem.save(commandArr[1]);
			break;
		case 6: // load
			fileSystem.load(commandArr[1]);
			break;
		case 7: // rm
			fileSystem.rm(absolutePath(commandArr[1]));
			break;

		case 8: // copy
			fileSystem.copy(absolutePath(commandArr[1]), absolutePath(commandArr[2]));
			break;

		case 9: // append
			fileSystem.append(absolutePath(commandArr[1]), absolutePath(commandArr[2]));
			break;

		case 10: // rename
            fileSystem.rename(absolutePath(commandArr[1]), absolutePath(commandArr[2]));
			break;

		case 11: // mkdir
			fileSystem.mkdir(absolutePath(commandArr[1]));
			break;

		case 12: // cd
			if (fileSystem.directoryExists(absolutePath(commandArr[1]))) {
				currentDir = "/" + absolutePath(commandArr[1]);
				if (currentDir[currentDir.length() - 1] != '/')
					currentDir += "/";
			} else {
				std::cout << "Directory does not exist." << std::endl;
            }
			break;

		case 13: // pwd
			std::cout << currentDir << std::endl;
			break;

		case 14: // help
			std::cout << help() << std::endl;
			break;
            
		case 15: // chmod
			if (commandArr[2].length() != 0){
                int perm = 0;
                try {
                    perm = stoi(commandArr[2]);
				}catch (std::invalid_argument) {
					std::cout << "Invalid argument." << std::endl;
                    break;
                }

				fileSystem.chmod(absolutePath(commandArr[1]), perm);
			} else {
				std::cout << "Invalid amount of arguments." << std::endl;
            }
			break;

		default:
			std::cout << "Unknown command: " << commandArr[0] << std::endl;
		}
	}

	return true;
}

int Shell::parseCommandString(const std::string &userCommand, std::string strArr[]) {
	std::stringstream ssin(userCommand);
	int counter = 0;
	while (ssin.good() && counter < MAXCOMMANDS) {
		ssin >> strArr[counter];
		counter++;
	}
	if (strArr[0] == "") {
		counter = 0;
	}
	return counter;
}

int Shell::findCommand(std::string &command) {
	int index = -1;
	for (int i = 0; i < NUMAVAILABLECOMMANDS && index == -1; ++i) {
		if (command == availableCommands[i]) {
			index = i;
		}
	}
	return index;
}

std::string Shell::absolutePath(std::string path) const {
	// Fix relative path
	if (path.length() == 0 || path[0] != '/')
		path = currentDir + path;
	path = path.substr(1);
    
	std::vector<std::string> parts = split(path, '/');
	std::vector<std::string> temp;
    
    // Replace ./ and ../
	for (std::string part : parts) {
        if (part == "..") {
            if (!temp.empty())
                temp.pop_back();
        } else if (part != ".") {
            temp.push_back(part);
        }
    }
    
    path = "";
    for (size_t i=0; i<temp.size(); i++) {
        if (i > 0)
            path += "/";
        path += temp[i];
    }

	return path;
}

std::string Shell::help() {
	std::string helpStr;
	helpStr += "OSD Disk Tool .oO Help Screen Oo.\n";
	helpStr += "-----------------------------------------------------------------------------------\n";
	helpStr += "* quit:                             Quit OSD Disk Tool\n";
	helpStr += "* format;                           Formats disk\n";
	helpStr += "* ls     <path>:                    Lists contents of <path>.\n";
	helpStr += "* create <path>:                    Creates a file and stores contents in <path>\n";
	helpStr += "* cat    <path>:                    Dumps contents of <file>.\n";
	helpStr += "* save   <real-file>:               Saves disk to <real-file>\n";
	helpStr += "* read   <real-file>:               Reads <real-file> onto disk\n";
	helpStr += "* rm     <file>:                    Removes <file>\n";
	helpStr += "* copy   <source>    <destination>: Copy <source> to <destination>\n";
	helpStr += "* append <source>    <destination>: Appends contents of <source> to <destination>\n";
	helpStr += "* rename <old-file>  <new-file>:    Renames <old-file> to <new-file>\n";
	helpStr += "* mkdir  <directory>:               Creates a new directory called <directory>\n";
	helpStr += "* cd     <directory>:               Changes current working directory to <directory>\n";
	helpStr += "* pwd:                              Get current working directory\n";
    helpStr += "* chmod  <file>      <permissions>: Change permissions for <file> to <permissions>.\n";
	helpStr += "* help:                             Prints this help screen\n";
	return helpStr;
}

std::vector<std::string> Shell::split(std::string text, char delimiter) {
	std::vector<std::string> parts;
    
	while (text.find(delimiter) != std::string::npos) {
        size_t pos = text.find(delimiter);
        parts.push_back(text.substr(0, pos));
        text = text.substr(pos+1);
    }
    
    if (!text.empty())
        parts.push_back(text);
    
    return parts;
}
