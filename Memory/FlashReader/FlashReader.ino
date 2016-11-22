/*
	FlashReader.ino

	This sketch reads data from EPROM chips and reads/writes data from/to PC flash 
	memory chips (EEPROM) in 24, 28 and 32 pin cases. Writing to Flash will be 
	implemented in a future release.
	
	Created 30 Aug 2016
	 Hermann-Josef Mathes <dc2ip@darc.de>

	Modified day month year
	By author's name

  $Id$

*/


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

// EEPROM object
EEprom eeprom;

// data buffer etc.
uint8_t eepromData[kMAX_BLOCK_SIZE];
uint32_t eepromAddr = 0;
//EEprom::eEEPROMtype eepromType = EEprom::eEEPROM_2732;
EEprom::eEEPROMtype eepromType = EEprom::eEEPROM_27020;
static int verbosity = 0;

/**  */
void setup() {

  /* Initialize serial output at UART_BAUD_RATE bps */
  Serial.begin(UART_BAUD_RATE);
#ifdef DEBUG
  Serial << F("FlashReader.ino starting ...") << endl;
#endif // DEBUG

  eeprom.setType(eepromType);
}


/**  */
void loop() {

#ifdef TEST
  testCode();
#endif // TEST

#if (INTERACTIVE != 0)
  loopInteractive();
#else
  loopNonInteractive();
#endif // INTERACTIVE
}

#if (INTERACTIVE == 0)
 // Calculated based on max input size expected for one command
 #define INPUT_SIZE   64
 // Maximum number of options expected
 #define MAX_OPTIONS   5

static char inputLine[INPUT_SIZE+1] = { 0 };
static int lineLength = 0;
static int errorCode = 0;

/** Reads characters from the serial inputs, puts them into the 'inputLine' 
 *  buffer and returns 'true' when a complete line has been read.
 *  
 *  The maximum length of the input line is limited by the INPUT_SIZE constant.
 */
static bool getInputLine() {

  static bool lineComplete = false;

  if ( lineComplete ) lineComplete = false;
  
  // read characters from serial interface and put them in input buffer until EOL
  int ch = Serial.read();

  if ( ch == -1 ) return false;

  // CR will be ignored
  if ( ch == 0x0d ) return false;
  
  // check for LF (aka. EOL) or add character to inputLine
  if ( ch == 0x0a ) {
    lineComplete = true;
  }
  else {
    // Add the final 0 to end the C (ASCIIZ) string
    inputLine[lineLength++] = ch;
    inputLine[lineLength] = 0;
  
    // too many chars entered? clear line buffer and issue warning
    if ( lineLength == INPUT_SIZE ) {
      lineLength = 0;
      inputLine[lineLength] = 0;
      Serial << F("ERR: max. line length exceeded!") << endl;
    }
  }

  return lineComplete;
}

/** Tokenize the given string 'inputLine' using the separator 'separator'. Upon initialisation, the max.
  * number of tokens (options) must be passed via the variable 'nopts'.
  * 
  * The functions returns the number of found tokens (options) in the variable 'nopts' and their
  * values in the array 'option' which must be sufficiently large to hold 'nopts' entries.
  * 
  * The function returns 'true' when the line is properly terminated (CR/LF) otherwise it has to be called
  * again (in order to complete the line).
  */
bool tokenize(const char *inputline,int& nopts,char **options,const char *separator=" ") {

  int max_nopts = nopts;
  
  nopts = 0;
  
  char* token = strtok(inputLine, separator);
  bool err = false;

  while ( token && !err ) {

    if ( nopts == max_nopts ) {
      err = true;
      Serial << F("ERR: maximum number of options exceeded") << endl;
    }
    else
      options[nopts++] = token;

    // find next token
    token = strtok(0, separator);
  }

  return err;
}

/** Check passed parameters and execute the 'read' command. */
int execReadCommand( /* EEprom& eeprom, */ int nopts,char *option[]) {

  // requires exactly two parameters
  if ( nopts != 3 ) {
    Serial << F("ERR: number of parameters mismatch") << endl;
    return 1;
  }

  eepromAddr = atoi(option[1]);
  size_t total_length = atoi(option[2]);

  if ( eepromAddr >= eeprom.getSize() ) {
    Serial << F("ERR: start address outside E(E)PROM address range") << endl;
    return 1;
  }
  
  if ( total_length <= 0 ) {
    Serial << F("ERR: illegal length parameter") << endl;
    return 1;
  }

  Serial << F("OK") << endl;
              
  size_t len = total_length;

  // if the end address is beyond the size of the E(E)PROM then reduce it accordingly
  if ( (eepromAddr+len) >= eeprom.getSize() )
    len -= eepromAddr+len - eeprom.getSize();

  while ( len > 0 ) {

    // limit number of bytes to be read into buffer
    size_t nBytes = min(len, kMAX_BLOCK_SIZE);
    len -= nBytes;

    // read data from E(E)PROM into local buffer
    eeprom.read(eepromAddr, eepromData, nBytes);
    // and output buffered data in IHEX format
    writeIhexData(Serial, eepromData, nBytes, eepromAddr);

    eepromAddr += nBytes;
  }

  // do we really want this?
  writeIhexEOF(Serial);

  return 0;
}

