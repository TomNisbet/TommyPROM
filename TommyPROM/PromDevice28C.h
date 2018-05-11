#ifndef INCLUDE_PROM_DEVICE_28C_H
#define INCLUDE_PROM_DEVICE_28C_H

#include "Arduino.h"
#include "PromDevice.h"

/*****************************************************************************/
/*****************************************************************************/
/**
 * PromDevice class
 *
 * Provides the interface to read and write data from a parallel PROM using the
 * Arduino.
 *
 * Block writes are supported on compatible devices by specifying a blockSize
 * in the constructor.  Use zero for byte writes.
 */
class PromDevice28C : public PromDevice
{
  public:
    PromDevice28C(unsigned long size, word blockSize, unsigned maxWriteTime, bool polling);
    void begin();
    void disableSoftwareWriteProtect();

  protected:
    void setAddress(word address);
    byte readByte(word address);
    bool burnByte(byte value, word address);
    bool burnBlock(byte data[], word len, word address);
    bool waitForWriteCycleEnd(byte lastValue);
    void setByte(byte value, word address);
};

#endif  // #define INCLUDE_PROM_DEVICE_28C_H

