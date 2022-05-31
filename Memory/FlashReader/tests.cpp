
//
// File   : tests.cpp
//
// Purpose: contains code for various tests ...
//

#include "FlashReader.h"

#include "ihex.h"
#include "utils.h"

/** Test code frame ...
  *
  *  Various code and hardware test sequences, to be activated by pre-processor directives
  */
void runTests() {

  static bool first = true;
#if 0
  if ( first ) {

    Serial.println("Fake data:");

    for ( int i=0; i<sizeof(eepromData); ++i )
      eepromData[i] = (uint8_t)(i & 0xff);

    dumpHex(eepromData, sizeof(eepromData), eepromAddr);

    first = false;
  }
#endif

  //testAddressLatches();

  int nBytes = kMAX_BLOCK_SIZE;

#if 0
  Serial.println("EEPROM data (single read):");

  // read 'nBytes' bytes from (E)EPROM
  for ( int i=0; i<nBytes; ++i ) {

    eeprom.setAddress( (uint32_t)(i + eepromAddr) );

    eepromData[i] = eeprom.read();
  }

  dumpHex(eepromData, nBytes, eepromAddr);
#endif

#if 1
  Serial.println("EEPROM data (block read):");

  eeprom.read(eepromAddr, eepromData, nBytes);
  dumpHex(eepromData, nBytes, eepromAddr);
#endif

  delay(5000);
}

/** Test code to test the address latching unit.
  *
  * The address bits are switched through subsequently (shift register mode).
  */
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
