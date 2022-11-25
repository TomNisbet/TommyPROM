---
title: PCB
description: "TommyPROM Arduino 28C256 programmer PCB"
has_children: false
nav_order: 35
---

# Printed Circuit Board

Although it was originally designed as a one-off programmer to be used on a breadboard,
there are now some PCB versions.

## 28C256 PCB

I built a PCB version for the 28C256 and 28C64 chips because so many burns of these were
needed for a SAP-1 TTL breadboard computer.  The PCB version with the ZIF socket was much
easier to work with, compared to prying the chips out of a breadboard without disturbing
any of the wires.

The [KiCad design files](https://github.com/TomNisbet/TommyPROM/tree/master/schematics)
are in the project repo.

[![TommyPROM PCB](images/TommyPROM-pcb-with-microcode-500.jpg)](images/TommyPROM-pcb.jpg)

## TommyPROM-32

A new PCB is in progress for 32-pin EEPROM and Flash chips.  This design uses a set of
headers that allow any of the signals to the target chip to be re-routed.  The default
configuration, with just jumper shunts installed, is wired for the SST39SF0x0 chips.

Many other chips, particularly if they follow JEDEC standards, can be supported be adding
just a few jumpers.

Chips with few pins can also be supported.  There are power pins in place to support
28-pin and 24-pin chips with a shunt to the correct VCC pin.
