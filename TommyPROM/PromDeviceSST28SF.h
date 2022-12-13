#ifndef INCLUDE_PROM_DEVICE_SST28SF_H
#define INCLUDE_PROM_DEVICE_SST28SF_H

#include "Arduino.h"
#include "PromDevice.h"

/*****************************************************************************/
/*****************************************************************************/
/**
 * PromDeviceSST28SF class
 *
 * Provides the device-specific interface to read and write data from an
 * SST28SF series parallel SuperFlash using the Arduino.
 */
class PromDeviceSST28SF : public PromDevice
{
  public:
    PromDeviceSST28SF(uint32_t size, word unsigned maxWriteTime, bool polling);
    void begin();
    const char * getName() { return "SST28SF series SuperFlash"; }
    ERET disableSoftwareWriteProtect();
    ERET enableSoftwareWriteProtect();
    ERET erase(uint32_t start, uint32_t end);


  protected:
    void setAddress(uint32_t address);
    byte readByte(uint32_t address);
    bool burnByte(byte value, uint32_t address);
    bool waitForWriteCycleEnd(byte lastValue);
    void setByte(byte value, uint32_t address);
    void getByte(uint32_t address);
    void eraseSector(uint32_t addr);
    ERET sendSdpSequence(uint16_t lastAddress);


    uint32_t currentSector;

};

#endif  // #define INCLUDE_PROM_DEVICE_SST28SF_H
