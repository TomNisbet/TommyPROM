# Hardware Verify Script

This is a sketch for low-level debugging of the TommyPROM hardware.  It allows the EEPROM data, address,
and control pins to be individually set so that they can be probed to catch wiring errors.  The usage is
described in the [Troubleshooting documentation page](https://tomnisbet.github.io/TommyPROM/docs/troubleshooting).

This sketch should only be used for debugging if the main TommyPROM sketch does not seem to be working.  To 
get started with a new TommyPROM build, load the TommyPROM sketch into the hardware and try the Unlock, 
Display and Zap commands to test reading and writing.  For example:

```
u
d0 ff
z0
d0 ff
```

The above commands will Unlock the Software Data Protection, Display the first 256 bytes, burn a
sequence of test data into the first 32 bytes, and then Display the first 256 bytes again.
