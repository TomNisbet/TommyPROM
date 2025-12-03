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
#ifndef IS_MEGA
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
        PORTB |= 0x03;  // set pullup resistors
        PORTD |= 0xfc;
    }
#else // #ifndef IS_MEGA
    // On the Mega, D2..D9 maps to stupid random ports...
    if (mode == OUTPUT) {
        DDRE |= 0x38; // D2,D3,D5 = PE4, PE5, PE3
        DDRG |= 0x20; // D4 = PG5
        DDRH |= 0x78; // D6..D9 = PH3..PH6
    } else {
        // Set to input
        DDRE &= ~0x38;
        DDRG &= ~0x20;
        DDRH &= ~0x78;
        // set pullup resistors
        PORTE |= 0x38;
        PORTG |= 0x20;
        PORTH |= 0x78;
    }

#endif // #ifndef IS_MEGA
}


// Read a byte from the data bus.  The caller must set the bus to input_mode
// before calling this or no useful data will be returned.
byte PromDevice::readDataBus()
{
#ifndef IS_MEGA
    return (PINB << 6) | (PIND >> 2);
#else // #ifndef IS_MEGA
    byte data = 0;
    if (PINE & 0x10) data |= 0x01;    // D2 = PE4
    if (PINE & 0x20) data |= 0x02;    // D3 = PE5
    if (PING & 0x20) data |= 0x04;    // D4 = PG5
    if (PINE & 0x08) data |= 0x08;    // D5 = PE3
    if (PINH & 0x08) data |= 0x10;    // D6 = PH3
    if (PINH & 0x10) data |= 0x20;    // D7 = PH4
    if (PINH & 0x20) data |= 0x40;    // D8 = PH5
    if (PINH & 0x40) data |= 0x80;    // D9 = PH6
    return data;
#endif // #ifndef IS_MEGA
}


// Write a byte to the data bus.  The caller must set the bus to output_mode
// before calling this or no data will be written.
void PromDevice::writeDataBus(byte data)
{
#ifndef IS_MEGA
     PORTB = (PORTB & 0xfc) | (data >> 6);
     PORTD = (PORTD & 0x03) | (data << 2);
#else // #ifndef IS_MEGA
    uint8_t portE = PORTE & ~0x38; // clear PE3,4,5
    if (data & 0x01) portE |= 0x10; // bit0 -> PE4 (D2)
    if (data & 0x02) portE |= 0x20; // bit1 -> PE5 (D3)
    if (data & 0x08) portE |= 0x08; // bit3 -> PE3 (D5)
    PORTE = portE;

    uint8_t portG = PORTG & ~0x20; // clear PG5
    if (data & 0x04) portG |= 0x20; // bit2 -> PG5 (D4)
    PORTG = portG;

    uint8_t portH = PORTH & ~0x78; // clear PH3..PH6
    if (data & 0x10) portH |= 0x08; // bit4 -> PH3 (D6)
    if (data & 0x20) portH |= 0x10; // bit5 -> PH4 (D7)
    if (data & 0x40) portH |= 0x20; // bit6 -> PH5 (D8)
    if (data & 0x80) portH |= 0x40; // bit7 -> PH6 (D9)
    PORTH = portH;
#endif // #ifndef IS_MEGA
}
