---
title: PROM Types
description: "PROM types supported by TommyPROM - EPROM, EEPROM, Flash"
has_children: false
nav_order: 40
---

TommyPROM can be used to read and write several different types of PROM chips.  Due to
differences in the technologies of these chips, some features of TommyPROM may work
differently or may not be applicable at all.  The standard code can read most types of
PROM, even if there is no specific software support for them.


# PROM Types

## EPROM - Erasable Programmable Read-only Memory

EPROMs can be written once, but must then be erased by exposing them to UV light for
several minutes.  They are usually easily read by TommyPROM, but there is limited
support for programming.

## EEPROM - Electrically Erasable Programmable Read-only Memory

EEPROMs are the easiest PROMs to use.  Modern EEPROMs usually can be erased and reprogrammed
electrically at the individual byte level.  This makes them appear similar to a slower
static RAM.  All of the interactive features of TommyPROM work well with EEPROMs.  Due
to their complexity, EEPROMs typically come in smaller sizes than other technologies.
The largest EEPROM in the 28C family is 32K bytes.  Some older EEPROMs cannot be reprogrammed at the individual byte level and are instead bulk erased before a new write
operation.  Programming and erasing for these chips usually requires voltages higher than 5V.

## Flash ROM

Flash is similar to EEPROM, in that it can be electrically erased and reprogrammed. Unlike
EEPROMs, Flash cannot erase single bytes individually.  Instead, the chip is arranged into
memory blocks called sectors.  The programmer can erase individual sectors or, sometimes,
the entire chip.  Some chip families require an explicit erase command and others initiate
an erase at the start of a programming operation.

Most of the features of TommyPROM are supported for Flash, but some operations that would
do multiple writes to the same sector work differently.

## OTP ROM - One-time Programmable ROM

One-time Programmable (OTP) ROM is written once, either at the factory or in the field.
These chips cannot be erased and re-written, but they can be read by TommyPROM.  Interesting
trivia - some of the field-programmable OTPs are just EPROMs packaged without the erase
window.

# Supported Chip Families

## 28C EEPROM

The 28C series EEPROMs, specifically the 28C256, are the original chips that TommyPROM
supported.  As a result, these have the best support, with all of the interactive features
of the command line.  In addition the basic read and write operations, there are also
commands to enable and disable the [Software Data protection (SDP)](../28C256-notes)
features of 28C chips.  There is also support for the fast block write mode of these
chips, allowing a 32KB chip to be programmed in just a few seconds.

## 27C EPROM

The 27C EPROMs use a variety of programming algorithms.  Code exists for some of these,
but very few of the chips have been tested.

Most, if not all, 27C series chips are not 5V only and require a higher voltage to program
them.  Some need the voltage constantly applied while programming and others use high
voltage pulses for each byte.  For constant voltage chips, it is probably easiest to just
add an external power supply and manually assert the voltage before starting a write.  For
those with switched voltages, some elements of the 8755 hardware may be leveraged to build
a version of the programer that supports these chips.

## Atmel SST39FS Flash

TommyPROM has a driver for Atmel SST39FS NOR flash chips.  This driver replaces the 28C
driver at compile time.  See configure.h to enable a different driver.

The SST39FS chips use fixed 4KB sectors that must be manually erased before a new program
operation, but the code manages this transparently.  Whenever a write is started to a new
segment, the driver first initiates an erase of that sector. A second write to the same
sector will not cause an erase, so it is possible to write to a segment multiple times
with no additional steps as long as the writes are to different parts of the sector.  For
example, 256 bytes could be written to the start of a sector from one file and then 512
bytes could be written to the end of the sector from another file.

Writing data that spans multiple sectors also works with no additional steps.  When
a sector boundary is crossed, the new sector is erased and set as the current sector.

The SST39FS driver supports a manual erase from the command line using the E command.
This is only needed if data will be rewritten to the same location after a previous write
to that sector.

There is also a driver for the SST28SF0x0 SuperFlash chips.  These are an earlier version
of the 39SF chips, using 256-byte sectors.  The 28SF and 39SF chips are pin compatible,
but use different command sets for programming and erasing.  For read-only applications,
they should be identical, although the 28SF are slower.

All programming and erase operations for both the 39SF and 28SF chips require only a
single 5V power supply.

## Misc Flash

#### 29C Series

The 29C series flash chips are similar to the 28C EEPROMs, except they must erase an
entire sector before programming new data.  The start of a block write operation causes
the target sector to be erased before the block is written.  If the entire sector is not
written in one operation, there is no way to write additional data to the sector because
a new block write will erase the sector again.

The write algorithm for the 29C chips is similar to 28C, so it is possible to use that
code to write these chips as well with some restrictions.  All 29C010 and 29C020 chips
that have been encountered use a sector size of either 64 bytes or 128 bytes.  A single
XModem packet holds 128 bytes of data, so these chips will program without issue.

The 28C040 chips from some manufacturers use a 256 byte sector.  This will not currently
work with TommyPROM, because each of the 128 byte XModem packets received will do its own
block write.  The first write will erase 256 bytes and then write data into the first half
of the sector.  The second write will then erase the data just written and then write 128
bytes into the second half of the sector.  To fix this, the code would need to be modified
to keep 256 bytes of data from two packets before initiating a block write.

#### 28F Series

