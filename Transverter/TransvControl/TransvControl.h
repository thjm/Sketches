
//
// File   : TransvControl.h
//
// Purpose:
//
// $Id$
//

#ifndef _TransvControl_h_
 #define _TransvControl_h_

/** Address definition for the ICom706MkIIG transceiver */
uint8_t const kIC706MK2G_ADDR = 0x58;

/** Default RX operating frequency. */
uint32_t const kRxFrequencyDefault = 70175000;
/** Minimum RX operating frequency. */
uint32_t const kRxFrequencyMin = 70000000;
/** Maximum RX operating frequency. */
uint32_t const kRxFrequencyMax = 70200000;

/** Green LED to indicate RX status. */
uint8_t const kRX_GREEN_LED = 20;
/** Red LED to indicate RX status.  */
uint8_t const kRX_RED_LED = 21;
/** Green LED to indicate TX status. */
uint8_t const kTX_GREEN_LED = 22;
/** Red LED to indicate TX status. */
uint8_t const kTX_RED_LED = 23;

/** Leftmost switch. */
uint8_t const kLEFT_SWITCH = 2;
/** Middle switch. */
uint8_t const kMIDDLE_SWITCH = 3;
/** Rightmost switch. */
uint8_t const kRIGHT_SWITCH = 4;

/** struct with all necessary variables. */
typedef struct {

  byte rxMode;
  byte txMode;
  byte desiredMode;

  uint32_t rxFrequency;
  uint32_t txFrequency;
  uint32_t desiredFrequency;
  
} TransvControlRec_t;

#endif // _TransvControl_h_
