#include "Configure.h"
#if defined(PROM_IS_8755A)


#define CE1         A0
#define RD          A1
#define CE2         A2
#define AD10        A3
#define AD9         A4
#define AD8         A5
#define ALE         10
#define VDDCTL      12


PromDevice8755A::PromDevice8755A(unsigned long size)
    : PromDevice(size, 0, 0, false)
{
}


void PromDevice8755A::begin()
{
    // Define the data bus as input initially so that it does not put out a
    // signal that could collide with output on the data pins of the EEPROM.
    setDataBusMode(INPUT);

    // Define the EEPROM control pins as output, making sure they are all
    // in the disabled state.
    digitalWrite(RD, HIGH);
    pinMode(RD, OUTPUT);
    digitalWrite(VDDCTL, LOW);
    pinMode(VDDCTL, OUTPUT);
    digitalWrite(CE1, LOW);
    pinMode(CE1, OUTPUT);
    digitalWrite(CE2, LOW);
    pinMode(CE2, OUTPUT);
    digitalWrite(ALE, LOW);
    pinMode(ALE, OUTPUT);

    // The address control pins are always outputs.
    pinMode(AD8, OUTPUT);
    pinMode(AD9, OUTPUT);
    pinMode(AD10, OUTPUT);
    digitalWrite(AD8, LOW);
    digitalWrite(AD9, LOW);
    digitalWrite(AD10, LOW);
}


// BEGIN PRIVATE METHODS
//

// Set an 11 bit address using the 8 address/data bus lines and three more dedicated
// address lines.  The read and burn code will take care of the ALE line
void PromDevice8755A::setAddress(word address)
{
    setDataBusMode(OUTPUT);
    writeDataBus(byte(address & 0xff));
    digitalWrite(AD8, address & 0x100  ? HIGH : LOW);
    digitalWrite(AD9, address & 0x200  ? HIGH : LOW);
    digitalWrite(AD10, address & 0x400 ? HIGH : LOW);
}


// Read a byte from a given address
byte PromDevice8755A::readByte(word address)
{
    byte data = 0;
    digitalWrite(RD, HIGH);
    digitalWrite(CE1, LOW);

	// Put the address on the bus and latch it with ALE
    digitalWrite(CE2, HIGH);
    setAddress(address);
	digitalWrite(ALE, HIGH);
	digitalWrite(ALE, LOW);

	// Read a byte
    setDataBusMode(INPUT);
    digitalWrite(RD, LOW);
    delayMicroseconds(1);
    data = readDataBus();
    digitalWrite(RD, HIGH);
    digitalWrite(CE2, LOW);

    return data;
}


// Burn a byte to the chip and verify that it was written.
bool PromDevice8755A::burnByte(byte value, word address)
{
    // Latch the address and the CE lines
    digitalWrite(ALE, HIGH);
    digitalWrite(CE1, LOW);
    digitalWrite(CE2, HIGH);
    setAddress(address);
    digitalWrite(ALE, LOW);

    // Burn the byte value by setting CE1 high and then setting VDD to +25V for 50ms.
    setDataBusMode(OUTPUT);
    writeDataBus(value);
    digitalWrite(CE1, HIGH);
    digitalWrite(VDDCTL, HIGH);
    delay(50);
    digitalWrite(VDDCTL, LOW);
    digitalWrite(CE1, LOW);

    // Read back the value and return success if it matches
    return readByte(address) == value;
}

#endif // #if defined(PROM_IS_8755A)

