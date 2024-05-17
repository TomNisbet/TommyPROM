---
title: Chips Supported
permalink: /docs/other-devices
exerpt: "Other PROM device types"
---

Several other chip families have been identified and aquired, but are not yet supported.

## 28F Series

The 28F series flash do not have sectors. The entire chip must be erased before writing new data.  All location must be manually written to logic zero before the erase operation in initiated.  Data is written a byte at a time using a command register to control the programming.  **These chips require a 12V programming voltage that must be present during the programming or erase operations**.  

TommyPROM does not currently support 28F chips, but it would not be difficult to write a driver.  The default 28C driver will read 28F chips.

## 29F Series

The 29F series flash have sectors that must be explicitly erased before writing new data.  Data is written a byte at a time using a command register to control the programming.  

TommyPROM does not currently support 29F chips, but it would not be difficult to write a driver.  The default 28C driver will read 29F chips.

## Chips to be Tested

|Model     |Manufacturer |Type   |Module |Notes|
|:---      |:---         |:---   |:---   |:--- |
|AM28F512  |AMD          |EEPROM |       ||
|AM29F040  |AMD          |EEPROM |       ||
|M29F040   |ST Micro     |       |       ||
|AMS29AF010|AMS          |       |       ||
