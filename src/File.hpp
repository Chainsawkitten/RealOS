#ifndef FILE_HPP
#define FILE_HPP

#include <string>
#include <vector>
#include <fstream>

/// A file on the harddrive.
/**
 * Contains a record of blocks it spans.
 */
class File {
public:
    /// File permissions.
    enum Permission {
        NONE, ///< No permissions granted.
        READ, ///< Reading but not writing.
        WRITE, ///< Writing, but not reading.
        READWRITE ///< Both reading and writing.
    };
    
	/// Create new file.
	/**
	 * @param name Name of the file.
	 */
	File(const std::string &name);
    
	/// Renames file.
    /**
     * @param name New filename.
     */
	void rename(const std::string& name);
    
    /// Get the file's name.
	/**
	 * @return string The file's name
	 */
	std::string getName() const;

    /// Set the files length.
	/**
	 * @param string containing the files name
	 */
	void setLength(const int length);
    
	/// Get the files length.
	/**
	 * @return The length of the file
	 */
	int getLength() const;
    
    /// Set the file's block numbers.
	/**
	 * @param blockNrs The new block numbers.
	 */
	void setBlockNumbers(std::vector<int> blockNrs);
    
	/// Get the file's block numbers.
	/**
	 * @return The file's block numbers
	 */
	std::vector<int> getBlockNumbers() const;

    /// Set permissions.
	/**
	 * @param perm File permissions.
	 */
	void setPermission(Permission perm);
    
	/// Get whether writing to the file is allowed.
	/**
	 * @return Whether writing to the file is allowed
	 */
	bool getWritePermission() const;

	/// Get whether reading from the file is allowed.
	/**
	 * @return Whether reading from the file is allowed
	 */
	bool getReadPermission() const;

	/// Get permissions.
	/**
	 * @return File permissions
	 */
	Permission getPermission() const;
    
    /// Save to file.
    /**
     * @param file %File stream to save to.
     */
    void save(std::ofstream &file);
    
    /// Load from file.
    /**
     * @param file %File stream to load from.
     */
    void load(std::ifstream &file);
    
    /// Get the file information as a string.
	/**
	 * @return String containing information about the file
	 */
	std::string toString() const;

private:
	bool read;
	bool write;
	int length;
	std::string name;

	std::vector<int> blockNumbers;
};

#endif // FILE_HPP
