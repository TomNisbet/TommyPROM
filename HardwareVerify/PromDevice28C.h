#ifndef INCLUDE_PROM_DEVICE_28C_H
#define INCLUDE_PROM_DEVICE_28C_H

#include "Arduino.h"
#include "PromDevice.h"

/*****************************************************************************/
/*****************************************************************************/
/**
 * PromDevice28C class
 *
 * Provides the device-specific interface to read and write data from a
 * 28C series parallel EEPROM using the Arduino.
 *
 * Block writes are supported on compatible devices by specifying a blockSize
 * in the constructor.  Use zero for byte writes.
 */

// IO lines for the EEPROM device control
// Pins D2..D9 are used for the data bus.
#define WE              A0
#define CE              A1
#define OE              A2

class PromDevice28C : public PromDevice
{
  public:
    PromDevice28C(unsigned long size, word blockSize, unsigned maxWriteTime, bool polling);
    void begin();
    const char * getName() { return "28C series EEPROM"; }
    void disableSoftwareWriteProtect();
    void enableSoftwareWriteProtect();

  //protected:
    void setAddress(word address);
    byte readByte(word address);
    bool burnByte(byte value, word address);
    bool burnBlock(byte data[], word len, word address);
    bool waitForWriteCycleEnd(byte lastValue);
    void setByte(byte value, word address);


    // Set the status of the device control pins
    void enableChip()       { digitalWrite(CE, LOW); }
    void disableChip()      { digitalWrite(CE, HIGH);}
    void enableOutput()     { digitalWrite(OE, LOW); }
    void disableOutput()    { digitalWrite(OE, HIGH);}
    void enableWrite()      { digitalWrite(WE, LOW);}
    void disableWrite()     { digitalWrite(WE, HIGH);}
};

#endif  // #define INCLUDE_PROM_DEVICE_28C_H

