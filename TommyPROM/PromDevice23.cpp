// NOTE - The 23 series device support is a work in progress.  It
//        has not been tested or documented.

#include "Configure.h"
#if defined(PROM_IS_23)

#include "PromAddressDriver.h"

// IO lines for the EPROM device control
// Pins D2..D9 are used for the data bus.
enum {
    CS3_PIN = A0,   // WE on 28C schematic
    CS2_PIN = A1,   // CE on 28C schematic
    CS1_PIN = A2    // OE on 28C schematic
};
enum {
    CS3_BIT = 0x04,
    CS2_BIT = 0x02,
    CS1_BIT = 0x01
};

static unsigned csBits;

// Set the status of the device control pins
static void enableCS(unsigned pin,  unsigned maskBit) { digitalWrite(pin, (csBits & maskBit) ? HIGH : LOW); }
static void disableCS(unsigned pin, unsigned maskBit) { digitalWrite(pin, (csBits & maskBit) ? LOW : HIGH); }

static void enableCS1()     { enableCS(CS1_PIN,  CS1_BIT); }
static void disableCS1()    { disableCS(CS1_PIN, CS1_BIT); }
static void enableCS2()     { enableCS(CS2_PIN,  CS2_BIT); }
static void disableCS2()    { disableCS(CS2_PIN, CS2_BIT); }
static void enableCS3()     { enableCS(CS3_PIN,  CS3_BIT); }
static void disableCS3()    { disableCS(CS3_PIN, CS3_BIT); }


PromDevice23::PromDevice23(uint32_t size)
    : PromDevice(size, 0, 0, false)
{
}


void PromDevice23::begin()
{
    // Define the data bus as input initially so that it does not put out a
    // signal that could collide with output on the data pins of the EEPROM.
    setDataBusMode(INPUT_PULLUP);

    csBits = 0;  // Init to CS3, CS2, and CS1 all active LOW
    pinMode(CS1_PIN, OUTPUT);
    pinMode(CS2_PIN, OUTPUT);
    pinMode(CS3_PIN, OUTPUT);

    disableCS1();
    disableCS2();
    disableCS3();

    // This chip uses the shift register hardware for addresses, so initialize that.
    PromAddressDriver::begin();
}


// Override the existing Unlock command to scan the different combinations of CS until
// data is found.  If the CS lines are not correctly configured, the chip should not
// assert any signals on the data bus and the Arduino will read back all FF.
ERET PromDevice23::disableSoftwareWriteProtect()
{
    // Finish out the Unlock message printed by the UI code
    Serial.println(F("no unlock needed, scanning chip selects..."));

    // Scan a few different addresses in case there are legitimate empty blocks.  With
    // the total ROM size of 2K, it is very unlikely that there are several unused
    // 16-byte blocks, so a valid set of CS pins should encounter some data.
    for (uint32_t addrBase = 0x0000; (addrBase < mSize); addrBase += 0x250)
    {
        if (scanAddress(addrBase))
        {
            Serial.print(F("Setting CS bits to "));
            printCSbits(csBits);
            Serial.println();
            return RET_OK;
        }
    }

    Serial.println(F("No valid CS settings found"));

    return RET_FAIL;
}


// BEGIN PRIVATE METHODS
//

// Print the current CS bit settings.
void PromDevice23::printCSbits(unsigned bits)
{
        Serial.print("CS3/WE:");
        Serial.print((bits & CS3_BIT) ? 'H':'L');
        Serial.print("  CS2/CE:");
        Serial.print((bits & CS2_BIT) ? 'H':'L');
        Serial.print("  CS1/OE:");
        Serial.print((bits & CS1_BIT) ? 'H':'L');
}


// Try all combinations of the CS bits and read 16 bytes of data from the chip.
// One of the CS settings should read some non-FF data.
bool PromDevice23::scanAddress(uint32_t addrBase)
{
    unsigned saveBits = 0xff;

    Serial.print(F("\nScanning Chip Select combinations starting at address "));
    printByte(addrBase >> 8);
    printByte(addrBase & 0xff);
    Serial.println();

    for (csBits = 0; (csBits < 8); csBits++)
    {
        // Disable all CS bits using the new definition of csBits
        disableCS3();
        disableCS2();
        disableCS1();

        printCSbits(csBits);
        Serial.print(" - ");
        for (unsigned offset = 0; (offset < 16); offset++)
        {
            byte data = readByte(addrBase + offset);
            printByte(data);
            Serial.print(' ');
            if (data != 0xff)
            {
                // this combination of Chip Selects successfully read data
                saveBits = csBits;
            }
        }
        Serial.println();
    }

    if (saveBits != 0xff)
    {
        csBits = saveBits;
        return true;
    }
    return false;
}


static const char hex[] = "0123456789abcdef";

void PromDevice23::printByte(byte b)
{
    char line[3];

    line[0] = hex[b >> 4];
    line[1] = hex[b & 0x0f];
    line[2] = '\0';

    Serial.print(line);
}


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
    setDataBusMode(INPUT_PULLUP);
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
