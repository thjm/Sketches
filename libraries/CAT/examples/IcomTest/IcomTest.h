
//
// File   : IcomTest.h
//
// Purpose: General definitions etc. for the IcomTest.ino project
//
// $Id$
//

#ifndef __IcomTest_h_
 #define __IcomTest_h_

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

// debug via serial interface
#define DEBUG

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

#endif // __IcomTest_h_
