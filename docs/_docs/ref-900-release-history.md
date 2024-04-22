---
title: TommyPROM Release History
permalink: /docs/release-history
exerpt: "What's New in TommyPROM"
---

## TommyPROM Release History

|Revision |Date_added |Notes|
|:---:    |:---:      |:--- |
|1.0      |2017-02-27 |Initial version to support 28C256 EEPROMs|
|1.5      |2018-05-10 |Split into modular PromDevice drivers to support 8655A|
|1.6      |2018-12-22 |Early suport for 27C devices |
|1.7      |2019-06-08 |Add Lock command for 28C256 |
|1.8      |2019-06-09 |Optimize address register code to meet unlock timing requirements and create HardwareVerify sketch|
|2.0      |2019-07-19 |Extend addressing to 19 bits to read larger chips, like the 29C040|
|         |2020-08-08 |Add dedicated sketch to unlock 28C256 chips using Ben Eater hardware design|
|2.1      |2020-08-15 |Add Poke command|
|2.2      |2020-08-24 |Add support for 74LS595 shift registers for addressing and fix XModem ACK issue|
|2.3      |2020-08-26 |Fix addressing bug|
|2.4      |2020-10-10 |Add XModem debugging information and fix #17 for AT28C256 write failures|
|2.5      |2020-10-14 |Change XModem default to 8-bit checksum instead of CRC16|
|2.6      |2020-11-11 |Change XModem default back to CRC16|
|2.7      |2020-11-14 |Additional XModem protocol error handling|
|3.0      |2021-08-12 |Support for SST39SF0x0 flash devices|
|3.1      |2022-12-10 |Support for 27C257/27E257 EEPROM|
|3.2      |2022-12-13 |Support for SST28SF flash|
|3.3      |2022-12-14 |Support 29C0x0 flash|
|         |2022-12-23 |Documentation for TommyPROM32 PCB|
|3.4      |2024-04-09 |Support 2316,2332, and 2364 mask-programmed ROMs|
|3.5      |2024-04-12 |jcranes's updates to CLI for better parameter defaults and additional error checking|
|3.6      |2024-04-23 |Remove debug commands by default - can be enabled in Configure.h|
