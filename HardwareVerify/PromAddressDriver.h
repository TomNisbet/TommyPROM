#ifndef INCLUDE_PROM_ADDRESS_DRIVER_H
#define INCLUDE_PROM_ADDRESS_DRIVER_H

#include "Arduino.h"

class PromAddressDriver {
  public:
    static void begin();
    static void setAddress(word address);

  private:
    static void setAddressRegister(uint8_t clkPin, byte addr);
};


#endif // #define INCLUDE_PROM_ADDRESS_DRIVER_H
