
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
  * - PC to Yaesu rig (P = parameter):
  *  +----+----+----+----+-----+
  *  | P1 | P2 | P2 | P3 | cmd |
  *  +----+----+----+----+-----+
  * 
  * - answer of Yaesu rig (several possibilities):
  *   - NONE
  *   - requested parameter (RX/TX status):
  *   +--------+
  *   | status |
  *   +--------+
  *   - requested frequency (123.45679 MHz) and mode
  *   +------+------+------+------+------+
  *   | 0x12 | 0x34 | 0x56 | 0x78 | mode |
  *   +------+------+------+------+------+
  */
class YaesuCAT {
public:

  /** CAT special codes. */
  enum {
    MAXLEN = 10,               // maximum message length
    ILLEGAL_MODE = 0xFF,       // no mode
    ILLEGAL_FREQ = 0xFFFFFFFF, // no frequency
  };
  
  /** enum with known CAT commands for the FT-817ND. 
    * See also the 'Opcode Command Chart' on page 72 of the
    * Operating Manual.
    */
  enum {
    eLOCK_ON         = 0x00,
    eSET_FRQUENCY    = 0x01,
    eSPLIT_ON        = 0x02,
    eREAD_FREQ_MODE  = 0x03,
    eCLAR_ON         = 0x05,  // Clarifier
    eSET_MODE        = 0x07,
    ePTT_ON          = 0x08,
    eSET_REP_OFFSET1 = 0x09,
    eSET_DCS_MODE    = 0x0A,
    eSET_CTCSS_TONE  = 0x0B,
    eSET_DCS_CODE    = 0x0C,
    ePOWER_ON        = 0x0F,
    eLOCK_OFF        = 0x80,
    eTOGGLE_VFO      = 0x81,
    eSPLIT_OFF       = 0x82,
    eCLAR_OFF        = 0x85,
    ePTT_OFF         = 0x88,
    ePOWER_OFF       = 0x8F,
    eREAD_RX_STATUS  = 0xE7,
    eSET_CLAR_FREQ   = 0xF5,
    eREAD_TX_STATUS  = 0xF7,
    eSET_REP_OFFSET2 = 0xF9,

  } eCATcommand;

  /** enum with the modes of the FT-817ND, commands 0x03 and 0x07. */
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
  /** Parse frequency in BCD format to integer representation. */
  uint32_t parseFrequency(const byte* msg);
  /** Send string of bytes to the rig. */
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
