
//
// File   : YaesuCat.h
//
// Purpose: Header file for the class YaesuCat
//
// $Id$
//


#ifndef _YAESU_CAT_H_
 #define _YAESU_CAT_H_

/** A class for the communication with Yaesu transceivers.
  *
  * Here only tested with a few commands and the FT-817ND.
  */
class YaesuCAT {
public:

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

};

#endif // _YAESU_CAT_H_
