# Arduino based EPROM etc. reader device

---

## Description

Simple, Arduino Nano based device to read contents of E(E)PROM and Flash
chips in 24 ... 32 pin DIP cases, currently only for 5V types.

---

## Future

- program 5V only flash chips
- command line interface and/or remote control suitable interface?

## Requirements

- Arduino 1.5.x (works with 1.9.8)
- Streaming library, http://arduiniana.org/libraries/streaming
- Flash library (read strings from flash memory),
  http://arduiniana.org/libraries/flash

## Hardware specific

### Jumper settings

JP1 = Vcc or A13 to pin 24 of 24-pin chips
JP2 = Vcc or A17 to pin 28 of 28-pin chips
JP3 = !WE/!PGM or A18 to pin 31 of 32-pin chips

### EPROMs/FLASH 2716/2732
- JP1 in Vcc position

### EPROMs/FLASH 2764 .. 27512
- JP1 in A13 position
- JP2 in Vcc position

### EPROMs/Flash 27010..
- JP1 in A13 position
- JP2 in A17 position
- JP3 in !WE position (if FLASH)

# References

- EPROM.pdf from http://cambridgearcade.dyndns.org/files/EPROM.pdf
