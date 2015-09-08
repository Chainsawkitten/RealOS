#ifndef MEMBLOCKDEVICE_HPP
#define MEMBLOCKDEVICE_HPP

#include "Block.hpp"

/// A simulated device containing multiple blocks in memory.
class MemBlockDevice {
public:
    /// Create new memory block device.
    /**
     * @param nrOfBlocks Number of blocks on the device.
     */
    MemBlockDevice(int nrOfBlocks = 250);

	/// Create new memory block device.
	/**
	*@param nrOfBlocks Number of blocks on the device.
	*@param nrOfElements Amount of elements per block.
	*/
	MemBlockDevice(int nrOfBlocks, int nrOfElements);
    
    /// Copy-constructor.
    /**
     * @param other Other memory block device to copy.
     */
    MemBlockDevice(const MemBlockDevice &other);
    
    /// Destructor.
    ~MemBlockDevice();

    /// Assignment operator.
    /**
     * @param other Memory block device to assign to.
     * @return This memory block device
     */
    MemBlockDevice& operator=(const MemBlockDevice &other);
    
    /// Get block.
    /**
     * @param index Index of the block in the device.
     * @return Requested block
     */
    Block &operator[] (int index) const;

    /// Write to a block on the device.
    /**
     * @param blockNr Number of the block to write to.
     * @param vec Char-vector to write to the block.
     * @return 1 on success, -2 on incorrect vector size, -1 on incorrect block number
     */
    int writeBlock(int blockNr, const std::vector<char> &vec);
    
    /// Write to a block on the device.
    /**
     * @param blockNr Number of the block to write to.
     * @param strBlock String to write to the block.
     * @return 1 on success, -2 on incorrect vector size, -1 on incorrect block number
     */
    int writeBlock(int blockNr, const std::string &strBlock);
    
    /// Write to a block on the device.
    /**
     * @warning Use with caution! Make sure that cArr is at least as large as private member block.
     * @param blockNr Number of the block to write to.
     * @param cArr Char-array to write to the block.
     * @return 1 on success, -1 on incorrect block number
     */
    int writeBlock(int blockNr, const char cArr[]);

    /// Get a copy of a block on the device.
    /**
     * @param blockNr Number of the block to read.
     * @return A copy of the block.
     */
    Block readBlock(int blockNr) const;

    /// Reset the device, clearing all blocks.
    void reset();

    /// Get size of device in blocks.
    /**
     * @return Number of blocks the device contains.
     */
    int size() const;

	///Get amount of bytes in blocks.
	/**
	* @return Number of bytes in blocks
	*/
	int getBlockLength() const;
    
private:
    Block* memBlocks;
    int nrOfBlocks;
	int nrOfElements;
};

#endif // MEMBLOCKDEVICE_HPP
