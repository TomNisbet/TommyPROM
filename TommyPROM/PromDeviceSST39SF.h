#ifndef INCLUDE_PROM_DEVICE_SST39SF_H
#define INCLUDE_PROM_DEVICE_SST39SF_H

#include "Arduino.h"
#include "PromDevice.h"

/*****************************************************************************/
/*****************************************************************************/
/**
 * PromDeviceSST39SF class
 *
 * Provides the device-specific interface to read and write data from an
 * SST39SF series parallel NOR Flash using the Arduino.
 */
class PromDeviceSST39SF : public PromDevice
{
  public:
    PromDeviceSST39SF(uint32_t size, word unsigned maxWriteTime, bool polling);
    void begin();
    const char * getName() { return "SST39SF series NOR Flash"; }
    ERET erase(uint32_t start, uint32_t end);


  protected:
    void setAddress(uint32_t address);
    byte readByte(uint32_t address);
    bool burnByte(byte value, uint32_t address);
    bool waitForWriteCycleEnd(byte lastValue);
    void setByte(byte value, uint32_t address);
    void eraseSector(uint32_t addr);

    uint32_t currentSector;

};

#endif  // #define INCLUDE_PROM_DEVICE_SST39SF_H
