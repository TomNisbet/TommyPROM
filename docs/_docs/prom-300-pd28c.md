---
title: PromDevice28C
permalink: /docs/prom-device-28c
exerpt: "TommyPROM support for 28C256, 28C64, 28C16 and 29C040 EEPROM"
---

This PromDevice28C module is enabled by default to program 28C256 EEPROMS.

The 28C series EEPROMs, specifically the 28C256, are the original chips that TommyPROM supported.  As a result, these have the best support, with all of the interactive features of the command line.  In addition the basic read and write operations, there are also commands to enable and disable the [Software Data protection (SDP)](28C256-notes) features of 28C chips.  There is also support for the fast block write mode of these chips, allowing a 32KB chip to be programmed in just a few seconds.

## 28C Series 28C256

Tested with chips from several manufacturers including Atmel AT28C256, Xicor X28C256, and CSI CAT28C256.  Also works with the smaller 28C64 and 28C16 chips, although some parameters may need to be adjusted. Some smaller chips do not support SDP, so the _Lock_ and _Unlock_ commands have no effect. Check the datasheet to see if block writes are supported.  Some 28C16s use a 16 byte block instead of the 64 bytes used on the 28C256. It is always safe to just set the block size parameter to zero when declaring the chip to disable block writes altogether.

The 28C chips do not need to be explicitly erased.  Each memory cell is automatically erased during a write operation, so they are more similar to a slow RAM than to other PROMs.

## 29C Series

The 29C0x0 chips are 5V sectored flash chips.  They  are similar to the 28C EEPROMs, except they must erase an entire sector before programming new data.  The start of a block write operation causes the target sector to be automatically erased before the block is written.  If the entire sector is not written in one operation, there is no way to write additional data to the sector because a new block write will erase the sector again.

The write algorithm for the 29C chips is similar to 28C, so it is possible to use that code to write these chips as well with some restrictions.  

All 29C010 and 29C020 chips that have been encountered use a sector size of either 64 bytes or 128 bytes.  A single XModem packet holds 128 bytes of data, so these chips will program without issue.

The 28C040 chips from some manufacturers use a 256 byte sector.  *This will not currently work with TommyPROM*, because each of the 128 byte XModem packets received will do its own block write.  The first write will erase 256 bytes and then write data into the first half of the sector.  The second write will then erase the data just written and then write 128 bytes into the second half of the sector.  To fix this, the code would need to be modified to keep 256 bytes of data from two packets before initiating a block write.

## Verified Chips

|Model     |Manufacturer |Type   |Notes|
|:---      |:---         |:---   |:--- |
|AT28C256  |Atmel        |EEPROM |Fully supported|
|CAT28C256 |Catalyst     |EEPROM |Fully supported|
|X28C256   |Xicor        |EEPROM |Fully supported|
|AT28C64   |Atmel        |EEPROM |Fully supported|
|AT29C010  |Atmel        |Flash  |Only with 128 byte or less sector size|

## Chips to be Tested

|Model     |Manufacturer |Type   |Module |Notes|
|:---      |:---         |:---   |:---   |:--- |
|AT28C16   |Atmel        |EEPROM |       ||
|W29C040   |Winbond      |Flash  |       ||
