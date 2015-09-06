#ifndef BLOCKDEVICE_HPP
#define BLOCKDEVICE_HPP

#include "Block.hpp"

/// A device consisting of multiple blocks.
/**
 * Pure virtual class.
 */
class BlockDevice {
public:
    /// Create new block device.
    /**
     * @param nrOfBlocks Number of blocks.
     */
    BlockDevice(int nrOfBlocks);
    
	/// Create new block device.
	/**
	* @param nrOfBlocks Number of blocks.
	* @param nrOfElements number of elements per block.
	*/
	BlockDevice(int nrOfBlocks, int nrOfElements);

    /// Copy-constructor.
    /**
     * @param other Other block device to copy.
     */
    BlockDevice(const BlockDevice &other);
    
    /// Destructor.
    virtual ~BlockDevice();
    
    /// Get amount of free blocks.
    /**
     * @return The amount of free blocks
     */
    virtual int spaceLeft() const = 0;
    
    /// Write to a block on the device.
    /**
     * @param blockNr Number of the block to write to.
     * @param vec Char-vector to write to the block.
     * @return 1 on success, -2 on incorrect vector size, -1 on incorrect block number
     */
    virtual int writeBlock(int blockNr, const std::vector<char> &vec) = 0;
    
    /// Write to a block on the device.
    /**
     * @param blockNr Number of the block to write to.
     * @param strBlock String to write to the block.
     * @return 1 on success, -2 on incorrect vector size, -1 on incorrect block number
     */
    virtual int writeBlock(int blockNr, const std::string &strBlock) = 0;
    
    /// Write to a block on the device.
    /**
     * Use with caution! Make sure that cArr is at least as large as private member block.
     * @param blockNr Number of the block to write to.
     * @param cArr Char-array to write to the block.
     * @return 1 on success, -1 on incorrect block number
     */
    virtual int writeBlock(int blockNr, const char cArr[]) = 0;
    
    /// Get a copy of a block on the device.
    /**
     * @param blockNr Number of the block to read.
     * @return A copy of the block.
     */
    virtual Block readBlock(int blockNr) const = 0;
    
    /// Reset the device, clearing all blocks.
    virtual void reset() = 0;
    
    /// Get size of device in blocks.
    /**
     * @return Number of blocks the device contains.
     */
    virtual int size() const = 0;
    
protected:
    Block* memBlocks;
    int nrOfBlocks;
	int nrOfElements;
    int freePointer;
    
};

#endif // BLOCKDEVICE_HPP