The 28F series flash do not have sectors. The entire chip must be erased before writing
new data.  Data is written a byte at a time using a command register to control the
programming.  **These chips require a 12V programming voltage that must be present during
the programming or erase operations**.  TommyPROM does not currently support 28F chips,
but it would not be difficult to write a driver.  The default 28C driver will read 28F
chips.

#### 29F Series

The 29F series flash have sectors that must be explicitly erased before writing new
data.  Data is written a byte at a time using a command register to control the
programming.  TommyPROM does not currently support 29F chips, but it would not be
difficult to write a driver.  The default 28C driver will read 29F chips.


## Intel 8755/8355 EPROM

TommyPROM has a driver for Intel 8755 EPROMs.  This driver replaces the 28C driver at
compile time.  See configure.h to enable a different driver.  This driver will also read
Intel 8355 OTP EPROMs.  Also note that the TommyPROM hardware for the 8755 is drastically
different from the other versions.

The Intel 8755 is a peripheral chip designed for 8085 systems.  It is a 40 pin device that
contains a 2KB EPROM plus two general-purpose I/O ports.  The 8355 is a one-time
programmable version of the 8755.  The 8755 requires a 25V programming pulse for each byte
to be written.  

A new hardware build was created to support the 8755 chips.  Because the 8755 has a multiplexed data and address bus, the usual shift registers are not used for addressing.
The chip only needs 8 connects that are shared for address and data, plus three dedicated
address lines.  The Arduino has enough pins to drive all of these directly, without the
need for shift registers to create address lines.

The 8755 build of TommyPROM also has a circuit to control the 25V programming pulses.

# Verified Chips

|Model     |Manufacturer |Type   |Module |Notes|
|:---      |:---         |:---   |:---   |:--- |
|AT28C256  |Atmel, others|EEPROM |28C    |Fully supported|
|SST39SF040|Microchip    |Flash  |SST39SF|All SST39SF0x0 supported|
|SST28SF040|SST          |Flash  |SST39SF|All SST28SF0x0 supported|
|SST27SF020|SST          |Flash  |27     |12V continuous for pgm/erase|
|W27C257   |Winbond      |EEPROM |27     |Continual 12V or 14V for program/erase|
|AT29C010  |Atmel        |Flash  |28C    |Only with 128 byte or less sector size|
|8755A     |Intel        |EPROM  |8755A  |Requires 25V pulses to program|

#### 28C256

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

#### SST39SF040

The SST39SF driver supports all chips in this family, so the 040, 020, and 010 chips can
be all be used with no code change.  These chips use sectors that must be erased before
writing new data.  The code keeps track of the current sector and will automatically do
an erase operation whenever a write starts to a new sector.  The _Erase_ command is
supported, but is not needed unless overwriting new data to a single sector.

#### SST28SF040

This is an earlier version of the SST39SF series chips.  They are pin compatible with the
39SF series, but use a different command set for programming.  Unlike the 39SF, these
flash chips support software data protection.  The _Lock_ and _Unlock_ commands can be
used to enable and disable SDP from the command line.

#### SST27SF020

The Silicon Storage SST27SF0x0 are programmed similarly to the 27C257 in that a constant
voltage is applied for program and erase operations.  Unlike the 27C257, these have a
dedicated WE pin that controls programming and erasing.  Another difference is that they
do not have a verify operation to read back the programmed data.

These Flash chips have a _VPP_ pin that needs a constant 12V during programming. Unlike
the newer 28C EEPROMs, these chips do not automatically erase before writing to a
location. Instead, the entire chip is erased by applying 12V to _VPP_ and _A9_ and then
pulsing _WE_.

#### W27C257 and W27C512

The Winbond W27C257 and W27E257 appear to be identical 32Kx8 EEPROMs.  The 27C version
has been tested.  The Winbond W27C512 is a 64Kx8 EEPROM with no dedicated _VPP_ pin.

The 257 EEPROMs have a _VPP_ pin that needs a constant 12V during programming. Unlike the
newer 28C EEPROMs, these chips do not automatically erase before writing to a location.
Instead, the entire chip is erased by applying 14V to _VPP_ and _A9_ and then pulsing
_CE_.

Unlike the 257 chips, the W27C512 does not have a dedicated pin for the programming
voltage and instead uses the OE pin to place the chip in programming mode.  The verify
operation requires that the OE pin be switched to _LOW_ and there is no hardware support
for this, so the current code supports the 512 chip by doing a single write cycle with no
verify.

Because the chips use a constant high voltage for programming instead of a pulse, an
external power supply and two diodes can be used to supply either 5V or 12V to the
pins for programming and erasing.

Note that the W27x257 chip are almost a drop-in replacement for the 28C256.  The _WE_,
_VPP_, and _A14_ pins are the only differences.  For reading, the _VPP_ pin should be
connected to 5V.

#### 29C010

The 29C0x0 chips are 5V sectored flash chips.  They automatically erase a sector at the
start of a new write, so they can be used with the default 28C code.

Note that some versions of the 29C040 use a 256 byte sector size.  This will not work
with the TommyPROM code because the XModem transfer buffer is only 128 bytes.  Code
changes would be needed to buffer up two packets of data into a single write operation
for chips with the 256 byte buffer.

# Chips to be Tested

|Model     |Manufacturer |Type   |Module |Notes|
|:---      |:---         |:---   |:---   |:--- |
|M27C4001  |ST Micro     |EEPROM |       |VCC=6.5V, VPP=12.75V to pgm|
|W27C512   |Winbond      |EEPROM |27     |Continual 12V or 14V for program/erase,VPP on OE|
