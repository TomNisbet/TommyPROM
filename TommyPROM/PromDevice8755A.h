#ifndef INCLUDE_PROM_DEVICE_8755A_H
#define INCLUDE_PROM_DEVICE_8755A_H

#include "Arduino.h"
#include "PromDevice.h"

/*****************************************************************************/
/*****************************************************************************/
/**
 * PromDevice8755A class
 *
 * Provides the device-specific interface to read and write data from an
 * Intel 8755A parallel EPROM using the Arduino.
 *
 * In addition to the 8755A, the Arduino also controls a voltage switching
 * circuit that provides the required programming pulses on the 8755A's Vdd.
 */
class PromDevice8755A : public PromDevice
{
  public:
    PromDevice8755A(unsigned long size);
    void begin();
    const char * getName() { return "Intel 8755A EPROM"; }

  protected:
    void setAddress(word address);
    byte readByte(word address);
    bool burnByte(byte value, word address);
};


#endif  // #define INCLUDE_PROM_DEVICE_8755A_H

