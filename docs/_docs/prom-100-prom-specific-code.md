---
title: Chips Supported
permalink: /docs/prom-specific-code
exerpt: "PROM types supported by TommyPROM - EPROM, EEPROM, Flash"
---

TommyPROM can be used to read and write many different types of EEPROM, Flash, and EPROM
chips.  Due to differences in the technologies of these chips, some features of TommyPROM
may work differently or may not be applicable at all.  The standard code can read most
types of PROM, even if there is no specific software support for them.

Broadly speaking, there are two different ways to write to chips.  The first method is
similar to a static RAM - the WE signal is asserted and data is written to a location
using the data and address lines.  This is the case for the 28C64. Some of these chips,
like the 27C257, need a programming voltage to be present on the Vpp or PGM pin.  There
are usually timing requirements for the program pulse and there may be repeated write
cycles needed with a verification cycle to complete the write.

The second method is the use of a program command register.  Bytes are written to a single
location by sending a command string, which is a series of chip writes.  These chips
usually have additional commands to erase sectors or to lock and unlock the chip.  The
SST39SF010 is an example of a chip that uses a command register.  

Some chips use a combination of both methods.  The 28C256 uses a command register to
enable and disable Software Data Protection, but uses a direct write for programming.

TommyPROM has a variety of PromDevice Modules that can be compiled in to support these
different chip technologies.  Only one PromDevice module can be compiled into a build.


## Suported Chips

|Module  |Manufacturer |Type   |Notes|
|:---    |:---         |:---   |:--- |
|28C     |Atmel, others|EEPROM |28C256, 28C64, 28C16, 29C040 (Flash)|
|SST     |Microchip    |Flash  |SST39SF040 (all SST39SF0x0), SST28SF040|
|27      |many         |EPROM  |variety of 27xx and 27Cxx EPROM, Flash, and EEPROMs|
|2316    |many         |ROM    |read-only 2316, 2332, 2364, 23128, 23256|
|8755A   |Intel        |EPROM  |8755A EPROM and read-only 8355|

## PromDevice Modules

This PromDevice28C module is enabled by default to program 28C256 EEPROMS. To use a
different module, make the following edits:

* In configure.h, comment out the #define PROM_IS_28C line
* Uncomment one of the other PROM_IS_ lines to compile a different module
* In TommyPROM.ino, uncomment or add a new PromDevice declaration for the specific chip

## Chip Manufacturers

|Name     |Code |Notes|
|:---                         |:---|:---|
|AMS - Advanced Memory Systems|AMS |Merged with Intersil|
|AMD - Advanced Micro Devices |AM  ||
|Atmel                        |AT  |Aquired by Microchip|
|CSI - Catalyst Semiconductor |CAT |Aquired by ON Semiconductor|
|Fujitsu                      |MBM ||
|Intel                        |i   ||
|Mitsubishi                   |M5L ||
|Motorola                     |MCM ||
|Seeq Technology              |    |Aquired by LSI Logic|
|SST - Silicon Storage Tech   |ST  |Aquired by Microchip|
|ST Microelectronics          |M   ||
|Texas Instruments            |TMS ||
|Winbond                      |W   ||
|Xicor                        |X   ||
