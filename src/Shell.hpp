#ifndef SHELL_HPP
#define SHELL_HPP

#include <string>
#include <vector>

/* Takes usercommand from input and returns number of commands, commands are stored in strArr[] */
int parseCommandString(const std::string &userCommand, std::string strArr[]);
int findCommand(std::string &command);

/* Splits a filepath into multiple strings */
std::vector<std::string> split(const std::string &filePath, const char delim = '/');

std::string help();

#endif // SHELL_HPP
