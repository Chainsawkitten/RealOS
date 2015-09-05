#include "Shell.hpp"
#include <iostream>
#include <sstream>
#include "FileSystem.hpp"

using namespace std;

const int MAXCOMMANDS = 8;
const int NUMAVAILABLECOMMANDS = 16;

string availableCommands[NUMAVAILABLECOMMANDS] = {
	"quit", "format", "ls", "create", "cat", "save", "load",
	"rm", "copy", "append", "rename", "mkdir", "cd", "pwd", "help", "chmod"
};

Shell::Shell(const string &user) {
	this->user = user;
	currentDir = "/";
}

bool Shell::getCommand() {
	string userCommand, commandArr[MAXCOMMANDS];

	cout << user << ":" << currentDir << "$ ";
	getline(cin, userCommand);

	int nrOfCommands = parseCommandString(userCommand, commandArr);
	if (nrOfCommands > 0) {
		int cIndex = findCommand(commandArr[0]);
		switch (cIndex) {

		case 0: // quit
			cout << "Exiting" << endl;
			return false;
			break;
		case 1: // format
			fileSystem.format();
            currentDir = "/";
			break;
		case 2: // ls
			cout << "Listing directory" << endl;
            
            if (nrOfCommands < 2)
                fileSystem.ls(absolutePath(currentDir));
            else
                fileSystem.ls(absolutePath(commandArr[1]));

			break;
		case 3: // create
			fileSystem.create(absolutePath(commandArr[1]));
			break;
		case 4: // cat
			fileSystem.cat(absolutePath(commandArr[1]));
			break;
		case 5: // save
			fileSystem.save(absolutePath(commandArr[1]));
			break;
		case 6: // load
			fileSystem.load(absolutePath(commandArr[1]));
			break;
		case 7: // rm
			fileSystem.rm(absolutePath(commandArr[1]));
			break;

		case 8: // copy
			/// @todo copy
			break;

		case 9: // append
			fileSystem.append(absolutePath(commandArr[1]), absolutePath(commandArr[2]));
			break;

		case 10: // rename
			/// @todo rename
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
                cout << "Directory does not exist." << endl;
            }
			break;

		case 13: // pwd
			cout << currentDir << endl;
			break;

		case 14: // help
			cout << help() << endl;
			break;
		case 15: // chmod
			if (commandArr[2].length() != 0){
				int perm = stoi(commandArr[2]);
				fileSystem.chmod(absolutePath(commandArr[1]), perm);
			}
			else
				cout << "Invalid amount of arguments. \n";
			break;

		default:
			cout << "Unknown command: " << commandArr[0] << endl;

		}
	}

	return true;
}

int Shell::parseCommandString(const string &userCommand, string strArr[]) {
	stringstream ssin(userCommand);
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

int Shell::findCommand(string &command) {
	int index = -1;
	for (int i = 0; i < NUMAVAILABLECOMMANDS && index == -1; ++i) {
		if (command == availableCommands[i]) {
			index = i;
		}
	}
	return index;
}

std::string Shell::absolutePath(string path) {
	/// Fix relative path
	if (path.length() == 0 || path[0] != '/')
		path = currentDir + path;
	path = path.substr(1);

	/// @todo Replace ./
	/// @todo Replace ../

	return path;
}

string Shell::help() {
	string helpStr;
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
	helpStr += "* help:                             Prints this help screen\n";
	return helpStr;
}
