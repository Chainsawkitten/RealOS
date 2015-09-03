#ifndef MEMBLOCKDEVICE_HPP
#define MEMBLOCKDEVICE_HPP

#include "BlockDevice.hpp"

/// A simulated device containing multiple blocks in memory.
class MemBlockDevice: public BlockDevice {
public:
    /// Create new memory block device.
    /**
     * @param nrOfBlocks Number of blocks on the device.
     */
    MemBlockDevice(int nrOfBlocks = 250);
    
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

    /// Get amount of free blocks.
    /**
     * @return The amount of free blocks
     */
    int spaceLeft() const;

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
     * Use with caution! Make sure that cArr is at least as large as private member block.
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
};

#endif // MEMBLOCKDEVICE_HPP
