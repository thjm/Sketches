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

### Circuit

FIXME
This s a brief description of the electronic scheme. It should be extended
by a circuit diagram.

- low nibble of data lines is connected to PC0 .. PC3 (A0..A3)
- high nibble of data lines is connected to PD4 .. PD7 (D4..D7)
- !CE is connected to PB5 (D13)
- !OE is connected to PB4 (D12)
- !WE is connected to PB3 (D11)
- STR1 (Strobe 1) latches LSB of address bus, connected to PB2 (D10)
- STR2 (Strobe 2) latches MSB of address bus, connected to PB1 (D9)
- STR3 (Strobe 3) latches HSB of address bus, connected to PB0 (D8)
- reserved lines:

  - PC4/PC5 for SDA/SCL (A4/A5)
  - PD0/PD1 for RxD/TxD (D0/D1)
  - PD2/PD3 not used
FIXME

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
