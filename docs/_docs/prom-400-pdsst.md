---
title: PromDeviceSST
permalink: /docs/prom-device-sst
exerpt: "TommyPROM support for SST39SF010, SST39SF020, and SST39040 Flash"
---

## PromDeviceSST39SF

SST39SF0x0 NOR Flash chips are 5V-only chips that use a command register for program and erase operations.  They use fixed 4K-byte sectors that must be manually erased before a new program operation, but the code manages this transparently.  Whenever a write is started to a new segment, the driver first initiates an erase of that sector. A second write to the same sector will not cause an erase, so it is possible to write to a segment multiple times with no additional steps as long as the writes are to different parts of the sector.  For example, 256 bytes could be written to the start of a sector from one file and then 512 bytes could be written to the end of the sector from another file.

Writing data that spans multiple sectors also works with no additional steps.  When a sector boundary is crossed, the new sector is erased and set as the current sector.

The SST39SF driver supports a manual erase from the command line using the E command. This is only needed if data will be rewritten to the same location after a previous write to that sector.

All programming and erase operations for the 39SF chips require only a single 5V power supply.

## PromDeviceSST28SF

The SST28SF0x0 SuperFlash chips are an earlier version of the 39SF chips, using 256-byte sectors.  The 28SF and 39SF chips are pin compatible, but use different command sets for programming and erasing.  For read-only applications, they should be identical, although the 28SF are slower.

All programming and erase operations for the 28SF chips require only a single 5V power supply.

### SST39SF040

The SST39SF driver supports all chips in this family, so the 040, 020, and 010 chips can be all be used with no code change.  These chips use sectors that must be erased before writing new data.  The code keeps track of the current sector and will automatically do an erase operation whenever a write starts to a new sector.  The _Erase_ command is supported, but is not needed unless overwriting new data to a single sector.

### SST28SF040

This is an earlier version of the SST39SF series chips.  They are pin compatible with the 39SF series, but use a different command set for programming.  Unlike the 39SF, these flash chips support software data protection.  The _Lock_ and _Unlock_ commands can be used to enable and disable SDP from the command line.

## Verified Chips

|Model     |Manufacturer |Type   |Module |Notes|
|:---      |:---         |:---   |:---   |:--- |
|SST39SF040|Microchip    |Flash  |SST39SF|All SST39SF0x0 supported|
|SST28SF040|SST          |Flash  |SST28SF|All SST28SF0x0 supported|

## Chips to be Tested

|Model     |Manufacturer |Type   |Module |Notes|
|:---      |:---         |:---   |:---   |:--- |
