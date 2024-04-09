// NOTE - The 23 series device support is a work in progress.  It
//        has not been tested or documented.


#ifndef INCLUDE_PROM_DEVICE_23_H
#define INCLUDE_PROM_DEVICE_23_H

#include "Arduino.h"
#include "PromDevice.h"

/*****************************************************************************/
/*****************************************************************************/
/**
 * PromDevice23 class
 *
 * Provides the device-specific interface to read data from a
 * 23 series parallel PROM using the Arduino.  Supported chips
 * include 2316.
 */

class PromDevice23 : public PromDevice
{
  public:
    PromDevice23(uint32_t size);
    void begin();
    const char *getName() { return "23 series PROM"; }
    ERET disableSoftwareWriteProtect();  // use the Unlock command to scan the chip selects

  protected:
    void printCSbits(unsigned bits);
    bool scanAddress(uint32_t addrBase);
    void printByte(byte b);
    void setAddress(uint32_t address);
    byte readByte(uint32_t address);
    bool burnByte(byte value, uint32_t address) { return false; }
};

#endif  // #define INCLUDE_PROM_DEVICE_23_H
