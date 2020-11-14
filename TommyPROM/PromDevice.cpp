#include "Configure.h"
#include "PromDevice.h"


PromDevice::PromDevice(uint32_t size, word blockSize, unsigned maxWriteTime, bool polling)
    : mSize(size),
      mBlockSize(blockSize),
      mMaxWriteTime(maxWriteTime),
      mSupportsDataPoll(polling)
{
}


// Write a block of data to the device.  If the device supports block writes,
// the data will be broken into chunks and written using the block mode.
// Otherwise, each byte will be individually written and verified.
bool PromDevice::writeData(byte data[], uint32_t len, uint32_t address)
{
    bool status = true;

    if (mBlockSize == 0)
    {
        // Device does not support block writes.
        for (uint32_t ix = 0; (ix < len); ix++)
        {
            if (burnByte(data[ix], address + ix) == false)
            {
                status = false;
                break;
            }
        }
    }
    else
    {
        uint32_t offset = 0;
        uint32_t chunkSize;
        if (address & (mBlockSize - 1))
        {
            // Address does not start on a block boundary.  Adjust the size of
            // the first block to fit within a single block.
            chunkSize = mBlockSize - (address & (mBlockSize - 1));
            chunkSize = (chunkSize > len) ? len : chunkSize;
            if (burnBlock(data, chunkSize, address) == false)
            {
                return false;
            }
            offset += chunkSize;
            len -= chunkSize;
        }

        // All writes are now aligned to block boundaries, so write full blocks
        // or remaining length, whichever is smaller.
        while (len > 0)
        {
            chunkSize = (len > mBlockSize) ? mBlockSize : len;
            if (burnBlock(data + offset, chunkSize, address + offset) == false)
            {
                status = false;
                break;
            }
            offset += chunkSize;
            len -= chunkSize;
        }
    }

    return status;
}

void PromDevice::resetDebugStats() {
    debugBlockWrites = 0;
    debugLastAddress = 0;
    debugLastExpected = 0;
    debugLastReadback = 0;
    debugRxDuplicates = 0;
    debugExtraChars = 0;
    debugRxStarts = 0;
    debugRxSyncErrors = 0;
}
void PromDevice::printDebugStats() {
    Serial.print(F("debugBlockWrites:  "));
    Serial.println(debugBlockWrites);
    Serial.print(F("debugLastAddress:  "));
    Serial.println(debugLastAddress, HEX);
    Serial.print(F("debugLastExpected: "));
    Serial.println(debugLastExpected, HEX);
    Serial.print(F("debugLastReadback: "));
    Serial.println(debugLastReadback, HEX);
    Serial.print(F("debugRxDuplicates: "));
    Serial.println(debugRxDuplicates);
    Serial.print(F("debugExtraChars:   "));
    Serial.println(debugExtraChars);
    Serial.print(F("debugRxStarts:     "));
    Serial.println(debugRxStarts);
    Serial.print(F("debugRxSyncErrors: "));
    Serial.println(debugRxSyncErrors);
}

// BEGIN PRIVATE METHODS
//

// Set the I/O state of the data bus.
// The first two bits of port D are used for serial, so the 8 bits data bus are
// on pins D2..D9.
void PromDevice::setDataBusMode(uint8_t mode)
{
    // On the Uno and Nano, D2..D9 maps to the upper 6 bits of port D and the
    // lower 2 bits of port B.
    if (mode == OUTPUT)
    {
        DDRB |= 0x03;
        DDRD |= 0xfc;
    }
        else
    {
        DDRB &= 0xfc;
        DDRD &= 0x03;
    }
}


// Read a byte from the data bus.  The caller must set the bus to input_mode
// before calling this or no useful data will be returned.
byte PromDevice::readDataBus()
{
    return (PINB << 6) | (PIND >> 2);
}


// Write a byte to the data bus.  The caller must set the bus to output_mode
// before calling this or no data will be written.
void PromDevice::writeDataBus(byte data)
{
     PORTB = (PORTB & 0xfc) | (data >> 6);
     PORTD = (PORTD & 0x03) | (data << 2);
}
