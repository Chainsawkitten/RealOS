#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include "MemBlockDevice.hpp"
#include "Directory.hpp"
#include "File.hpp"
#include <vector>
#include <iostream>
#include <fstream>

/// The simulated file system.
class FileSystem {
public:
    /// Create new file system.
    FileSystem();
    
    /// Destructor.
    ~FileSystem();
    
    /// Format the file system.
    void format();
    
    /// Print the contents of a given directory.
    /**
     * @param path Path to the directory to print.
     */
    void ls(const std::string &path) const;
    
    /// Create a new file.
    /**
     * @param filePath Path to the file to create.
     */
    void create(const std::string &filePath);
    
    /// Create a new directory.
    /**
     * @param path Path to directory to create.
     */
    void mkdir(const std::string &path);
    
    /// Prints the contents of a file.
    /**
     * @param fileName Path to the file to print.
     */
    void cat(const std::string &fileName) const;
    
    /// Save the file system to a file on the real hard drive.
    /**
     * @param saveFile Filename.
     */
    void save(const std::string &saveFile) const;
    
    /// Read the file system from a file on the real hard drive.
    /**
     * @param saveFile Filename.
     */
    void load(const std::string &saveFile) ;
    
    /// Delete a file.
    /**
     * @param filePath Path to the file to delete.
     */
    void rm(const std::string &filePath);
    
    /// Copy a file.
    /**
     * @param source Path to the file to copy.
     * @param dest Path to the destination.
     */
    void copy(const std::string &source, const std::string &dest);
    
    /// Append the contents of one file to another.
    /**
     * @param source Path to source file.
     * @param app Path to file to append to.
     */
    void append(const std::string &source, const std::string &app);

    /// Rename a file.
    /**
     * @param source Old file path.
     * @param newName New file path.
     */
    void rename(const std::string &source, const std::string &newName);
    
    /// Change permissions.
    /**
     * @param permission Permission to set to.
     * @param file Path to the file to set permissions for.
     */
	void chmod(const std::string &path, int permission);

	/// Checks whether a given directory exists.
	/**
	* @param path Path to the directory to check.
	* @return Whether the directory exists
	*/
	bool directoryExists(const std::string &path);
private:
	/// Helper functions that helps the system to create files
	/**
	* @param file file to append to.
	* @param contents contents to write to file.
	*/
	void appendToFile(File* file, std::string contents);

	/// Finds unused blocknumbers in freeBlockNumbers
	/**
	* @return a vector of unused blocknumbers
	*/
	std::vector<int> freeBlocks() const;

	/// Checks whether a file/directory exists or not
	/**
	* @return wheter a file/directory exists or not.
	*/
	bool fileOrDirectoryExists(const std::string &path) const;

	/// Checks whether a file exists or not
	/**
	* @return wheter a file exists or not.
	*/
	bool fileExists(const std::string &path) const;

	/// Puts a files content into a string
	/**
	* @param path path to the file
	* @return file content
	*/
	std::string fileToString(const std::string &path) const;

    MemBlockDevice mMemblockDevice;
	std::vector<bool> freeBlockNumbers;
    Directory* root;
    
    static std::string directoryPart(const std::string &path);
    static std::string filePart(const std::string &path);
};

#endif // FILESYSTEM_HPP
