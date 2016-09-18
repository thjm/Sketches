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
	* STR2 (Strobe 2) latches MSB of address bus, connected to PB1 (D9)
	* STR3 (Strobe 3) latches HSB of address bus, connected to PB0 (D8)
	* reserved lines:
	  - PC4/PC5 for SDA/SCL (A4/A5)
	  - PD0/PD1 for RxD/TxD (D0/D1)
	  - PD2/PD3 not used

	Created 30 Aug 2016
	 Hermann-Josef Mathes <dc2ip@darc.de>
	Modified day month year
	By author's name

  $Id$

  Port manipulation, for the Arduino Nano we have:
  - PB0 .. PB5 = D8 .. D13
  - PC0 .. PC5 = A0 .. A5
  - PD0 .. PD7 = D0 .. D7
  See also:
	https://www.arduino.cc/en/Reference/PortManipulation
  
  https://www.arduino.cc/en/Hacking/Atmega168Hardware

*/

#define DEBUG

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

// data buffer
uint8_t eepromData[512];
uint32_t eepromAddr = 0;

/**  */
void loop() {

#if 0
  static bool first = true;

  if ( first ) {

    Serial.println("Fake data:");
    
    for ( int i=0; i<sizeof(eepromData); ++i )
      eepromData[i] = (uint8_t)(i & 0xff);
    
    HexDump(eepromData, sizeof(eepromData), eepromAddr);

    first = false;
  }
#endif

  //testAddressLatches();

  int nBytes = 256;

#if 0
  Serial.println("EEPROM data (single read):");
  
  // read 'nBytes' bytes from (E)EPROM
  for ( int i=0; i<nBytes; ++i ) {

    eeprom.setAddress( (uint32_t)(i + eepromAddr) );
    
    eepromData[i] = eeprom.read();
  }

  HexDump(eepromData, nBytes, eepromAddr);
#endif

#if 1
  Serial.println("EEPROM data (block read):");

  eeprom.read(eepromAddr, eepromData, nBytes);
  HexDump(eepromData, nBytes, eepromAddr);
#endif

  delay(5000);
}

/** Function to create hex dump from data, organized as 8 bit words. */
void HexDump(uint8_t *data,size_t length,uint32_t addr) {

  for ( size_t j=0; j<length/16+1; ++j ) {

    if ( j*16 >= length ) break;

    // to be fixed: https://forum.arduino.cc/index.php?topic=38107.0
    Serial.print(F("0x"));
    if ( addr+j*16 < 0x1000 )
      Serial.print(F("0"));
    if ( addr+j*16 < 0x100 )
      Serial.print(F("0"));
    if ( addr+j*16 < 0x10 )
      Serial.print(F("0"));
    Serial.print(addr+j*16, HEX);   // output real address

    for (size_t i=0; i<16; ++i ) {  // output binary data, 1-byte format
      if ( i == 8 )
        Serial.print( " -");
      if ( j*16+i >= length )
        Serial.print(F("     "));
      else {
        Serial.print(F(" 0x"));
        if ( data[j*16+i] < 0x10 )
          Serial.print(F("0"));
        Serial.print(data[j*16+i], HEX );
      }
    }

    Serial.print(F("  "));

    for (size_t i=0; i<16; ++i ) {  // output char representation (if any)

      if ( j*16+i >= length ) break;

      if ( data[j*16+i]<0x20 || data[j*16+i]>0x7f )
        Serial.print(F("."));
      else
        Serial.write(data[j*16+i]);
    }
    Serial.println();
  }
}

/** Test code to test the address latching unit. */
void testAddressLatches() {

  static uint8_t addr = 0x01;

#ifdef DEBUG
  Serial.print("0x");
  if ( addr < 0x0f )
    Serial.print("0");
  Serial.println(addr, HEX);
#endif // DEBUG

  eeprom.setAddressLSB(addr);

  delay(100);

  eeprom.setAddressMSB(addr);

  delay(100);

  eeprom.setAddressHSB(addr);

  addr <<= 1;
  //addr += 1;

  if ( !addr ) addr = 0x01;

  delay(2500);
}

