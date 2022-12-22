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


PromDevice27::PromDevice27(uint32_t size, E27C_PGM pgmType, unsigned long pulseWidthUsec,
                           unsigned writeAttempts, unsigned overwriteMultiplier, bool verify)
    : PromDevice(size, 0, 0, false),
      mPgmType(pgmType),
      mPulseWidthUsec(pulseWidthUsec),
      mWriteAttempts(writeAttempts),
      mOverwriteMultiplier(overwriteMultiplier),
      mVerifyByte(verify)
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
    switch(mPgmType) {
    case E27C_PGM_WE:  return burnByteWE(value, address);  break;
    case E27C_PGM_CE:  return burnByteCE(value, address);  break;
    default: return false;  // PGM_D13 not implemented yet
    }
}


// Burn a byte to the chip and verify that it was written.
//
// This uses a dedicated WE or PGM chip that operates on TTL levels and is active LOW.
// Overwrite burning is supported.
bool PromDevice27::burnByteWE(byte value, uint32_t address)
{
    bool status = false;
    unsigned writeCount = 0;

    disableOutput();
    disableWrite();
    enableChip();
    setAddress(address);

    while (!status && (writeCount < mWriteAttempts)) {
        setDataBusMode(OUTPUT);
        writeDataBus(value);
        delayMicroseconds(1);
        enableWrite();
        myDelay(mPulseWidthUsec);
        disableWrite();
        ++writeCount;

        if (mVerifyByte) {
			setDataBusMode(INPUT);
	        enableOutput();
	        status = readDataBus() == value;
	        disableOutput();
		} else {
            status = true;
        }
    }

    if (status && (mOverwriteMultiplier > 0)) {
        setDataBusMode(OUTPUT);
        writeDataBus(value);
        delayMicroseconds(1);
        enableWrite();
        myDelay(mPulseWidthUsec * mOverwriteMultiplier);
        disableWrite();
    }

	setDataBusMode(INPUT);
    disableChip();
    return status;
}


// Burn a byte to the chip and verify that it was written.
//
// This uses an active LOW program pulse on the CE line and a verify operation with CE
// HIGH.  Overwrite is not supported, but could be added is a chip is found that needs it.
// Chips that use this mode require a programming voltage on the PGM or VPP pin and
// possibly on other pins as well  The above applies to the W27C257 EEPROM.  The W27C512
// is a bit more difficult because it does not have a dedicated VPP or PGM pin.  Instead,
// the programming voltage is applied to OE to put the chip in programming mode and OE is
// then switched LOW for the verify operation. Because the voltage switch would require
// additional hardware, this code does not support verify-after-write for the W27C512 chip
// and will instead just do a single write cycle.
//
// VCC may also have a non-standard voltage in program mode.  Be sure to separate the
// PROM's VCC line from system VCC if a non-standard voltage is used.
bool PromDevice27::burnByteCE(byte value, uint32_t address)
{
    bool status = false;
    unsigned writeCount = 0;

    disableOutput();
    disableWrite();
    disableChip();
    setAddress(address);

    while (!status && (writeCount < mWriteAttempts)) {
        setDataBusMode(OUTPUT);
        writeDataBus(value);
        delayMicroseconds(2);
        enableChip();
        myDelay(mPulseWidthUsec);
        disableChip();
        delayMicroseconds(2);
        ++writeCount;

        if (mVerifyByte) {
	        setDataBusMode(INPUT);
	        enableOutput();
	        status = readDataBus() == value;
	        disableOutput();
		}
    }

	setDataBusMode(INPUT);
	disableChip();
    return status;
}


ERET PromDevice27::erase(uint32_t start, uint32_t end)
{
	ERET status = RET_FAIL;

    disableChip();
    disableOutput();
    setAddress(0);
    setDataBusMode(OUTPUT);
    writeDataBus(0xff);
    delayMicroseconds(2);

    if (mPgmType == E27C_PGM_WE) {
		// Erase code for the SST27C0x0.  The Vpp and A9 pins are held at 12V for the
		// erase cycle.  This erases the entire chip, so the start and end address
		// parameters are ignored.  There is no erase verification for this chip.
		enableChip();
        delayMicroseconds(1);
		enableWrite();
		delayMicroseconds(100);	// Hard coded for SST27F020
		disableWrite();
		disableChip();
		setDataBusMode(INPUT);
		status = RET_OK;
    } else {
		// Erase code for the 27E257 and 27C257.  The Vpp and A9 pins are held at 14V for
		// the erase and verify cycle.  This erases the entire chip, so the start and end
		// address parameters are ignored.
	    unsigned writeCount = 0;
	    while ((status == RET_FAIL) && (writeCount < mWriteAttempts)) {
	        setAddress(0);
	        setDataBusMode(OUTPUT);
	        writeDataBus(0xff);
	        delayMicroseconds(2);
	        enableChip();
	        delay(100);
	        disableChip();
	        delayMicroseconds(2);

	        // Read back the data to verify all cells are erased.  Note That this is done
	        // while CE is HIGH.  This is the erase verify mode.
	        setDataBusMode(INPUT);
	        for (uint32_t address = 0; (address < mSize); address++) {
	            setAddress(address);
	            enableOutput();
	            uint8_t b = readDataBus();
	            disableOutput();
	            if (b != 0xff)  break;
	        }
	        status = RET_OK;
	    }
	}

    return status;
}


void PromDevice27::myDelay(unsigned int us)
{
    if (us > 16000) {
        // The delayMicroseconds code can't do delays longer than 16ms, so use the
        // ms delay code for larger values.  This rounds down to the nearest ms, so
        // it is not possible to delay for 40.5 ms, for example.
        delay(us / 1000);
    } else {
        delayMicroseconds((unsigned int) us);
    }
}


#endif // #if defined(PROM_IS_27)
