#include "Configure.h"
#if defined(PROM_IS_SST28SF)

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


PromDeviceSST28SF::PromDeviceSST28SF(uint32_t size, unsigned maxWriteTime, bool polling)
    : PromDevice(size, 0, maxWriteTime, polling),
      currentSector(0xffffffff)
{
}


void PromDeviceSST28SF::begin()
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


// Turn off Software Data Protection.
ERET PromDeviceSST28SF::disableSoftwareWriteProtect()
{
    return sendSdpSequence(0x041a);
}

// Turn on Software Data Protection.
ERET PromDeviceSST28SF::enableSoftwareWriteProtect()
{
    return sendSdpSequence(0x040a);
}


// Erase all 256 byte sectors containing the specified address range.
ERET PromDeviceSST28SF::erase(uint32_t start, uint32_t end)
{
    start >>= 8;
    end >>= 8;
    for (uint32_t sector = start; (sector <= end); sector++)
    {
        eraseSector(sector << 8);
    }

    return RET_OK;
}


// BEGIN PRIVATE METHODS
//

// Use the PromAddressDriver to set an address in the two address shift registers.
void PromDeviceSST28SF::setAddress(uint32_t address)
{
    PromAddressDriver::setAddress(address);
}


// Read a byte from a given address
byte PromDeviceSST28SF::readByte(uint32_t address)
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
bool PromDeviceSST28SF::burnByte(byte value, uint32_t address)
{
    bool status = false;

    // Erase a sector before writing any new data to it.  Note that multiple
    // burbByte calls to the same sector will only do an erase on the first call.
    // If multiple burn calls will be needed for the same address, it is up to the
    // caller to erase the sector before writing.
    if ((address & 0xffffff00) != currentSector)
    {
        eraseSector(address);
        currentSector = address & 0xffffff00;
    }
    disableOutput();
    disableWrite();

    setDataBusMode(OUTPUT);
    enableChip();
    setByte(0x10, 0x0000);

    setAddress(address);
    writeDataBus(value);

    delayMicroseconds(1);
    enableWrite();
    delayMicroseconds(1);
    disableWrite();

    status = waitForWriteCycleEnd(value);

    disableChip();

    return status;
}


bool PromDeviceSST28SF::waitForWriteCycleEnd(byte lastValue)
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
        for (unsigned readCount = 1; (readCount < (mMaxWriteTime * 100)); readCount++)
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
void PromDeviceSST28SF::setByte(byte value, uint32_t address)
{
    setAddress(address);
    writeDataBus(value);

    delayMicroseconds(1);
    enableWrite();
    delayMicroseconds(1);
    disableWrite();
}

// Set an address value and toggle the read control.  This is used for control sequences,
// like the software write protect.  This is not a complete byte read function because it
// does not set the chip enable or the mode of the data bus.
void PromDeviceSST28SF::getByte(uint32_t address)
{
    setAddress(address);
    enableOutput();
    disableOutput();
}

void PromDeviceSST28SF::eraseSector(uint32_t addr)
{
    disableOutput();
    disableWrite();
    setDataBusMode(OUTPUT);
    enableChip();
    setByte(0x20, 0x0000);
    setByte(0xD0, addr & 0xffffff00);
    delay(4);
    disableChip();
}


// Software Data protection is enabled and disabled by reading a sequence of seven
// addresses.  The sequence only differs by the final address, so this common code is used
// for both commands.
//
// The 2001 SST datasheet is a bit unclear on the sequence.  The timing diagrams are
// labeled as "Unprotect Disable" and "Protect Disable", which would seem to be protect
// and unprotect, respectively.  However, the softare command summary table references the
// "Protect Disable" timing diagram for the Protect command.
ERET PromDeviceSST28SF::sendSdpSequence(uint16_t lastAddress)
{
    disableOutput();
    disableWrite();
    enableChip();
    setDataBusMode(INPUT);

    getByte(0x1823);
    getByte(0x1820);
    getByte(0x1822);
    getByte(0x0418);
    getByte(0x041b);
    getByte(0x0419);
    getByte(lastAddress);

    disableChip();

    return RET_OK;
}

#endif // #if defined(PROM_IS_SST28SF)
