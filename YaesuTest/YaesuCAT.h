
//
// File   : YaesuCat.h
//
// Purpose: Header file for the class YaesuCat
//
// $Id$
//


#ifndef _YAESU_CAT_H_
 #define _YAESU_CAT_H_

#if  (defined(__linux) || defined(linux)) && !defined(__ARDUINO_X86__)

  #include <stdint.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <sys/time.h>
  #include <unistd.h> 
#else
  #include <Arduino.h>
  #include <Stream.h>
#endif

#include <stdint.h>
#include <string.h>

#if defined(__ARDUINO_X86__) || (defined (__linux) || defined (linux))
  #undef PROGMEM
  #define PROGMEM __attribute__(( section(".progmem.data") ))
  #define pgm_read_byte(p) (*(p))
  typedef unsigned char byte;
  #define printf_P printf
  #define PSTR(x) (x)
#else
  #include <avr/pgmspace.h>
#endif

/** A class for the communication with Yaesu transceivers.
  *
  * Here only tested with a few commands and the FT-817ND.
  * 
  * - PC to Yaesu rig:
  * 
  * - answer of Yaesu rig:
  */
class YaesuCAT {
public:

  /** CAT special codes. */
  enum {
    MAXLEN = 10,               // maximum message length
    ILLEGAL_MODE = 0xFF,       // no mode
    ILLEGAL_FREQ = 0xFFFFFFFF, // no frequency
  };
  
  /** enum with known CAT commands for the FT-817ND. */
  enum {
    eSET_FRQUENCY   = 0x01,
    eREAD_FREQ_MODE = 0x03,
    eSET_MODE       = 0x07,
    ePOWER_ON       = 0x0F,
    ePOWER_OFF      = 0x8F,
    eREAD_RX_STATUS = 0xE7,
    eREAD_TX_STATUS = 0xF7,

  } eCATcommand;

  /** enum with the modes of the FT-817ND. */
  enum {
    eModeLSB   = 0x00,
    eModeUSB   = 0x01,
    eModeCW    = 0x02,
    eModeCWR   = 0x03,
    eModeAM    = 0x04,
    eModeFM    = 0x08,
    eModeDIG   = 0x0a,
    eModePKT   = 0x0c,

  } eCATmode;

  /** Constructor for the class YaesuCAT, there is no rig-specific address here. */
  YaesuCAT(Stream& stream);

  /** Get the frequency from internal store. */
  uint32_t getFrequency()
   { return myFrequency; }
  /** Get the mode from internal store. */
  byte getMode()
   { return myMode; }
  
  /** Read one byte from the serial stream and put it into the internal message buffer. 
   *  
   *  @return true if ...
   */
  bool read();

  /** Request frequency and mode from the rig. */
  bool requestFrequencyAndMode();
  
  /** Write the desired frequency to the rig. */
  bool writeFrequency(uint32_t frequency);
  /** Write the desired mode to the rig. */
  bool writeMode(byte mode);

protected:
  uint32_t parseFrequency(const byte* msg);
  bool sendMessage(const byte* txMsg,size_t msgLen);

  Stream&   myStream;
  byte      rxMessage[MAXLEN+1];
  int       rxMsgLength;
  int       rxBytesExpected;
  byte      lastCommand;
  
  byte      myMode;
  uint32_t  myFrequency;
};

#endif // _YAESU_CAT_H_
