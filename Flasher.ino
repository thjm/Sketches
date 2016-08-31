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
	  - PD2/PD3 not used

	Created 30 Aug 2016
	Hermann-Josef Mathes <dc2ip@darc.de>
	Modified day month year
	By author's name

  $Id$

  Port manipulation: for the Arduino Nano we have:
  - PB0 .. PB5 = D8 .. D13
  - PC0 .. PC5 = A0 .. A5
  - PD0 .. PD7 = D0 .. D7
  See also:
	https://www.arduino.cc/en/Reference/PortManipulation
  
  https://www.arduino.cc/en/Hacking/Atmega168Hardware

*/

// http://arduiniana.org/libraries/streaming/
#include <Streaming.h>

#include "EEprom.h"

// UART baud rate
#define UART_BAUD_RATE  9600

EEprom eeprom;

/**  */
void setup() {

  /* Initialize serial output at UART_BAUD_RATE bps */
  Serial.begin(UART_BAUD_RATE);
  Serial << F("Starting ...") << endl;
}

uint8_t addr = 0x01;

/**  */
void loop() {

  Serial.print("0x");
  if ( addr < 0x0f )
    Serial.print("0");
  Serial.println(addr, HEX);
  
  eeprom.setAddressLSB(addr);

  delay(2000);

#if 0
  eeprom.setAddressMSB(addr);

  delay(200);

  eeprom.setAddressHSB(addr);

  delay(2000);
#endif

  addr <<= 1;
  //addr += 1;

  if ( !addr ) addr = 0x01;
}
