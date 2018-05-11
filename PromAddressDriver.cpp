#include "PromAddressDriver.h"

#define ADDR_CLK_HI     A3
#define ADDR_CLK_LO     A4
#define ADDR_DATA       A5


void PromAddressDriver::begin()
{
    // The address control pins are always outputs.
    pinMode(ADDR_DATA, OUTPUT);
    pinMode(ADDR_CLK_LO, OUTPUT);
    pinMode(ADDR_CLK_HI, OUTPUT);
    digitalWrite(ADDR_DATA, LOW);
    digitalWrite(ADDR_CLK_LO, LOW);
    digitalWrite(ADDR_CLK_HI, LOW);


    // To save time, the setAddress only writes the hi byte if it has changed.
    // The value used to detect the change is initialized to a non-zero value,
    // so set an initial address to avoid the the case where the first address
    // written is the 'magic' initial address.
    setAddress(0x0000);
}


// Set a 16 bit address in the two address shift registers.
void PromAddressDriver::setAddress(word address)
{
    static byte lastHi = 0xca;
    byte hi = address >> 8;
    byte lo = address & 0xff;

    if (hi != lastHi)
    {
        setAddressRegister(ADDR_CLK_HI, hi);
        lastHi = hi;
    }
    setAddressRegister(ADDR_CLK_LO, lo);
}


// Shift an 8-bit value into one of the address shift registers.  Note that
// the data pins are tied together, selecting the high or low address register
// is a matter of using the correct clock pin to shift the data in.
void PromAddressDriver::setAddressRegister(uint8_t clkPin, byte addr)
{
    // Make sure the clock is low to start.
    digitalWrite(clkPin, LOW);

    // Shift 8 bits in, starting with the MSB.
    for (int ix = 0; (ix < 8); ix++)
    {
        // Set the data bit
        if (addr & 0x80)
        {
            digitalWrite(ADDR_DATA, HIGH);
        }
        else
        {
            digitalWrite(ADDR_DATA, LOW);
        }

        digitalWrite(clkPin, HIGH); // Clock in a bit
        digitalWrite(clkPin, LOW);  // Reset the clock pin
        addr <<= 1;
    }
}


