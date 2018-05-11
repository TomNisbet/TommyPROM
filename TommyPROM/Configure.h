// Uncomment only one of these to use the fast I/O code for the data bus, or
// comment them all out to use the slower bit-at-a-time code.

//#define ARDUINO_IS_MICRO
//#define ARDUINO_IS_UNO
#define ARDUINO_IS_NANO

// Comment this out to remove extra debugging commands and code
#define ENABLE_DEBUG_COMMANDS

// Uncomment only one of these to choose the PROM device code that will be
// compiled in.

#define PROM_IS_28C
//#define PROM_IS_8755


#if defined(PROM_IS_28C)
#include "PromDevice28C.h"
#elif defined(PROM_IS_8755)
#include "PromDevice8755.h"
#else
#error "No Prom Device defined"
#endif
