
//
// File   : IcomCAT.h
//
// Purpose: Declaration of the interface for the CAT (CIV) based communication
//          with ICom transceivers
//
// $Id$
//

#ifndef __ICOM_CAT_H_
 #define __ICOM_CAT_H_

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

/** Class which support the ICom CAT protocol.
 *  
 * Cn = command number
 * Sc = sub-command
 *
 * - PC to ICom (IC706MkIIG)
 *
 *  +------+------+----+----+----+----+------+------+
 *  | 0xFE | 0xFE | 58 | E0 | Cn | Sc | Data | 0xFD |
 *  +------+------+----+----+----+----+------+------+
 *
 * - ICom to PC
 *
 *  +------+------+----+----+----+----+------+------+
 *  | 0xFE | 0xFE | E0 | 58 | Cn | Sc | Data | 0xFD |
 *  +------+------+----+----+----+----+------+------+
 *
 * - OK message to PC
 *
 *  +------+------+----+----+------+------+
 *  | 0xFE | 0xFE | E0 | 58 | 0xFB | 0xFD |
 *  +------+------+----+----+------+------+
 *
 * - NG (not good) message to PC
 *
 *  +------+------+----+----+------+------+
 *  | 0xFE | 0xFE | E0 | 58 | 0xFA | 0xFD |
 *  +------+------+----+----+------+------+
 *
 */
class IcomCAT {
public:
  /** CAT special message codes. */
  enum { 
    NG = 0xfa,
    OK = 0xfb,
    COLLISION = 0xfc,
    EOM = 0xfd,
    INTRO = 0xfe,

    MY_ADDRESS = 0xe0,         // standard CIV address of host software
    
    MAXLEN = 20,               // maximum message length
    ILLEGAL_MODE = 0xFF,       // no mode
    ILLEGAL_FREQ = 0xFFFFFFFF, // no frequency
  };

  /** enum with known CIT/CAT commands. */
  enum {
    eTRANSFER_FREQ       = 0x00,
    eTRANSFER_MODE       = 0x01,
    //eGET_BAND_LIMITS    = 0x02,
    eGET_OPERATING_FREQ  = 0x03,
    eGET_OPERATING_MODE  = 0x04,
    eSET_OPERATING_FREQ  = 0x05,
    eSET_OPERATING_MODE  = 0x06,
   
  } eCATCommand;

  /** enum with known rig modes (ICom CIV). */
  enum {
    eModeLSB    = 0x00,
    eModeUSB    = 0x01,
    eModeAM     = 0x02,
    eModeCW     = 0x03,
    eModeRTTY   = 0x04,
    eModeFM     = 0x05,
    eModeWFM    = 0x06,
    eModeCW_R   = 0x07,
    eModeRTTY_R = 0x08,
    
  } eCATMode;
  
  /** Constructor for the class CIV, we pass the CIV address of the transceiver. */
  IcomCAT(Stream& stream,byte address);

  /** Get the frequency from internal store. */
  uint32_t getFrequency()
   { return myFrequency; }
  /** Get the mode from internal store. */
  byte getMode()
   { return myMode; }

  /** Return true if a message is complete for parsing. */
  bool msgIsComplete()
   { return rxMsgComplete; }
  /**  */
  bool parseMessage();
  
  /** Read one byte from the serial stream and put it into the internal message buffer. 
   *  
   *  @return true if a complete message is in the buffer.
   */
  bool read();
  
  /** Requesting the frequency from the rig. */
  bool requestFrequency();
  /** Requesting  the modefrom the rig. */
  bool requestMode();

  /** Write the desired frequency to the rig. */
  bool writeFrequency(uint32_t frequency);
  /** Write the desired mode to the rig. */
  bool writeMode(byte mode);

protected:
  /** Parse frequency in BCD format to integer representation. */
  uint32_t parseFrequency(const byte* message);
  /** Send string of bytes to the rig. */
  bool sendMessage(const byte* txMsg,size_t msgLen);

  Stream&   myStream;
  byte      rigAddress;
  byte      rxMessage[MAXLEN+1];
  int       rxMsgLength;
  bool      rxMsgComplete;

  byte      myMode;
  uint32_t  myFrequency;
};

#endif // __ICOM_CAT_H_
