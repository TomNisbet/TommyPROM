// This controls the shift register that generates the address lines for A0..A15 for most
// chip families.  This is not used by the PromDevice8755A code.
//
// Note that this uses direct port control instead of digitalWrite calls so that the code
// can run fast enough to meet the tBLC requirements for SDP and block writes.  This
// sacrifices portability and readability for speed. //
//
// This code will only work on Arduino Uno and Nano hardware.  The ports for other
// Arduinos map to different IO pins.

#include "PromAddressDriver.h"

#define ADDR_CLK_HI     A3
#define ADDR_CLK_LO     A4
#define ADDR_DATA       A5

// Define masks for the address clk and data lines on PC3..PC5 for direct port control.
#define ADDR_CLK_HI_MASK    0x08
#define ADDR_CLK_LO_MASK    0x10
#define ADDR_DATA_MASK      0x20

// For larger ROMs, address lines A16..A18 are controlled by D10..D12 (PB2..PB4).
#define UPPER_ADDR_MASK     0x1c

// When using the 74LS595 shift registers, the RCLK lines of both shift registers can be
// connected to D13 (PB5).  Uncomment the #define SHIFT_REGISTER_IS_595 in Configure.h to
// enable the code for this.
#define RCLK_595_MASK       0x20


void PromAddressDriver::begin()
{
    // The address control pins are always outputs.
    pinMode(ADDR_DATA, OUTPUT);
    pinMode(ADDR_CLK_LO, OUTPUT);
    pinMode(ADDR_CLK_HI, OUTPUT);
    digitalWrite(ADDR_DATA, LOW);
    digitalWrite(ADDR_CLK_LO, LOW);
    digitalWrite(ADDR_CLK_HI, LOW);
    DDRB |= UPPER_ADDR_MASK | RCLK_595_MASK; // Set D10..D13 as outputs

    // To save time, the setAddress only writes the hi byte if it has changed.
    // The value used to detect the change is initialized to a non-zero value,
    // so set an initial address to avoid the the case where the first address
    // written is the 'magic' initial address.
    setAddress(0x0000);
}


// Set a 16 bit address in the two address shift registers and
// the upper bits on the extened address pins.
void PromAddressDriver::setAddress(uint32_t address)
{
    static byte lastHi = 0xca;
    static byte lastUpper = 0xca;
    byte upper = (address >> 16) & 0xff;
    byte hi = (address >> 8) & 0xff;
    byte lo = address & 0xff;

    if (upper != lastUpper)
    {
        setUpperAddress(upper);
        lastUpper = upper;
    }
    if (hi != lastHi)
    {
        setAddressRegister(ADDR_CLK_HI, hi);
        lastHi = hi;
    }
    setAddressRegister(ADDR_CLK_LO, lo);
}


void PromAddressDriver::setUpperAddress(byte addr)
{
    // Set the upper address on pins D10..D12.
    PORTB = (PORTB & ~UPPER_ADDR_MASK) | ((addr << 2) & UPPER_ADDR_MASK);
}


// Shift an 8-bit value into one of the address shift registers.  Note that
// the data pins are tied together, selecting the high or low address register
// is a matter of using the correct clock pin to shift the data in.
void PromAddressDriver::setAddressRegister(uint8_t clkPin, byte addr)
{
    byte mask = 0;
    if (clkPin == ADDR_CLK_HI)
        mask = ADDR_CLK_HI_MASK;
    else if (clkPin == ADDR_CLK_LO)
        mask = ADDR_CLK_LO_MASK;

    // Make sure the clock is low to start.
    PORTC &= ~mask;

    // Shift 8 bits in, starting with the MSB.
    for (int ix = 0; (ix < 8); ix++)
    {
        // Set the data bit
        if (addr & 0x80)
        {
            PORTC |= ADDR_DATA_MASK;
        }
        else
        {
            PORTC &= ~ADDR_DATA_MASK;
        }

        // Toggle the clock high then low
        PORTC |= mask;
        delayMicroseconds(3);
        PORTC &= ~mask;
        addr <<= 1;
    }

    // Toggle the RCLK pin to output the data for 74LS595 shift registers.  This pin is
    // not connected when using 74LS164 shift registers.
    PORTB &= ~RCLK_595_MASK;
    delayMicroseconds(1);
    PORTB |= RCLK_595_MASK;
    delayMicroseconds(1);
    PORTB &= ~RCLK_595_MASK;
}