/** Check passed parameters and execute the 'write' command. */
int execWriteCommand( /* EEprom& eeprom, */ int nopts,char *option[]) {

  // requires only one parameters, the IHEX string
  if ( nopts != 2 ) {
    Serial << F("ERR: number of parameters mismatch") << endl;
    return 1;
  }
 
  char *ihex_string = option[1];
  size_t len;

  uint8_t code;
  uint16_t addr;
            
  if ( !parseIhexString(ihex_string, eepromData, addr, len, code) ) {
    Serial << F("ERR: parsing Ihex string") << endl;
    return 1;
  }

  // only code=1 contains relevant data
  if ( code == 0 ) {
    Serial << F("Line: len=") << len << F(" addr=") << addr 
           << F(" code=") << code << endl;
            
    if ( addr >= eeprom.getSize() ) {
      Serial << F("ERR: start address outside E(E)PROM address range") << endl;
      return 1;
    }
    if ( (addr+len) >= eeprom.getSize() ) {
      Serial << F("WRN: end address will be outside E(E)PROM address range") << endl;
      //return 1;
    }

    // now write the data into the device
    for ( eepromAddr=addr; eepromAddr<min(addr+len,eeprom.getSize()); ++eepromAddr) {
      Serial << F("Writing to 0x"); printHex16(Serial, eepromAddr); 
      Serial << F(": 0x"); printHex8(Serial, eepromData[eepromAddr-addr]); Serial << endl;
    }
              
    Serial << F("next address 0x"); printHex16(Serial, eepromAddr); Serial << endl;
  }
            
  Serial << F("OK") << endl;

  return 0;
}

/** Check passed parameters and execute the 'type' command. */
int execTypeCommand( /* EEprom& eeprom, */ int nopts,char *option[]) {

  // requires exactly one parameter
  if ( nopts != 2 ) {
    Serial << F("ERR: number of parameters mismatch") << endl;
    return 1;
  }

  if ( *option[1] == '?' ) {
    Serial << F("OK") << endl;
    Serial << F("T ") << (int)eepromType << endl;
    return 0;
  }

  if (    atoi(option[1]) > (int)EEprom::eEEPROM_2716
                    || atoi(option[1]) <= (int)EEprom::eEEPROM_27040 ) {
    Serial << F("OK") << endl;
    eepromType = (EEprom::eEEPROMtype)atoi(option[1]);
    eeprom.setType( eepromType );
  }
  else {
    Serial << F("ERR: parameter range mismatch") << endl;
    return 1;
  }

  return 0;
}

/** loop() function for the non-interactive version. */
void loopNonInteractive() {

  if ( getInputLine() ) {

    int err = 0;

#if 0
    Serial << strlen(inputLine) << " '" << inputLine << "'" << endl;
#endif
    //
    // valid commands:
    // 't ?' - get E(E)PROM type
    // 't <t>' - set E(E)PROM type
    // 'r <addr> <len>' - read <len> bytes starting from address <addr>
    // 'w <addr> <bytes>' - write <bytes> (in IHEX format) to address <addr> 
    //

    char *command = NULL;
    int nopts = MAX_OPTIONS;
    char *option[MAX_OPTIONS];

    err = tokenize(inputLine, nopts, option, " ");

    if ( !err && nopts ) {
      command = option[0];

      if ( strlen(command) != 1 ) {
        Serial << F("ERR: no command found!") << endl;
        err = 1;
      }
    }

#if 0
    // final control output after tokenisation
    Serial << F("nopts= ") << nopts << endl;
    if ( command && !err ) {
      Serial << F("command= '") << command << "'" << endl;
      for ( int i=0; i<nopts; ++i )
        Serial << F("option[") << i << F("]= '") << option[i] << "'" << endl;
    }
#endif

    if ( !err ) {
      
      switch ( command[0] ) {

        case 't': 
        case 'T':
          err = execTypeCommand(nopts, option);
        break;

        case 'r': 
        case 'R':
          err = execReadCommand(nopts, option);
        break;

        case 'w': 
        case 'W':
          err = execWriteCommand(nopts, option);
        break;

        case '?': 
          Serial << F("OK") << endl;
          Serial << F("R T V W") << endl;
        break;

        default:
          Serial << F("ERR: unknown command entered!") << endl;
          err = true;
      }
    }

    lineLength = 0;
    inputLine[lineLength] = 0;
  }
}
#endif // INTERACTIVE



/** Test code frame ... 
  *  
  *  Various code and hardware test sequences, to be activated by pre-processor directives
  */
void testCode() {
  
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

