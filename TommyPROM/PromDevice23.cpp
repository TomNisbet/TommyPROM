// NOTE - The 23 series device support is a work in progress.  It
//        has not been tested or documented.

#include "Configure.h"
#if defined(PROM_IS_23)

#include "PromAddressDriver.h"

// IO lines for the EPROM device control
// Pins D2..D9 are used for the data bus.
#define CS3              A0
#define CS1              A1
#define CS2              A2

// Set the status of the device control pins
static void enableCS1()     { digitalWrite(CS1, HIGH); }
static void disableCS1()    { digitalWrite(CS1, LOW);}
static void enableCS2()     { digitalWrite(CS2, HIGH); }
static void disableCS2()    { digitalWrite(CS2, LOW);}
static void enableCS3()     { digitalWrite(CS3, HIGH); }
static void disableCS3()    { digitalWrite(CS3, LOW);}


PromDevice23::PromDevice23(uint32_t size)
    : PromDevice(size, 0, 0, false)
{
}


void PromDevice23::begin()
{
    // Define the data bus as input initially so that it does not put out a
    // signal that could collide with output on the data pins of the EEPROM.
    setDataBusMode(INPUT);

    pinMode(CS1, OUTPUT);
    pinMode(CS2, OUTPUT);
    pinMode(CS3, OUTPUT);
    
    disableCS1();
    disableCS2();
    disableCS3();

    // This chip uses the shift register hardware for addresses, so initialize that.
    PromAddressDriver::begin();
}


// BEGIN PRIVATE METHODS
//

// Use the PromAddressDriver to set a 16 bit address in the two address shift registers.
void PromDevice23::setAddress(uint32_t address)
{
    PromAddressDriver::setAddress(address);
}


// Read a byte from a given address
byte PromDevice23::readByte(uint32_t address)
{
    byte data = 0;
    setAddress(address);
    setDataBusMode(INPUT);
    enableCS1();
    enableCS2();
    enableCS3();

    delayMicroseconds(20);
    data = readDataBus();

    disableCS3();
    disableCS2();
    disableCS1();

    return data;
}

#endif // #if defined(PROM_IS_23)
