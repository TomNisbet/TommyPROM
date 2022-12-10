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
class PromDevice28C : public PromDevice
{
  public:
    PromDevice28C(uint32_t size, word blockSize, unsigned maxWriteTime, bool polling);
    void begin();
    const char * getName() { return "28C series EEPROM"; }
    ERET disableSoftwareWriteProtect();
    ERET enableSoftwareWriteProtect();

  protected:
    void setAddress(uint32_t address);
    byte readByte(uint32_t address);
    bool burnByte(byte value, uint32_t address);
    bool burnBlock(byte data[], uint32_t len, uint32_t address);
    bool waitForWriteCycleEnd(byte lastValue);
    void setByte(byte value, uint32_t address);
};

#endif  // #define INCLUDE_PROM_DEVICE_28C_H
