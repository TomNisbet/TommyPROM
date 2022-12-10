#ifndef INCLUDE_PROM_DEVICE_H
#define INCLUDE_PROM_DEVICE_H

#include "Arduino.h"

/*****************************************************************************/
/*****************************************************************************/
/**
 * PromDevice class
 *
 * Provides the interface to read and write data from a parallel PROM using the
 * Arduino.
 *
 * Block writes are supported on compatible devices by specifying a blockSize
 * in the constructor.  Use zero for devices that only support byte writes.
 */

enum ERET {
  RET_OK,
  RET_FAIL,
  RET_NOT_SUPPORT
};

class PromDevice
{
  public:
    PromDevice(uint32_t size, word blockSize, unsigned maxWriteTime, bool polling);
    bool writeData(byte data[], uint32_t len, uint32_t address);
    byte readData(uint32_t address) { return readByte(address); }
    void resetDebugStats();
    void printDebugStats();

    virtual void begin() = 0;
    virtual const char * getName() = 0;
    virtual ERET disableSoftwareWriteProtect() { return RET_NOT_SUPPORT; }
    virtual ERET enableSoftwareWriteProtect() { return RET_NOT_SUPPORT; }
    virtual ERET erase(uint32_t start, uint32_t end) { return RET_NOT_SUPPORT; }

    uint32_t debugBlockWrites;      // Number of block write operations
    uint32_t debugLastAddress;      // Last address with an issue
    uint8_t  debugLastExpected;     // Last expected readback value
    uint8_t  debugLastReadback;     // Last actual readback value
    uint32_t debugRxDuplicates;     // XModem received packet duplicates
    uint32_t debugExtraChars;       // XModem received bytes beyond first packet
    uint8_t  debugRxStarts;         // XModem start transfer requests
    uint8_t  debugRxSyncErrors;     // XModem unexpected characters after packet

  protected:
    uint32_t mSize;                 // Size of the device, in bytes
    unsigned int mBlockSize;        // Block size for page writes, zero if N/A
    unsigned int mMaxWriteTime;     // Max time (in ms) to wait for write cycle to complete
    bool mSupportsDataPoll;         // End of write detected by data polling

    void setDataBusMode(uint8_t mode);
    byte readDataBus();
    void writeDataBus(byte data);

    virtual void setAddress(uint32_t address) = 0;
    virtual byte readByte(uint32_t address) = 0;
    virtual bool burnByte(byte value, uint32_t address) = 0;
    virtual bool burnBlock(byte data[], uint32_t len, uint32_t address) { return false; }

};


#endif  // #define INCLUDE_PROM_DEVICE_H
