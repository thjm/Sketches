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

 static inline void SEND_SYNC(uint16_t ctr) {
   if ( ctr == 1 )
     morseGen.print(F(" KA "));
   else
     morseGen.print(F(" + "));
 }

// send the contents of the "cycle counter"
static inline void SEND_CYCLE_COUNTER(uint16_t ctr) {
    morseGen.print(F(" = nr "));
    morseGen.print(ctr & 0x1ff);
 }

// LDR
static inline void SEND_LDR(uint16_t value) {
  morseGen.print(F("= LDR "));
  morseGen.print(value & 0x1ff);
  morseGen.print(" ");
}

// UBAT
static inline void SEND_UBAT(uint16_t value) {
  morseGen.print(F("= UB "));
  morseGen.print(value);
}

// send raw temperature
static inline void sendRawTemperature(uint8_t sensor,uint16_t raw_temp) {
  morseGen.print(F("= T"));
  morseGen.print((int)sensor);
  morseGen.print(" ");
  morseGen.print(raw_temp & 0x1ff);
  morseGen.print(" ");
}

// send temperature
static inline void sendTemperature(uint8_t sensor,float temp) {
  morseGen.print(F("= T"));
  morseGen.print((int)sensor);
  morseGen.print(" ");
  morseGen.print((int)(temp * 10.0));
  //morseGen.print((float)((int)(temp * 10.0))/ 10.0);
  morseGen.print(" ");
}

// T1
static inline void SEND_T1(float temp) {
#ifdef SEND_RAW_TEMPERATURE
  uint16_t raw_temp = getRawTemperature(temp, TEMPERATURE_PRECISION);
  sendRawTemperature(1, raw_temp);
#else
  sendTemperature(1, temp);
#endif
}

// T2
static inline void SEND_T2(float temp) {
#ifdef SEND_RAW_TEMPERATURE
  uint16_t raw_temp = getRawTemperature(temp, TEMPERATURE_PRECISION);
  sendRawTemperature(2, raw_temp);
#else
  sendTemperature(2, temp);
#endif
}

// T3
static inline void SEND_T3(float temp) {
#ifdef SEND_RAW_TEMPERATURE
  uint16_t raw_temp = getRawTemperature(temp, TEMPERATURE_PRECISION);
  sendRawTemperature(3, raw_temp);
#else
  sendTemperature(3, temp);
#endif
}

// T4
static inline void SEND_T4(float temp) {
#ifdef SEND_RAW_TEMPERATURE
  uint16_t raw_temp = getRawTemperature(temp, TEMPERATURE_PRECISION);
  sendRawTemperature(4, raw_temp);
#else
  sendTemperature(4, temp);
#endif
}

// T5
static inline void SEND_T5(float temp) {
#ifdef SEND_RAW_TEMPERATURE
  uint16_t raw_temp = getRawTemperature(temp, TEMPERATURE_PRECISION);
  sendRawTemperature(5, raw_temp);
#else
  sendTemperature(5, temp);
#endif
}

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
