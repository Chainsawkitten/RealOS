#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include "MemBlockDevice.hpp"
#include "Directory.hpp"
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
    
    /// Print the contents of the current directory.
    void ls() const;
    
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
    
    /// Prints the contents of a file.
    /**
     * @param fileName Path to the file to print.
     */
    void cat(std::string &fileName) const;
    
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
    void chmod(int permission, const std::string &file);
    
private:
    MemBlockDevice mMemblockDevice;
    Directory* root;
};

#endif // FILESYSTEM_HPP
