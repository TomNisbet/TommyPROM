// NOTE - The 27 series device support is a work in progress.  It
//        has not been tested or documented.


#ifndef INCLUDE_PROM_DEVICE_27_H
#define INCLUDE_PROM_DEVICE_27_H

#include "Arduino.h"
#include "PromDevice.h"

/*****************************************************************************/
/*****************************************************************************/
/**
 * PromDevice27 class
 *
 * Provides the device-specific interface to read and write data from a
 * 27 or 27C series parallel EPROM using the Arduino.  Supported chips
 * include 2716, 2732, 2764, 27C040, and 27C2001.
 *
 * Intelligent programming algorithms, like TurboProgram, are supported.
 * These allow a variable number of shorter program pulses until the byte
 * verifies.  This is optionally followed by an overprogram pulse that is
 * a multiple of the number of program pulses that were written.
 *
 * See the constructor definition for an explanation of the parameters that
 * control programming.
 */
class PromDevice27 : public PromDevice
{
  public:
    PromDevice27(uint32_t size, unsigned long mPulseWidthUsec, unsigned writeAttempts, unsigned overwriteMultiplier);
    void begin();
    const char * getName() { return "27 series EPROM"; }

  protected:
    void setAddress(uint32_t address);
    byte readByte(uint32_t address);
    bool burnByte(byte value, uint32_t address);

    void myDelay(unsigned int us);

    unsigned long mPulseWidthUsec;
    unsigned mWriteAttempts;
    unsigned mOverwriteMultiplier;
};

#endif  // #define INCLUDE_PROM_DEVICE_27_H

