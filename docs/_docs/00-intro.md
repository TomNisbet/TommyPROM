---
title: "TommyPROM - An Arduino-based EEPROM programmer"
permalink: /
excerpt: "TommyPROM Arduino based programmer for 28C256 EEPROMs"
---

TommyPROM is a simple EEPROM programmer and reader that can be assembled using an Arduino and a
few additional parts.  It has been successfully built using the Arduino UNO, Nano and
Boarduino models.

The original code was specific to the 28C256 32Kx8 EEPROM, but it has been extended to
also support SST39SF flash, 27C EPROMs, 29C flash and the Intel 8755A EPROMS.

[![TommyPROM Nano Hardware](docs/images/TommyPROM-nano-700.jpg)](docs/images/TommyPROM-nano.jpg)

Features include:
* Simple hardware design that can be assembled on a breadboard.
* ROM images transfers using XMODEM - no special host client needed.
* Support for fast block EEPROM writes - a 32K EEPROM will program in just a few seconds.
* Optimized code that supports the timing requirements needed to unlock the 28C series [Software Data Protection](docs/28C256-notes) Algorithm.
* Modular software design to easily support other EEPROM, flash, and EPROM families.
* Hardware support for 19 address lines, allowing the use of chips as large as 512KB, like the SST39SF040 and 29C040.

## Design

The 28C hardware and software design can be used with other 5V PROMS as well.  Many 5V
chips, including UV EPROMs such as the 2716, 2764, 27C2001 and 27C040, can be read, but
not written, with the basic hardware. Some pin changes may be needed to get the signals to
the correct pins on the device.  See the [Extending the design](docs/extending) page for
details on suggested hardware and software changes needed to support new
[EPROM, flash,and EEPROM families](docs/prom-families).

The PROM-specific code is modular and can be easily adapted to support additional devices.
There are currently drivers for 28C series EEPROMS, SST39SF flash, 27C EPROMs and the
Intel 8755A EPROM.

Some 29C series chips, like the 29C010 can be written with the 28C hardware and driver.
The 29C series only differs from the 28C in that the 29C chips erase and write an entire
sector at a time.  The 29C010 and some 29C020 chips use a 128 byte sector, which matches
the XModem buffer in the current code.  Other 29C020s and all 29C040s use a 256 byte
sector and cannot be written without code changes to buffer up an entire 256 byte block of
data before writing.

The [hardware](docs/hardware) page has descriptions of the hardware designs and links to
schematics.  The [software](docs/software) page has class definitions and more information on
the software design.

The project was inspired by the
[MEEPROMMER programmer](https://github.com/mkeller0815/MEEPROMMER).

## PCB Version

Although it was originally designed as a one-off programmer to be used on a breadboard,
there are now some PCB versions as well.  The [PCB](docs/pcb) page has descriptions with links
to the KiCad schematics and Gerber files.

[![TommyPROM PCB](docs/images/TommyPROM-pcb-with-microcode-500.jpg)](docs/images/TommyPROM-pcb.jpg)

## Compiling

Open the TommyPROM.ino file in the Arduino IDE. It should automatically open the cpp and h
files as well. The default code programs 28C series chips using Arduino Nano hardware.  To
use this version, just compile and upload it to the Arduino.

**Note well** that this code has been optimized for the Aduino UNO and Nano hardware so
that it can run quickly enough to meet 28C series chip timing requirements for SDP
unlocking.  To use different Arduino hardware, like the Micro, the board-specific code in
PromDevice.cpp and PromAddressDriver.cpp must be changed to match the port mappings
between the ATmega chip and the Arduino I/O pins. If you are compiling for the
Arduino Mega2560, uncomment the IS_MEGA line in Configure.

Because the Arduino has limited code and RAM space available, only one PROM driver is
compiled in at a time.  To change drivers, uncomment the desired PROM_IS_xx line in
Configure.h and  comment out the other choices.  The code to instantiate the driver is
near the top in TommyPROM.ino.  The change to Configure.h will automatically cause a
driver to be installed, but it may be necessary to modify the parameters for a specific
chip type.

## Troubleshooting

* Verify that the Arduino type you are using is a supported board or that its I/O port definitions match one of the supported boards.  Some other Arduino boards, like the
Duemilanove, appear to be compatible but have not been tested. Others, like the Micro,
have different port mappings and definitely will not work without software changes.
* If the code doesn't appear to be working, concentrate on the read operations first to
verify that the data and address paths are good.
* 28C series EEPROMS, like the X28C256, sometimes ship from the factory with Data
Protection enabled.  Use the UNLOCK command to disable this. See the
[28C256 Notes](docs/28C256-notes) for more information.
* Re-check all hardware connections and verify the the control pins are going to the
Arduino pins that match the definitions in the code.
* This repo contains a standalone program called HardwareVerify that allows low-level
access to the address, data, and control lines through a menu-driven interface.  See the [troubleshooting](docs/troubleshooting) section for more tips.
