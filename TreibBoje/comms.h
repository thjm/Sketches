
//
// comms.h
//
// definitions and inline functions for comms protocol
//


#ifndef _comms_h_
 #define _comms_h_

#include "general.h"

#ifdef SEND_DATA
//
// This is how the data is encoded for the option 'USE_RCSWITCH':
//
// - the ASK message consist of 12 bits
// - 3 bits (a2..a0) are used to specify the sensor
// - the sensor data are put in the
//   remaining 9 bits (d8..d0)
//
//  D11 D10 D9 D8 | D7 D6 D5 D4 | D3 D2 D1 D0
//   a2  a1 a0 d8   d7 d6 d5 d4   d3 d2 d1 d0
//
// - a2a1a0 = 0 specifies the cycle counter
// - a2a1a0 = 1..5 specifies the temperature sensors
//            which therefor operate in 9 bit mode
// - a2a1a0 = 6 specifies the LDR message
// - a2a1a0 = 7 specifies sync messages
//  
 #if (defined USE_RCSWITCH)

  #define SEND_SYNC(_x) theSender.send((_x & 0x1ff) | 0x0e00, 12)
  #define SEND_CYCLE_COUNTER() theSender.send((gCycleCounter & 0x1ff), 12)
  #define SEND_LDR(_x) theSender.send((_x & 0x1ff) | (0x0600 << 1), 12)
  #define SEND_T1(_x) theSender.send((_x & 0x1ff) | (0x0100 << 1), 12)
  #define SEND_T2(_x) theSender.send((_x & 0x1ff) | (0x0200 << 1), 12)
  #define SEND_T3(_x) theSender.send((_x & 0x1ff) | (0x0300 << 1), 12)
  #define SEND_T4(_x) theSender.send((_x & 0x1ff) | (0x0400 << 1), 12)
  #define SEND_T5(_x) theSender.send((_x & 0x1ff) | (0x0500 << 1), 12)

 #elif (defined USE_MORSE)

  #define SEND_SYNC(_x) { if ( _x == 1 ) \
                            morseGen.print("KA "); \
                          else \
                            morseGen.print("+"); \
                        }
  
  #define SEND_CYCLE_COUNTER() { morseGen.print(" nr "); morseGen.print(gCycleCounter & 0x1ff); }
  // LDR
  //#define SEND_LDR(_x) { morseGen.print(_x & 0x1ff); }
  #define SEND_LDR(_x) { morseGen.print("= LDR "); morseGen.print(_x & 0x1ff); morseGen.print(" "); }
  // T1
  //#define SEND_T1(_x) { morseGen.print(_x & 0x1ff); }
  #define SEND_T1(_x) { morseGen.print("= T1 "); morseGen.print(_x & 0x1ff); morseGen.print(" "); }
  // T2
  //#define SEND_T2(_x) { morseGen.print(_x & 0x1ff); }
  #define SEND_T2(_x) { morseGen.print("= T2 "); morseGen.print(_x & 0x1ff); morseGen.print(" "); }
  // T3
  //#define SEND_T3(_x) { morseGen.print(_x & 0x1ff); }
  #define SEND_T3(_x) { morseGen.print("= T3 "); morseGen.print(_x & 0x1ff); morseGen.print(" "); }
  // T4
  //#define SEND_T4(_x) { morseGen.print(_x & 0x1ff); }
  #define SEND_T4(_x) { morseGen.print("= T4 "); morseGen.print(_x & 0x1ff); morseGen.print(" "); }
  // T5
  //#define SEND_T5(_x) { morseGen.print(_x & 0x1ff); }
  #define SEND_T5(_x) { morseGen.print("= T5 "); morseGen.print(_x & 0x1ff); morseGen.print(" "); }
  //#define SEND_T5(_x) { morseGen << "T5 " << (_x & 0x1ff); }

 #else

  #define SEND_SYNC(_x) {}
  #define SEND_CYCLE_COUNTER() {}
  #define SEND_LDR(_x) {}
  #define SEND_T1(_x) {}
  #define SEND_T2(_x) {}
  #define SEND_T3(_x) {}
  #define SEND_T4(_x) {}
  #define SEND_T5(_x) {}

 #endif // !USE_RCSWITCH && !USE_MORSE

#else // SEND_DATA

 #define SEND_SYNC(_x) {}
 #define SEND_CYCLE_COUNTER() {}
 #define SEND_LDR(_x) {}
 #define SEND_T1(_x) {}
 #define SEND_T2(_x) {}
 #define SEND_T3(_x) {}
 #define SEND_T4(_x) {}
 #define SEND_T5(_x) {}

#endif // SEND_DATA

#endif // _comms_h_
