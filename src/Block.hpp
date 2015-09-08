#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <string>
#include <vector>
#include <iostream>

/// A block on the simulated hard drive.
class Block {
public:
    /// Create new block.
    /**
     * @param nrOfElements Size of block in bytes.
     */
    Block(int nrOfElements = 512);
    
    /// Copy-constructor.
    /**
     * @param other Other block to copy.
     */
    Block(const Block &other);

    /// Destructor.
    ~Block();

    /// Assignment operator.
    /**
     * @param other Block to be assigned to.
     * @return This block.
     */
    Block& operator = (const Block &other);
    
    /// Get byte in block.
    /**
     * @param index Index of the byte to get.
     * @return The requested byte
     */
    char operator[] (int index) const;
    
    /// Print block to output stream.
    /**
     * @param os Output stream to print to.
     * @param blck %Block to print.
     * @return The output stream
     */
    friend std::ostream& operator<<(std::ostream &os, const Block& blck)
    {
        for (int i = 0; i < blck.nrOfElements; ++i)
            os << blck.block[i];
        return os;
    }
    
    /// Reset the contents of the block.
    /**
     * @param c Char to set the entire block to.
     */
    void reset(char c = 0);
    
    /// Get the size of the block.
    /**
     * @return Size of the block in bytes
     */
    int size() const;
    
    /// Return a copy of the block.
    /**
     * @return A copy of the block
     */
    Block readBlock() const;

    /// Write the block's contents.
    /**
     * @param strBlock The content as a string.
     * @return 1 on success, -2 on failure.
     */
    int writeBlock(const std::string &strBlock);
    
    /// Write the block's contents.
    /**
     * @param vec The content as a char-vector.
     * @return 1 on success, -2 on failure.
     */
    int writeBlock(const std::vector<char> &vec);
    
    /// Write the block's contents.
    /**
     * @warning Use with caution! Make sure that cArr is at least as large as private member block.
     * @param cArr The content as a char-array.
     */
    void writeBlock(const char cArr[]);
    
    /// Get block's contents as string.
    /**
     * @return The block's contents as a string
     */
    std::string toString() const;
    
private:
    char *block;
    int nrOfElements;
};

#endif // BLOCK_HPP
