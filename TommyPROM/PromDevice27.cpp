// NOTE - The 27 series device support is a work in progress.  It
//        has not been tested or documented.

#include "Configure.h"
#if defined(PROM_IS_27)

#include "PromAddressDriver.h"

// IO lines for the EPROM device control
// Pins D2..D9 are used for the data bus.
#define WE              A0
#define CE              A1
#define OE              A2

// Set the status of the device control pins
static void enableChip()       { digitalWrite(CE, LOW); }
static void disableChip()      { digitalWrite(CE, HIGH);}
static void enableOutput()     { digitalWrite(OE, LOW); }
static void disableOutput()    { digitalWrite(OE, HIGH);}
static void enableWrite()      { digitalWrite(WE, LOW); }
static void disableWrite()     { digitalWrite(WE, HIGH);}


PromDevice27::PromDevice27(uint32_t size, unsigned long pulseWidthUsec,
                           unsigned writeAttempts, unsigned overwriteMultiplier)
    : PromDevice(size, 0, 0, false),
      mPulseWidthUsec(pulseWidthUsec),
      mWriteAttempts(writeAttempts),
      mOverwriteMultiplier(overwriteMultiplier)
{
}


void PromDevice27::begin()
{
    // Define the data bus as input initially so that it does not put out a
    // signal that could collide with output on the data pins of the EEPROM.
    setDataBusMode(INPUT);

    // Define the EEPROM control pins as output, making sure they are all
    // in the disabled state.
    digitalWrite(OE, HIGH);
    pinMode(OE, OUTPUT);
    digitalWrite(CE, HIGH);
    pinMode(CE, OUTPUT);
    digitalWrite(WE, HIGH);
    pinMode(WE, OUTPUT);

    // This chip uses the shift register hardware for addresses, so initialize that.
    PromAddressDriver::begin();
}


// BEGIN PRIVATE METHODS
//

// Use the PromAddressDriver to set a 16 bit address in the two address shift registers.
void PromDevice27::setAddress(uint32_t address)
{
    PromAddressDriver::setAddress(address);
}


// Read a byte from a given address
byte PromDevice27::readByte(uint32_t address)
{
    byte data = 0;
    setAddress(address);
    setDataBusMode(INPUT);
    disableOutput();
    disableWrite();
    enableChip();
    enableOutput();
    data = readDataBus();
    disableOutput();
    disableChip();

    return data;
}


// Burn a byte to the chip and verify that it was written.
bool PromDevice27::burnByte(byte value, uint32_t address)
{
    bool status = false;
    unsigned writeCount = 0;

    byte data = 0;
    disableOutput();
    disableWrite();
    enableChip();
    setAddress(address);

    while (!status && (writeCount < mWriteAttempts))
    {
        setDataBusMode(OUTPUT);
        writeDataBus(value);
        delayMicroseconds(1);
        enableWrite();
        myDelay(mPulseWidthUsec);
        disableWrite();
        ++writeCount;

        setDataBusMode(INPUT);
        enableOutput();
        data = readDataBus();
        disableOutput();
        status = (readDataBus() == value);
    }

    if (status && (mOverwriteMultiplier > 0))
    {
        setDataBusMode(OUTPUT);
        writeDataBus(value);
        delayMicroseconds(1);
        enableWrite();
        myDelay(mPulseWidthUsec * mOverwriteMultiplier);
        disableWrite();
    }

    disableChip();

    return status;
}

void PromDevice27::myDelay(unsigned int us)
{
    if (us > 16000)
    {
        // The delayMicroseconds code can't do delays longer than 16ms, so use the
        // ms delay code for larger values.  This rounds down to the nearest ms, so
        // it is not possible to delay for 40.5 ms, for example.
        delay(us / 1000);
    }
    else
    {
        delayMicroseconds((unsigned int) us);
    }
}


#endif // #if defined(PROM_IS_27)

