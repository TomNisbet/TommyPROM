---
title: Printed Circuit Board
description: "TommyPROM Arduino 28C256 programmer PCB"
has_children: false
nav_order: 35
---

# Printed Circuit Board

Although it was originally designed as a one-off programmer to be used on a breadboard,
there are now some PCB versions.

## 28C256 PCB

A PCB version for the 28C256 and 28C64 chips was built to support a SAP-1 TTL breadboard
computer.  This project required a lot of burns for its  microcode and the PCB version
with the ZIF socket was much quicker than prying the chips out of a breadboard without
disturbing any of the wires.

The [KiCad design files](https://github.com/TomNisbet/TommyPROM/tree/master/schematics)
are in the project repo.

[![TommyPROM PCB](images/TommyPROM-pcb-with-microcode-500.jpg)](images/TommyPROM-pcb.jpg)

## TommyPROM-32

A new PCB is in progress for 32-pin EEPROM and Flash chips.  This design uses a set of
headers that allow any of the signals to the target chip to be re-routed.  The default
configuration, with just jumper shunts installed, is wired for the SST39SF0x0 chips.

Many other chips, particularly if they follow JEDEC standards, can be supported by
removing shunts and adding just a few jumper wires.

Chips with fewer pins can also be supported.  There are power pins in place to support
28-pin and 24-pin chips with a shunt to the correct VCC pin.

Power inputs and a switch are provided to support older flash and EEPROM chips that need
higher programming voltages.  Many of these chips just need a higher voltage, like 12V,
applied to the VPP pin continuously during the programing and verification cycle.  In
normal operation, the 5V VCC signal is also applied to the VPP pin.  The board has VPP
inputs for two voltages.  Applying 12V to _VPP In High_ and 5V to _VPP In Low_ lets
the VPP output be switched between these two voltages using a manual slide switch for
programming.

For chips that require a high voltage VPP pulse during programming, some external
switching circuitry will be needed to allow the Arduino to control the VPP voltage.  There
is a header connected to the unused D13 pin that can support this.  See the
[Intel 8755 version of the hardware](../hardware/#intel-8755a-hardware-version) for an
example of voltage switching by TommyPROM.
