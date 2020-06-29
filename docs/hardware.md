---
title: Hardware Design
has_children: false
nav_order: 3
---

# Hardware Design

## Basic Hardware Version

The hardware uses an Arduino to write data and to toggle control lines with the
appropriate timing to access the PROM.  A pair of 74LS164 serial to parallel shift
registers latch the address lines.  Use of the shift registers allows the Arduino to
control up to 16 address lines using only 3 output ports.

The basic circuit is as follows:
* Pins D2..D9 are wired to the data lines on the target PROM.
* Pins A0..A2 are wired to the WE, CE, and OE control lines on the target PROM.
* Pins A3..A5 control shift registers to produce the address lines.
* Pins D10..D12 control A16..A18 for chips larger than 64K bytes.

Note that the existing design uses 74LS164 shift registers, but another 8-bit parallel out
shift register, like the 74LS594 or 74LS595, could be used instead with some pin changes.

The two shift registers can produce a sixteen bit address, although the 28C256 only needs
15 addresses. Chips larger than 64K are supported by using the shift registers for A0..A15
and connecting Arduino pins D10..D12 to the chip's A16..A18

![TommyPROM Nano Schematic](images/TommyPROM-nano-sch.png)

**NOTE:**
The schematic does not show the Vcc and ground pins for the 74LS164 shift registers.
These must be connected to +5 and ground, respectively.

## Intel 8755A Hardware Version

The Intel 8755A uses a multiplexed data and address bus, plus 3 additional address lines.
Most Arduino hardware has enough pins to support this directly, so no additional hardware
is needed for addressing. An Arduino pin is also used to drive the ALE latch pin.

The 8755A requires the Vdd pin to be be switched between 5V and 25V during the programming
of each byte. A simple transistor circuit is used for this. No voltage regulator circuit
is present here for the programming voltage. Because this is designed as a quickly
assembled hardware design, a triple output bench supply was used to provide the 25.5V,
5.5V and 5V outputs.

![TommyPROM Nano Schematic](images/TommyPROM-8755A-sch.png)

Eagle-format schematics can be downloaded from the
[hardware project page](https://github.com/TomNisbet/TommyPROM/tree/master/hardware).
