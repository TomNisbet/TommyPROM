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

enum E27C_PGM {
    E27C_PGM_WE,  // Dedicated WE or PGM pin that is active LOW
    E27C_PGM_CE,  // CE is pulsed low to program, CE HIGH for verify
    E27C_PGM_D13  // Program voltage pulse switched using Arduino D13 pin
};

class PromDevice27 : public PromDevice
{
  public:
    PromDevice27(uint32_t size, E27C_PGM pgmType, unsigned long pulseWidthUsec,
                 unsigned writeAttempts, unsigned overwriteMultiplier, bool verify=true);
    void begin();
    const char * getName() { return "27 series EPROM"; }
    ERET erase(uint32_t start, uint32_t end);

  protected:
    void setAddress(uint32_t address);
    byte readByte(uint32_t address);
    bool burnByte(byte value, uint32_t address);

    bool burnByteWE(byte value, uint32_t address);
    bool burnByteCE(byte value, uint32_t address);
    void myDelay(unsigned int us);

    E27C_PGM mPgmType;
    unsigned long mPulseWidthUsec;
    unsigned mWriteAttempts;
    unsigned mOverwriteMultiplier;
    bool     mVerifyByte;
};

#endif  // #define INCLUDE_PROM_DEVICE_27_H
