#ifndef SHELL_HPP
#define SHELL_HPP

#include <string>
#include <vector>
///Takes usercommand from input and returns number of commands, commands are stored in strArr[]
/**
@param userCommand the command the user wants to execute
@param strArr[] array that stores commands
@return the number of commands
*/
int parseCommandString(const std::string &userCommand, std::string strArr[]);

///
/**
@param command command string
@return the index number of the command, -1 if command could not be found
*/
int findCommand(std::string &command);

/// Returns help-string
/**
@return returns a string with helpful information
*/
std::string help();

#endif // SHELL_HPP
