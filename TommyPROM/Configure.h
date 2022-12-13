// Comment this out to remove extra debugging commands and code

#define ENABLE_DEBUG_COMMANDS


// Uncomment only one of these to choose the PROM device code that will be
// compiled in.

#define PROM_IS_28C
//#define PROM_IS_27
//#define PROM_IS_SST39SF
//#define PROM_IS_SST28SF
//#define PROM_IS_8755A

// Don't change anything below this comment unless you are adding support for a new device type.
#if defined(PROM_IS_28C)
#include "PromDevice28C.h"
#elif defined(PROM_IS_27)
#include "PromDevice27.h"
#elif defined(PROM_IS_SST39SF)
#include "PromDeviceSST39SF.h"
#elif defined(PROM_IS_SST28SF)
#include "PromDeviceSST28SF.h"
#elif defined(PROM_IS_8755A)
#include "PromDevice8755A.h"
// Additional device support goes here...
// Also add matching code in TommyPROM.ino to declare the new device
#else
#error "No Prom Device defined"
#endif
