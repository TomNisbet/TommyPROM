#ifndef INCLUDE_PROM_ADDRESS_DRIVER_H
#define INCLUDE_PROM_ADDRESS_DRIVER_H

#include "Arduino.h"
#include "Configure.h"

class PromAddressDriver {
  public:
    static void begin();
    static void setAddress(uint32_t address);
  private:
    static void setUpperAddress(byte addr);
#ifndef IS_MEGA
    static void setAddressRegister(uint8_t clkPin, byte addr);
#else // #ifndef IS_MEGA
    static void setLowerAddress(byte addr, bool isHigh);
#endif // #ifndef IS_MEGA
};


#endif // #define INCLUDE_PROM_ADDRESS_DRIVER_H
