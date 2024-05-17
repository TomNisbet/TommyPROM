---
title: PromDevice27
permalink: /docs/prom-device-27
exerpt: "TommyPROM support for 27 series EPROM - 2716, 2732, 2764, 27C128, etc"
---

The PromDevice27 driver supports Flash, EEPROM, and EPROM chips that use a variety of programming algorithms.  

Most, if not all, 27C series chips are not 5V-only and require a higher voltage to program or erase. This driver supports chips that need the voltage constantly applied while programming or erasing.  The programming voltages are provided by an external power supply and manually switched before starting a write or erase operation.  See the [TommyPROM32 hardware](pcb#TommyPROM32) for a diode circuit that allows the programming voltage to be switched between Vcc and a higher voltage.  

Check the [Chip Details](#27c-and-27e-series-eproms-and-flash) for the high voltage connections for a particular chip or consult your data sheet. Also note the warning in that section about issuing other commands while the programming voltage is asserted.

Chips that use high voltage pulses for each byte are not supported.  For those chips, some elements of the [8755A hardware](prom-device-8755) may be leveraged to build a version of the programmer that is able to provide high voltage pulses.

## 27C and 27E Series EPROMs and Flash

*WARNING*: Some chips in this series use the _CE_ and _OE_ pins in non-standard ways when the high voltage is asserted on _VPP_.  Because the programming voltage is switched manually, care needs to be taken when using chips that pulse the _CE_ pin for programming. The programming code itself knows how to manage this, but other commands, like the DUMP command, will also toggle _CE_.  If the programming voltage is asserted, these other commands can inadvertently cause a write operation.  Be sure to assert the programming voltage, issue the needed write commands, and then remove the high voltage before issuing any additional commands that may corrupt the data.

### M27C256

The M27C256 is a 32Kx8 byte UV-erasable EPROM.  It needs two special voltages for programming.  _VPP_ must be raised to 12.75V and _VCC_ must be raised to 6.25V.  Be sure to isolate the chip's _VCC_ pin from other Vcc connections so that the 6.25V is not fedback into the  Adbuino or into the shift registers.

This chip does not have a dedicated _PGM_ or _WE_ pin, so programming and verify are controlled by the _E_ (Chip Enable) and _G_ (Output Enable) pins.  The _CE_ pin is pulsed to program bytes into a location.  To verify, the _OE_ pin is asserted, but the _CE_ pin is not.

Note the warning in the section above about data corruption from issuing other commands while the programming voltages are present.

This chip can only be erased with UV light, so the erase command is not supported.

### M27256

This is an older version of the M27C256.  Pin connections are the same, but for programming Vcc=6V and Vpp=12.5V.  The programming pulse width is 1ms instead of 100us, and the programming algorithm uses an overwrite pulse equal to 3ms * the number of program pulses written.

### W27C257, W27E257 and W27C512

The Winbond W27C257 and W27E257 appear to be identical 32Kx8 EEPROMs.  The 27C version has been tested.  The Winbond W27C512 is a 64Kx8 EEPROM with no dedicated _VPP_ pin.

The 257 EEPROMs have a _VPP_ pin that needs a constant 12V during programming. Unlike the newer 28C EEPROMs, these chips do not automatically erase before writing to a location. Instead, the entire chip is erased by applying 14V to _VPP_ and _A9_ and then pulsing _CE_.  To erase the chip, assert the voltages on _VPP_ and _A9_ and then issue the _Erase_ command from the terminal.

Unlike the 257 chips, the W27C512 does not have a dedicated pin for the programming voltage and instead uses the 12V on _OE_ pin to place the chip in programming mode.  The verify operation requires that the _OE_ pin be switched to _LOW_ and there is no hardware support for this, so the current code supports the 512 chip by doing a single write cycle with no verify.

Because the chips use a constant high voltage for programming instead of a pulse, an external power supply and two diodes can be used to supply either 5V or 12V to the pins for programming and erasing.

Note that the W27x257 chip are almost a drop-in replacement for the 28C256.  The _WE_, _VPP_, and _A14_ pins are the only differences.  For reading, the _VPP_ pin should be connected to 5V.

## SST27SF020

The Silicon Storage SST27SF0x0 are programmed similarly to the 27C257 in that a constant
voltage is applied for program and erase operations.  Unlike the 27C257, these have a
dedicated WE pin that controls programming and erasing.  Another difference is that they
do not have a verify operation to read back the programmed data.

These Flash chips have a _VPP_ pin that needs a constant 12V during programming. Unlike
the newer 28C EEPROMs, these chips do not automatically erase before writing to a
location. Instead, the entire chip is erased by applying 12V to _VPP_ and _A9_ and then
pulsing _WE_.

## Verified Chips

|Model     |Manufacturer |Type   |Module |Notes|
|:---      |:---         |:---   |:---   |:--- |
|M27256    |ST Micro     |EPROM  |27     |VCC=6V, VPP=12.5V to pgm|
|M27C256   |ST Micro     |EPROM  |27     |VCC=6.5V, VPP=12.75V to pgm|
|W27C257   |Winbond      |EEPROM |27     |Continuous 12V or 14V for program/erase|
|SST27SF020|SST          |Flash  |27     |12V continuous for pgm/erase|

## Chips to be Tested

|Model     |Manufacturer |Type   |Module |Notes|
|:---      |:---         |:---   |:---   |:--- |
|TMS2564   |TI           |EPROM  |       |VPP=25V, 50ms low pulse on PGM|
|AM2716    |AMD          |EPROM  |       ||
|B2732     |Intel        |EPROM  |       |VPP/OE=25V, 50ms low pulse CE|
|MBM2732A  |Fujitsu      |EPROM  |       ||
|2764      |SEEQ         |EPROM  |       |VPP/OE=21V, 50ms low pulse CE|
|AM2764-2  |AMD          |EPROM  |       |VPP=21V, 50ms low pulse PGM|
|M5L2764K  |Mitsubishi   |EPROM  |       |VCC=6V, VPP=21V, 1ms low pulse PGM 4x overwrite|
|D2764     |Intel        |EPROM  |       |VCC=6V, VPP=12.5V, 1ms low pulse PGM 3x overwrite|
|D27128    |Intel        |EPROM  |       |VCC=6V, VPP=12.5V, 1ms low pulse PGM 3x overwrite|
|W27C512   |Winbond      |EEPROM |27     |Continual 12V or 14V for program/erase,VPP on OE|
|M27C2001  |ST Micro     |EPROM  |       |VCC=6.5V, VPP=12.75V to pgm|
|M27C4001  |ST Micro     |EPROM  |       |VCC=6.5V, VPP=12.75V to pgm|
|MCM68766  |Motorola     |EPROM  |       |PGM pulse is 25V|

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
