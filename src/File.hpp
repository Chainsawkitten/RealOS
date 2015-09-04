#ifndef FILE_HPP
#define FILE_HPP

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
	* @param name Name of the file.
	*/
	File(const std::string &name);

	/// Destructor.
	~File();
	
	///Prints file to screen
	//void cat(const FileSystem &fileSystem) const;

	///Appends file to this file
	void append(const File& other);

	///Renames file
	void rename(std::string name);

	/// Get the file information as a string.
	/**
	* @return string containing information
	*/
	std::string toString() const;

	/// Get the files name.
	/**
	* @return string containing the files name
	*/
	std::string getName() const;

	/// Get the files length.
	/**
	* @return string containing the files name
	*/
	int getLength() const;

	/// Get the files blocknumbers
	/**
	* @return vector containing the files blocknumbers
	*/
	std::vector<int> getBlockNumbers() const;
private:
	bool read;
	bool write;
	int length;
	std::string name;

	std::vector<int> blockNumbers;
};

#endif // FILE_HPP
