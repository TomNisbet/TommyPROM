#include "Configure.h"
#include "PromDevice.h"


PromDevice::PromDevice(unsigned long size, word blockSize, unsigned maxWriteTime, bool polling)
    : mSize(size),
      mBlockSize(blockSize),
      mMaxWriteTime(maxWriteTime),
      mSupportsDataPoll(polling)
{
}


// Write a block of data to the device.  If the device supports block writes,
// the data will be broken into chunks and written using the block mode.
// Otherwise, each byte will be individually written and verified.
bool PromDevice::writeData(byte data[], word len, word address)
{
    bool status = true;

    if (mBlockSize == 0)
    {
        // Device does not support block writes.
        for (word ix = 0; (ix < len); ix++)
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
        word offset = 0;
        word chunkSize;
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


// BEGIN PRIVATE METHODS
//

// Set the I/O state of the data bus.
// The first two bits of port D are used for serial, so the 8 bits data bus are
// on pins D2..D9.
void PromDevice::setDataBusMode(uint8_t mode)
{
#if defined(ARDUINO_IS_UNO) || defined(ARDUINO_IS_NANO)
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
#elif defined(ARDUINO_IS_MICRO)
    // On the Micro, D2..D9 maps to the upper 7 bits of port B and the
    // lower bit of port D.
    if (mode == OUTPUT)
    {
        DDRB |= 0xfe;
        DDRD |= 0x01;
    }
        else
    {
        DDRB &= 0x01;
        DDRD &= 0xfe;
    }
#else
    byte bit = 0x01;
    for (int pin = 2; (pin <= 9); pin++) {
        pinMode(pin, mode);
        bit <<= 1;
    }
#endif
}


// Read a byte from the data bus.  The caller must set the bus to input_mode
// before calling this or no useful data will be returned.
byte PromDevice::readDataBus()
{
#if defined(ARDUINO_IS_UNO) || defined(ARDUINO_IS_NANO)
    return (PINB << 6) | (PIND >> 2);
#elif defined(ARDUINO_IS_MICRO)
    return (PINB & 0xfe) | (PIND & 0x01);
#else
    byte data = 0;
    byte bit = 0x01;
    for (int pin = 2; (pin <= 9); pin++) {
        if (digitalRead(pin) == HIGH) {
            data |= bit;
        }
        bit <<= 1;
    }
    return data;
#endif
}


// Write a byte to the data bus.  The caller must set the bus to output_mode
// before calling this or no data will be written.
void PromDevice::writeDataBus(byte data)
{
#if defined(ARDUINO_IS_UNO) || defined(ARDUINO_IS_NANO)
     PORTB = (PORTB & 0xfc) | (data >> 6);
     PORTD = (PORTD & 0x03) | (data << 2);
#elif defined(ARDUINO_IS_MICRO)
    PORTB = (PORTB & 0x01) | (data & 0xfe);
    PORTD = (PORTD & 0xfe) | (data & 0x01);
#else
    byte bit = 0x01;
    for (int pin = 2; (pin <= 9); pin++) {
        digitalWrite(pin, (data & bit) ? HIGH : LOW);
        bit <<= 1;
    }
#endif
}


