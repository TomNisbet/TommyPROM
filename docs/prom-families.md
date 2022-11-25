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

EEPROMs are the easiest PROMs to use.  They usually can be erased and reprogrammed
electrically at the individual byte level.  This makes them appear similar to a slower
static RAM.  All of the interactive features of TommyPROM work well with EEPROMs.  Due
to their complexity, EEPROMs typically come in smaller sizes than other technologies.
The largest EEPROM in the 28C family is 32K bytes.

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

TommyPROM has a driver for Atmel SST39FS flash chips.  This driver replaces the 28C driver at compile time.  See configure.h to enable a different driver.

The SST39FS chips use fixed 2KB sectors that must be manually erased before a new program
operation, but the code manages this transparently.  Whenever a write is started to a new
segment, the driver first initiates an erase of that sector. A second write to the same
sector will not cause an erase, so is is possible to write to a segment multiple times
with no additional steps as long as the writes are to different parts of the sector.  For
example, 256 bytes could be written to the start of a sector from one file and then 512
bytes could be written to the end of the sector from another file.

Writing data that spans multiple sectors also works with no additional steps.  When
a sector boundary is crossed, the new sector is erased and set as the current sector.

The SST39FS driver supports a manual erase from the command line using the E command.
This is only needed if data will be rewritten to the same location after a previous write
to that sector.

## Misc Flash

**29C Series**

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

**28F Series**

The 28F series flash do not have sectors. The entire chip must be erased before writing
new data.  Data is written a byte at a time using a command register to control the
programming.  **These chips require a 12V programming voltage that must be present during
the programming or erase operations**.  TommyPROM does not currently support 28F chips,
but it would not be difficult to write a driver.  The default 28C driver will read 28F
chips.

**29F Series**

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
