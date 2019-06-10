## hardware Verifier Tool
This tools allows access to individual control lines from the Arduino to verify that the hardware was assembled correctly.

It can be used without a chip installed to scope out address and data lines.  It also offers low-level control when the chip
is installed.  

Note that the comamnds allow for direct writing of the 28C control lines with some exceptions to protect the chip and the host arduino:

* When the O command is used to enable chip output, the arduino data bus is set to INPUT
* When the D command is used to write data from the arduino, the chip output is disabled
* The R command sets the output enable (OE) on the chip, but not the chip enable (CE)
* The L and U commands reset CE, OE, and WE back to disabled on completion and change the data and address

The session below shows how a write fails to a locked chip and then succeeds once the chip is unlocked.

```
Hardware Verifier - 28C series EEPROM

Valid commands are:
  Axxxx         - Set address bus to xxxx
  Dxx           - Set Data bus to xx
  Cs            - Set Chip enable to state (e=enable, d=disable)
  Os            - Set Output enable to state (e=enable, d=disable)
  Ws            - Set Write enable to state (e=enable, d=disable)
  R             - Read and print the value on the data bus
  L             - Send Lock sequence to enable device Software Data Protection
  U             - Send Unlock sequence to disable device Software Data Protection

#l
Writing the lock code to enable Software Write Protect mode.

#a0000

#ce

#r
22

#d55

#we

#wd

#r
22

#u
Writing the unlock code to disable Software Write Protect mode.

#d33

#we

#wd

#r
33

#cd

```