# TommyPROM - An Arduino-based EEPROM programmer

This is a simple EEPROM programmer and reader that can be assembled using an Arduino and a
few additional parts.  The original code was specific to the 28C256 32Kx8 EEPROM, but it
has been extended to also support SST39SF040 Flash, many other EEPROMs and flash, and even
Intel 8755A EPROMS.

See the [documentation pages](https://tomnisbet.github.io/TommyPROM/) for the full
documentation with software, schematics, and troubleshooting tips.

Features include:
* Simple hardware design that can be assembled on a breadboard.
* ROM images transfers using XMODEM - no special host client needed.
* Support for fast block EEPROM writes - a 32K EEPROM will program in just a few seconds.
* Optimized code that supports the timing requirements needed to unlock the 28C series
[Software Protection Algorithm](docs/28C256-notes).
* Modular software design to easily support other EEPROM and EPROM families.

![TommyPROM Nano Hardware](docs/_docs/images/TommyPROM-nano.jpg)
