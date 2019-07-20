#ifndef INCLUDE_PROM_ADDRESS_DRIVER_H
#define INCLUDE_PROM_ADDRESS_DRIVER_H

#include "Arduino.h"

class PromAddressDriver {
  public:
    static void begin();
    static void setAddress(uint32_t address);

  private:
    static void setUpperAddress(byte addr);
    static void setAddressRegister(uint8_t clkPin, byte addr);
};


#endif // #define INCLUDE_PROM_ADDRESS_DRIVER_H
