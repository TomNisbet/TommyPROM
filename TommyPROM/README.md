# Software Design

The software is designed around several major blocks and classes.  The code has been broken out into individual files to make it easy to extend the hardware support to additional EPROM and EEPROM families. See the [extension readme](../README-extension.md) for details on suggested hardware and software changes needed.

## CommandStatus class
The CommandStatus class stores the execution status of the previous command.  It allows the status to be saved (and recalled using the / command) instead of just printing the status at the completion of the command.  This was important for debugging XMODEM problems, because the error messages would get eaten as part of the transfer.  The class has utility beyond XMODEM because it includes formatting that relieves each command from having to build paramterized error messages with multiple print calls.

## Xmodem class
The Xmodem class implements the communications protocols needed to do XMODEM CRC transmit and receive.  It calls directly into the PROM read and write code, to the complete files are never stored during the transfer.

## CLI code and command implementation
This code parses input commands and parameters and executes the commands.

A compile-time switch in Configure.h enables additional debug commands that are not needed in normal operation, but are very useful to verify proper operation of the hardware.

## PromDevice class
The PromDevice class and its subclasses encapsulate all of the communication between the Arduino and the target PROM device.

The PromDevice class can access the data bus using direct port writes instead of 8 individual pin accesses.  This greatly increases performance, but it makes the code dependent on the particular flavor of Arduino being used.  The code can currently be compiled for Uno, Nano, or Micro versions of Arduino hardware or in a slower hardware-indpendent mode.

The PromDevice class contains common code used by all devices, including the block write code that will break a large write request into a set of properly-aligned smaller blocks for devices that support block writing, or a sequence of infividual byte writes for devices that do not.

Subclasses of the PromDevice class provide the device-specific code. To accomodate the limited memory footprint of the Arduino, only a single subclass will be compiled into the final code. This is controled via switches in the Configure.h file.

The device-specific subclasses are described below.  Each class provides pin definitions, byte read, byte burn, and optional block burn code that is specific to the timing of the chip. They also supply a SetAddress method that either calls the built-in code for the shift register hardware or provices device-specific address code.

### PromDevice28C class
Although the existing code is specific to the 28C256, the constructor has parameters that easily support other chips.  The current design has been used to read other chips, including 2764 and 29c040 EPROMs.

The 28C code supports fast block writes, allowing a 32K byte chip to be programmed in just a few seconds.

### PromDevice8755A class
The PromDevice8755A class provices the byte read and byte write code, as well as SetAddress code that accomodates the ALE used on this chip.  The byte write code controls the Vdd switching hardware that toggles the Vdd pin between the normal 5V value and the 25V programming pulse.

### PromAddressDriver class
Although not actually a subclass of PromDevice, this contains the code to drive the shift registers that provide the address lines. If this hardware is used, the PromDeviceXX class can call this code to implement the SetAddress methods.



