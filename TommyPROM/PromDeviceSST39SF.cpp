#include "Configure.h"
#if defined(PROM_IS_SST39SF)

#include "PromAddressDriver.h"

// IO lines for the EEPROM device control
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


PromDeviceSST39SF::PromDeviceSST39SF(uint32_t size, unsigned maxWriteTime, bool polling)
    : PromDevice(size, 0, maxWriteTime, polling),
      currentSector(0xffffffff)
{
}


void PromDeviceSST39SF::begin()
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


// Erase all 4K byte sectors containing the specified address range.
ERET PromDeviceSST39SF::erase(uint32_t start, uint32_t end)
{
    start >>= 12;
    end >>= 12;
    for (uint32_t sector = start; (sector <= end); sector++)
    {
        eraseSector(sector << 12);
    }

    return RET_OK;
}




// BEGIN PRIVATE METHODS
//

// Use the PromAddressDriver to set an address in the two address shift registers.
void PromDeviceSST39SF::setAddress(uint32_t address)
{
    PromAddressDriver::setAddress(address);
}


// Read a byte from a given address
byte PromDeviceSST39SF::readByte(uint32_t address)
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
bool PromDeviceSST39SF::burnByte(byte value, uint32_t address)
{
    bool status = false;

    // Erase a sector before writing any new data to it.  Note that multiple
    // burnByte calls to the same sector will only do an erase on the first call.
    // If multiple burn calls will be needed for the same address, it is up to the
    // caller to erase the sector before writing.
    if ((address & 0xfffff000) != currentSector)
    {
        eraseSector(address);
        currentSector = address & 0xfffff000;
    }
    disableOutput();
    disableWrite();

    setDataBusMode(OUTPUT);
    enableChip();
    setByte(0xaa, 0x5555);
    setByte(0x55, 0x2aaa);
    setByte(0xa0, 0x5555);

    setAddress(address);
    setDataBusMode(OUTPUT);
    writeDataBus(value);

    delayMicroseconds(1);
    enableWrite();
    delayMicroseconds(1);
    disableWrite();

    status = waitForWriteCycleEnd(value);

    disableChip();

    return status;
}


bool PromDeviceSST39SF::waitForWriteCycleEnd(byte lastValue)
{
    if (mSupportsDataPoll)
    {
        // Verify programming complete by reading the last value back until it matches the
        // value written twice in a row.  The D7 bit will read the inverse of last written
        // data and the D6 bit will toggle on each read while in programming mode.
        //
        // This loop code takes about 18uSec to execute.  The max readcount is set to the
        // device's maxReadTime (in uSecs) divided by ten rather than eighteen to ensure
        // that it runs at least as long as the chip's timeout value, even if some code
        // optimizations are made later. In actual practice, the loop will terminate much
        // earlier because it will detect the end of the write well before the max time.
        byte b1=0, b2=0;
        setDataBusMode(INPUT);
        delayMicroseconds(1);
        for (int readCount = 1; (readCount < (mMaxWriteTime * 100)); readCount++)
        {
            enableChip();
            enableOutput();
            delayMicroseconds(1);
            b1 = readDataBus();
            disableOutput();
            disableChip();
            enableChip();
            enableOutput();
            delayMicroseconds(1);
            b2 = readDataBus();
            disableOutput();
            disableChip();
            if ((b1 == b2) && (b1 == lastValue))
            {
                return true;
            }
        }

        debugLastExpected = lastValue;
        debugLastReadback = b2;
        return false;
    }
    else
    {
        // No way to detect success.  Just wait the max write time.
        delayMicroseconds(mMaxWriteTime * 1000L);
        return true;
    }
}


// Set an address and data value and toggle the write control.  This is used to write
// control sequences, like the sector erase.  This is not a complete byte write function
// because it does not set the chip enable or the mode of the data bus.
void PromDeviceSST39SF::setByte(byte value, uint32_t address)
{
    setAddress(address);
    writeDataBus(value);

    delayMicroseconds(1);
    enableWrite();
    delayMicroseconds(1);
    disableWrite();
}

void PromDeviceSST39SF::eraseSector(uint32_t addr)
{
    disableOutput();
    disableWrite();
    setDataBusMode(OUTPUT);
    enableChip();
    setByte(0xaa, 0x5555);
    setByte(0x55, 0x2aaa);
    setByte(0x80, 0x5555);
    setByte(0xaa, 0x5555);
    setByte(0x55, 0x2aaa);
    setByte(0x30, addr & 0xfffff000);
    delay(28);
    disableChip();
}

#endif // #if defined(PROM_IS_SST39SF)
