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
different chip technologies.


# Verified Chips

|Model     |Manufacturer |Type   |Module |Notes|
|:---      |:---         |:---   |:---   |:--- |
|AT28C256  |Atmel, others|EEPROM |28C    |Fully supported|
|AT28C64   |Atmel, others|EEPROM |28C    |Fully supported|
|AT29C010  |Atmel        |Flash  |28C    |Only with 128 byte or less sector size|
|SST39SF040|Microchip    |Flash  |SST39SF|All SST39SF0x0 supported|
|SST28SF040|SST          |Flash  |SST28SF|All SST28SF0x0 supported|
|M27C256   |ST Micro     |EPROM  |27     |VCC=6.5V, VPP=12.75V to pgm|
|W27C257   |Winbond      |EEPROM |27     |Continuous 12V or 14V for program/erase|
|SST27SF020|SST          |Flash  |27     |12V continuous for pgm/erase|
|8755A     |Intel        |EPROM  |8755A  |Requires 25V pulses to program|
|2316      |Commodore    |ROM    |23     |Read-only|


# PromDevice Modules

This PromDevice28C module is enabled by default to program 28C256 EEPROMS. To use a
different module, make the following edits:
* In configure.h, comment out the #define PROM_IS_28C line
* Uncomment one of the other PROM_IS_ lines to compile a different module
* In TommyPROM.ino, uncomment or add a new PromDevice declaration for the specific chip

## PromDevice28C

The 28C series EEPROMs, specifically the 28C256, are the original chips that TommyPROM
supported.  As a result, these have the best support, with all of the interactive features
of the command line.  In addition the basic read and write operations, there are also
commands to enable and disable the [Software Data protection (SDP)](28C256-notes)
features of 28C chips.  There is also support for the fast block write mode of these
chips, allowing a 32KB chip to be programmed in just a few seconds.

## PromDeviceSST39SF

SST39SF0x0 NOR Flash chips are 5V-only chips that use a command register for program and
erase operations.  They use fixed 4K-byte sectors that must be manually erased before a
new program operation, but the code manages this transparently.  Whenever a write is
started to a new segment, the driver first initiates an erase of that sector. A second
write to the same sector will not cause an erase, so it is possible to write to a segment
multiple times with no additional steps as long as the writes are to different parts of
the sector.  For example, 256 bytes could be written to the start of a sector from one
file and then 512 bytes could be written to the end of the sector from another file.

Writing data that spans multiple sectors also works with no additional steps.  When a
sector boundary is crossed, the new sector is erased and set as the current sector.

The SST39SF driver supports a manual erase from the command line using the E command. This
is only needed if data will be rewritten to the same location after a previous write to
that sector.

All programming and erase operations for the 39SF chips require only a single 5V power
supply.

## PromDeviceSST28SF

The SST28SF0x0 SuperFlash chips are an earlier version of the 39SF chips, using 256-byte
sectors.  The 28SF and 39SF chips are pin compatible, but use different command sets for
programming and erasing.  For read-only applications, they should be identical, although
the 28SF are slower.

All programming and erase operations for the 28SF chips require only a single 5V power supply.

## PromDevice27

The 27C supports Flash, EEPROM, and EPROM chips that use a variety of programming algorithms.  

