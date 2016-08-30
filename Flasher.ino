/*
	Flasher.ino

	This sketch reads/writes data from PC flash memory chips in 28 to 32 pin
	chips.
	
	The circuit:
	* low nibble of data lines is connected to PC0 .. PC3 (A0..A3)
	* high nibble of data lines is connected to PD4 .. PD7 (D4..D7)
	* CE is connected to PB5 (D13)
	* OE is connected to PB4 (D12)
	* WE is connected to PB3 (D11)
	* STR1 (Strobe 1) latches LSB of address bus, connected to PB2 (D10)
	* STR2 (Strobe 1) latches MSB of address bus, connected to PB1 (D9)
	* STR3 (Strobe 1) latches HSB of address bus, connected to PB0 (D8)
	* reserved lines:
	  - PC4/PC5 for SDA/SCL (A4/A5)
	  - PD0/PD1 for RxD/TxD (D0/D1)
	  - PD2/PD3

	Created 30 Aug 2016
	Hermann-Josef Mathes <dc2ip@darc.de>
	Modified day month year
	By author's name

	https://www.arduino.cc/en/Reference/PortManipulation
  https://www.arduino.cc/en/Hacking/Atmega168Hardware

*/


#include "EEprom.h"

EEprom eeprom;

/**  */
void setup() {
}

uint8_t addr = 0x01;

/**  */
void loop() {

  eeprom.setAddressLSB(addr);

  delay(2000);

#if 0
  eeprom.setAddressMSB(addr);

  delay(200);

  eeprom.setAddressHSB(addr);

  delay(2000);
#endif

  addr <<= 1;

  if ( !addr ) addr = 0x01;
}
