#ifndef DIRECTORY_HPP
#define DIRECTORY_HPP

#include "File.hpp"
#include <string>
#include <map>
#include <vector>
#include <fstream>

/// A directory on the harddrive.
/**
 * Can contain files and other directories.
 */
class Directory {
public:
    /// Create new directory.
    /**
     * @param name Name of the directory.
     */
    Directory(const std::string &name);
    
    /// Destructor.
    ~Directory();
    
    /// Create a subdirectory in the directory.
    /**
     * @param name Name of the subdirectory to create.
     * @return The created directory
     */
    Directory* createDirectory(const std::string &name);
    
    /// Get a subdirectory in the directory.
    /**
     * @param path Path to the subdirectory (relative to this directory).
     * @return The directory or nullptr if it doesn't exist
     */
    Directory* getDirectory(const std::string &path);

	/// Create a file in the directory.
	/**
	 * @param name Name of the file to create.
	 * @return The created file
	 */
	File* createFile(const std::string &name);
    
    /// Add an existing file to the directory.
    /**
     * @param file The file to add.
     */
    void addFile(File* file);

	/// Get a file in this directory.
	/**
	 * @param name Name of the file to get.
	 * @return The file or nullptr if it doesn't exist
	 */
	File* getFile(const std::string &name) const;
    
    /// Print the contents of the directory.
    void ls() const;
    
	/// Removes file listing
    /**
     * @param name Name of the file to remove.
     */
	void rm(const std::string &name);

    /// Get the directory information as a string.
    /**
     * @return String containing the directory's information
     */
    std::string toString() const;
    
    /// Save to file.
    /**
     * @param file %File stream to save to.
     */
    void save(std::ofstream &file) const;
    
    /// Load from file.
    /**
     * @param file %File stream to load from.
     */
    void load(std::ifstream &file);
    
private:
    std::string name;
    
	std::map<std::string, File*> files;
    std::map<std::string, Directory*> directories;
};

#endif // DIRECTORY_HPP