Most, if not all, 27C series chips are not 5V-only and require a higher voltage to program
or erase. This driver supports chips that need the voltage constantly applied while
programming or erasing.  The programming voltages are provided by an external power supply
and manually switched before starting a write or erase operation.  See the [TommyPROM32
hardware](pcb#TommyPROM32) for a diode circuit that allows the programming voltage to be
switched between Vcc and a higher voltage.  

Check the [Chip Details](#27c-and-27e-series-eproms-and-flash) for the high voltage
connections for a particular chip or consult your data sheet. Also note the warning in
that section about issuing other commands while the programming voltage is asserted.

Chips that use high voltage pulses for each byte are not supported.  For those chips, some
elements of the [8755A hardware](8755A-hardware) may be leveraged to build a version of
the programmer that is able to provide high voltage pulses.

## PromDevice23

The PromDevice23 driver is used to read Commodore 2316, 2332, and 2364 mask-programmed ROMs.
This were used in early computers like the Commodore PET, Atari, and others.

Most ROM chips can be read using the standard 28C driver.  The 23 series ROMs are unique because
the chip select polarity is configurable when the chip is initally programmed.  This means that, for example, 
some chips will use CS1 as an active HIGH signal and other will use it active LOW.  This existing
Unlock command was repurposed in this driver to scan the chip to determine the polarity of the
Chip Select pin(s).

For the 2316 make the following connections:

* CS3 to Arduino A0 (WE)
* CS2 to Arduino A1 (CE)
* CS1 to Arduino A2 (OE)

The 2332 does not have a CS3, and the 2364 does not have CS3 or CS2, so those Arduino pins are not used.

Before reading the chip, run the Unlock command to scan the Chip Selects.  This will try all combinations
of CS values to read data from the chip.  After the Unlock command runs successfully, the Dump or Read commands
can then be used to read from the chip.

## PromDevice8755

TommyPROM has a driver for Intel 8755 EPROMs.  This driver replaces the 28C driver at
compile time.  See configure.h to enable a different driver.  This driver will also read
Intel 8355 OTP EPROMs.  Also note that the TommyPROM hardware for the 8755 is drastically
different from the other versions.

The Intel 8755 is a peripheral chip designed for 8085 systems.  It is a 40 pin device that
contains a 2KB EPROM plus two general-purpose I/O ports.  The 8355 is a one-time
programmable version of the 8755.  The 8755 requires a 25V programming pulse for each byte
to be written.  

A new hardware build was created to support the 8755 chips.  Because the 8755 has a
multiplexed data and address bus, the usual shift registers are not used for addressing.
The chip only needs 8 connects that are shared for address and data, plus three dedicated
address lines.  The Arduino has enough pins to drive all of these directly, without the
need for shift registers to create address lines.

The 8755 build of TommyPROM also has a circuit to control the 25V programming pulses.


# Chip Details

## 28C Series 28C256

Tested with chips from several manufacturers including Atmel AT28C256, Xicor X28C256, and
CSI CAT28C256.  Also works with the smaller 28C64 and 28C16 chips, although some
parameters may need to be adjusted. Some smaller chips do not support SDP, so the _Lock_
and _Unlock_ commands have no effect. Check the datasheet to see if block writes are
supported.  Some 28C16s use a 16 byte block instead of the 64 bytes used on the 28C256.
It is always safe to just set the block size parameter to zero when declaring the chip to
disable block writes altogether.

The 28C chips do not need to be explicitly erased.  Each memory cell is automatically
erased during a write operation, so they are more similar to a slow RAM than to other
PROMs.

## SST39SF040

The SST39SF driver supports all chips in this family, so the 040, 020, and 010 chips can
be all be used with no code change.  These chips use sectors that must be erased before
writing new data.  The code keeps track of the current sector and will automatically do
an erase operation whenever a write starts to a new sector.  The _Erase_ command is
supported, but is not needed unless overwriting new data to a single sector.

## SST28SF040

This is an earlier version of the SST39SF series chips.  They are pin compatible with the
39SF series, but use a different command set for programming.  Unlike the 39SF, these
flash chips support software data protection.  The _Lock_ and _Unlock_ commands can be
used to enable and disable SDP from the command line.

## 27C and 27E Series EPROMs and Flash

*WARNING*: Some chips in this series use the _CE_ and _OE_ pins in non-standard ways when
 the high voltage is asserted on _VPP_.  Because the programming voltage is switched
 manually, care needs to be taken when using chips that pulse the _CE_ pin for
 programming. The programming code itself knows how to manage this, but other commands,
 like the DUMP command, will also toggle _CE_.  If the programming voltage is asserted,
 these other commands can inadvertently cause a write operation.  Be sure to assert the
 programming voltage, issue the needed write commands, and then remove the high voltage
 before issuing and additional commands that may corrupt the data.

### M27C256

The M27C256 is a 32Kx8 byte UV-erasable EPROM.  It needs two special voltages for
programming.  _VPP_ must be raised to 12.75V and _VCC_ must be raised to 6.25V.  Be sure
to isolate the chip's _VCC_ pin from other Vcc connections so that the 6.25V is not fed
back into the  Adbuino or into the shift registers.

This chip does not have a dedicated _PGM_ or _WE_ pin, so programming and verify are
controlled by the _E_ (Chip Enable) and _G_ (Output Enable) pins.  The _CE_ pin is pulsed
to program bytes into a location.  To verify, the _OE_ pin is asserted, but the _CE_ pin
is not.

Note the warning in the section above about data corruption from issuing other commands
while the programming voltages are present.

This chip can only be erased with UV light, so the erase command is not supported.

### W27C257 and W27C512

The Winbond W27C257 and W27E257 appear to be identical 32Kx8 EEPROMs.  The 27C version
has been tested.  The Winbond W27C512 is a 64Kx8 EEPROM with no dedicated _VPP_ pin.

The 257 EEPROMs have a _VPP_ pin that needs a constant 12V during programming. Unlike the
newer 28C EEPROMs, these chips do not automatically erase before writing to a location.
Instead, the entire chip is erased by applying 14V to _VPP_ and _A9_ and then pulsing
_CE_.  To erase the chip, assert the voltages on _VPP_ and _A9_ and then issue the
_Erase_ command from the terminal.

Unlike the 257 chips, the W27C512 does not have a dedicated pin for the programming
voltage and instead uses the _OE_ pin to place the chip in programming mode.  The verify
operation requires that the _OE_ pin be switched to _LOW_ and there is no hardware support
for this, so the current code supports the 512 chip by doing a single write cycle with no
verify.

Because the chips use a constant high voltage for programming instead of a pulse, an
external power supply and two diodes can be used to supply either 5V or 12V to the
pins for programming and erasing.

Note that the W27x257 chip are almost a drop-in replacement for the 28C256.  The _WE_,
_VPP_, and _A14_ pins are the only differences.  For reading, the _VPP_ pin should be
connected to 5V.

## 29C Series

The 29C0x0 chips are 5V sectored flash chips.  They  are similar to the 28C EEPROMs,
except they must erase an entire sector before programming new data.  The start of a block
write operation causes the target sector to be automatically erased before the block is
written.  If the entire sector is not written in one operation, there is no way to write
additional data to the sector because a new block write will erase the sector again.

The write algorithm for the 29C chips is similar to 28C, so it is possible to use that
code to write these chips as well with some restrictions.  

All 29C010 and 29C020 chips that have been encountered use a sector size of either 64
bytes or 128 bytes.  A single XModem packet holds 128 bytes of data, so these chips will
program without issue.

The 28C040 chips from some manufacturers use a 256 byte sector.  *This will not currently
work with TommyPROM*, because each of the 128 byte XModem packets received will do its own
block write.  The first write will erase 256 bytes and then write data into the first half
of the sector.  The second write will then erase the data just written and then write 128
bytes into the second half of the sector.  To fix this, the code would need to be modified
to keep 256 bytes of data from two packets before initiating a block write.

## 28F Series

The 28F series flash do not have sectors. The entire chip must be erased before writing
new data.  Data is written a byte at a time using a command register to control the
programming.  **These chips require a 12V programming voltage that must be present during
the programming or erase operations**.  TommyPROM does not currently support 28F chips,
but it would not be difficult to write a driver.  The default 28C driver will read 28F
chips.

## 29F Series

The 29F series flash have sectors that must be explicitly erased before writing new
data.  Data is written a byte at a time using a command register to control the
programming.  TommyPROM does not currently support 29F chips, but it would not be
difficult to write a driver.  The default 28C driver will read 29F chips.

## SST27SF020

The Silicon Storage SST27SF0x0 are programmed similarly to the 27C257 in that a constant
voltage is applied for program and erase operations.  Unlike the 27C257, these have a
dedicated WE pin that controls programming and erasing.  Another difference is that they
do not have a verify operation to read back the programmed data.

These Flash chips have a _VPP_ pin that needs a constant 12V during programming. Unlike
the newer 28C EEPROMs, these chips do not automatically erase before writing to a
location. Instead, the entire chip is erased by applying 12V to _VPP_ and _A9_ and then
pulsing _WE_.


# Chips to be Tested

|Model     |Manufacturer |Type   |Module |Notes|
|:---      |:---         |:---   |:---   |:--- |
|M27C2001  |ST Micro     |EPROM  |       |VCC=6.5V, VPP=12.75V to pgm|
|M27C4001  |ST Micro     |EPROM  |       |VCC=6.5V, VPP=12.75V to pgm|
|W27C512   |Winbond      |EEPROM |27     |Continual 12V or 14V for program/erase,VPP on OE|
|AM28F512  |AMD          |EEPROM |       ||
|AM29F040  |AMD          |EEPROM |       ||
|AMS29AF010|             |       |       ||
|2764      |SEEQ         |EPROM  |       ||
|W29C040   |Winbond      |       |       ||



# Chip Manufacturers

|Name     |Code |Notes|
|:---                         |:---|:---|
|AMS - Advanced Memory Systems|AMS |Merged with Intersil|
|AMD - Advanced Micro Devices |AM  ||
|Atmel                        |AT  |Aquired by Microchip|
|CSI - Catalyst Semiconductor |CAT |Aquired by ON Semiconductor|
|Intel                        |i   ||
|Seeq Technology              |    |Aquired by LSI Logic|
|SST - Silicon Storage Tech   |ST  |Aquired by Microchip|
|ST Microelectronics          |M   ||
|Winbond                      |W   ||
|Xicor                        |X   ||
