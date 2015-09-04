#ifndef FILE_HPP
#define FILE_HPP

#include "Directory.hpp"
#include <string>
#include <vector>

/// A file on the harddrive.
/**
* Contains a record of blocks it spans
*/
class File {
public:
	/// Create new file.
	/**
	* @param parent Parent directory.
	* @param name Name of the file.
	*/
	File(const std::string &name);

	/// Destructor.
	~File();
	
	///Prints file to screen
	void cat() const;

	///Appends file to this file
	void append(const File* other);

	///Renames file
	void rename(string name);

	/// Get the file information as a string.
	/**
	* @return String
	*/
	std::string toString() const;

private:
	const Directory* parent;
	bool read;
	bool write;
	std::string name;

	std::vector<int> blockNumbers;
};

#endif // DIRECTORY_HPP
