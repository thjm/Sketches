
//
// File   : interactive.cpp
//
// Purpose: Contains the code for the interactive (menu like) program version
//

// http://arduiniana.org/libraries/streaming/
// has to be included BEFORE Flash
#include <Streaming.h>

// http://arduiniana.org/libraries/flash/
// https://github.com/mikalhart/Flash/releases
//
// all 'prog_char' types in this library (Flash.h/Flash.cpp) must be replaced by the 'char' type
//
// my 'local copy':
// https://svn.ikp.kit.edu/svn/users/mathes/sketchbook/libraries/Flash
//
#include <Flash.h>

// check version of Flash library, V4 isn't compatible with Arduino 1.5.x and higher
#if (defined FLASH_LIBRARY_VERSION) && (FLASH_LIBRARY_VERSION < 5)
 #error "Flash version >= 5 is required!"
#endif // FLASH_LIBRARY_VERSION

#include "FlashReader.h"

#include "utils.h"
#include "ihex.h"

#if (INTERACTIVE != 0)

//
// flash strings for the main menu
//
FLASH_STRING(help_main, "Main menu, options:\n"
        "a    - enter <a>address from where to read\n"
        "f    - toggle output <f>ormat\n"
        "l    - enter <l>ength of data to read\n"
        "p    - <p>rint some information\n"
        "r    - <r>ead length bytes\n"
        "t    - set EPROM <t>ype\n"
        "h, ? - display this <h>elp\n"
      /*  "Ctrl-] - leave miniterm.py\n" */ );

/** loop() function for the interactive programm version. */
void loopInteractive() {

  static bool first = true;
  static byte output_format = 0;

  uint32_t total_length = 64;
  uint32_t i, len, nBytes;
  
  if ( first ) {
    help_main.print(Serial);
    Serial.print(F("E(E)PROM type: ")); 
    Serial << (int)eeprom.getType() << " (" 
           << EEprom::getTypeString(eeprom.getType()) << ")" << endl;
    first = false;
  }

#if 0
  // test, is working
  uint32_t t = strtol("1000", NULL, 0); Serial.println(t);
  t = strtol("0x1000", NULL, 0); Serial.println(t);
#endif
  
  while (1) {

    int ch = Serial.read();

    // ignore also LFs (from windowish systems)
    if ( ch == -1 || ch == 0x0a ) continue;
    
    switch (ch) {

      case 'a':  // --- set the base address
      case 'A':
          Serial.print(F("\nEnter start address? ")); Serial.flush();
          eepromAddr = readInt32();
          //eepromAddr = Serial.parseInt();
          //Serial.println(eepromAddr);
          Serial.println();
        break;

      case 'f':  // --- toggle output format
      case 'F':
          if ( output_format == 0 ) {
            output_format = 1;
            Serial.println(F("Switched to IHEX output format!"));
          }
          else {
            output_format = 0;
            Serial.println(F("Switched to DUMP output format!"));
          }
        break;

      case 'l':  // --- set the length
      case 'L':
        Serial.print(F("\nEnter block length? ")); Serial.flush();
        total_length = readInt();
        Serial.println();
        break;

      case 'r':  // --- read length bytes starting from address
      case 'R':
          len = total_length;
          while ( len > 0 ) {
            nBytes = min(len, kMAX_BLOCK_SIZE);
            len -= nBytes;
            
            eeprom.read(eepromAddr, eepromData, nBytes);
            if ( output_format == 0 )
              dumpHex(eepromData, nBytes, eepromAddr);
            else
              writeIhexData(Serial, eepromData, nBytes, eepromAddr);
            
            eepromAddr += nBytes;
          }
          if ( output_format == 1 )
            writeIhexEOF(Serial);
        break;

      case 'p':  // --- print some information
      case 'P':
          Serial.println();
          Serial.print(F("E(E)PROM type: ")); 
          Serial << (int)eeprom.getType() << " (" 
                 << EEprom::getTypeString(eeprom.getType()) << ")" << endl;
          Serial.print(F("Size:          ")); printHex(eeprom.getSize(), 6); 
          Serial << " (" << eeprom.getSize() << ")";
          Serial.println();
          Serial.print(F("Start address: ")); printHex(eepromAddr, 6);
          Serial.println();
          Serial.print(F("Block length:    ")); printHex(total_length, 4);
          Serial.println();
        break;
      
      case 't':
      case 'T':
          Serial.println();
          Serial.println(F("Select any possible E(E)PROM type:"));
          Serial.print(EEprom::eEEPROM_2716); Serial.println(F(" - 2716 (2K * 8)"));
          Serial.print(EEprom::eEEPROM_2732); Serial.println(F(" - 2732 (4K * 8)"));
          Serial.print(EEprom::eEEPROM_2764); Serial.println(F(" - 2764 (8K * 8)"));
          Serial.print(EEprom::eEEPROM_27128); Serial.println(F(" - 27128 (16K * 8)"));
          Serial.print(EEprom::eEEPROM_27256); Serial.println(F(" - 27256 (32K * 8)"));
          Serial.print(EEprom::eEEPROM_27512); Serial.println(F(" - 27512 (64K * 8)"));
          Serial.print(EEprom::eEEPROM_27010); Serial.println(F(" - 271001, 27C010 (128K * 8)"));
          Serial.print(EEprom::eEEPROM_27020); Serial.println(F(" - 272001, 27C020 (256K * 8)"));
          Serial.print(EEprom::eEEPROM_27040); Serial.println(F(" - 274001, 27C040 (512K * 8)"));
          Serial.print(EEprom::eEEPROM_NONE); Serial.println(F(" - Cancel"));
          Serial.print(F("E(E)PROM type? ")); Serial.flush();
          eepromType = (EEprom::eEEPROMtype)readInt((uint16_t)EEprom::eEEPROM_NONE, 
                                                    (uint16_t)EEprom::eEEPROM_27040); // Serial.parseInt();
          if (eepromType) {
            eeprom.setType(eepromType);
            eepromAddr = 0;  // start again with first addr
          }
          Serial.println();
        break;

      case 'h':
      case 'H':
      case '?':
          help_main.print(Serial);
        break;

      default:
          Serial << "?" << endl;
    }
  }
}
#endif // INTERACTIVE

