#ifndef INCLUDE_PROM_DEVICE_8755_H
#define INCLUDE_PROM_DEVICE_8755_H

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
class PromDevice8755 : public PromDevice
{
  public:
    PromDevice8755(unsigned long size);
    void begin();

  protected:
    void setAddress(word address);
    byte readByte(word address);
    bool burnByte(byte value, word address);
};


#endif  // #define INCLUDE_PROM_DEVICE_8755_H

