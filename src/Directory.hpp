#ifndef DIRECTORY_HPP
#define DIRECTORY_HPP

#include <string>
#include <vector>

/// A directory on the harddrive.
/**
 * Can contain files and other directories.
 */
class Directory {
public:
    /// Create new directory.
    /**
     * @param parent Parent directory.
     * @param name Name of the directory.
     */
    Directory(const Directory* parent, const std::string &name);
    
    /// Destructor.
    ~Directory();
    
    /// Add a subdirectory to the directory.
    /**
     * @param name Name of te subdirectory to add.
     */
    void addDirectory(const std::string &name);
    
    /// Print the contents of the directory.
    void ls() const;
    
    /// Get the directory information as a string.
    /**
     * @return String containing the directory's information
     */
    std::string toString() const;
    
private:
    const Directory* parent;
    std::string name;
    
    std::vector<Directory*> contents;
};

#endif // DIRECTORY_HPP
