---
title: PROM Types
permalink: /docs/prom-families
exerpt: "PROM types supported by TommyPROM - EPROM, EEPROM, Flash"
---

## EPROM - Erasable Programmable Read-only Memory

EPROMs can be written once, but must then be erased by exposing them to UV light for
several minutes.  They are usually easily read by TommyPROM, but there is limited
support for programming.

## EEPROM - Electrically Erasable Programmable Read-only Memory

EEPROMs are the easiest PROMs to use.  Modern EEPROMs usually can be erased and
reprogrammed electrically at the individual byte level.  This makes them appear similar to
a slower static RAM.  All of the interactive features of TommyPROM work well with EEPROMs.
Due to their complexity, EEPROMs typically come in smaller sizes than other technologies.
The largest EEPROM in the 28C family is 32K bytes.  

Some older EEPROMs cannot be reprogrammed at the individual byte level and are instead
bulk erased before a new write operation.  Programming and erasing for these chips usually
requires voltages higher than 5V.

## Flash ROM

Flash is similar to EEPROM, in that it can be electrically erased and reprogrammed. Unlike
EEPROMs, Flash cannot erase single bytes individually.  Instead, the chip is arranged into
memory blocks called sectors.  The programmer can erase individual sectors or, sometimes,
the entire chip.  Some chip families require an explicit erase command and others initiate
an erase at the start of a programming operation.

Most of the features of TommyPROM are supported for Flash, but some operations that would
do multiple writes to the same sector work differently.

## OTP ROM - One-time Programmable ROM

One-time Programmable (OTP) ROM is written once, either at the factory or in the field.
These chips cannot be erased and re-written, but they can be read by TommyPROM.
Interesting trivia - some of the field-programmable OTPs are just EPROMs packaged without
the erase window.
