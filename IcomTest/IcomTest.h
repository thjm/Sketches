
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

/** address definition for the ICom706MkIIG transceiver */
#define IC706MK2G_ADDR   0x58

#endif // __IcomTest_h_

