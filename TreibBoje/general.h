
//
// general.h
//
// File with general definitions etc.
//

#ifndef _general_h_
 #define _general_h_

// debug via serial interface
#define DEBUG

// scan sensors (to inquire addresses)
#define SCAN_SENSORS
// read temperature sensors
#define READ_SENSORS

// send the data via 433MHZ Tx
#define SEND_DATA
// send the data using the RCSwitch library
#undef USE_RCSWITCH
// send the data using the Morse library
#define USE_MORSE
//#define MORSE_SPEED     20
#define MORSE_SPEED     25

// read photo resistor (LDR)
#undef USE_LDR

// use the RTC for time tagging the data
#define USE_RTC

// use up to four LEDs, will blink
#define USE_LEDS

#ifdef SEND_DATA
 #define TX_PIN     10
 //#define TX_PIN     13
#endif // SEND_DATA

#ifdef USE_RTC
// The required RTClib is from Adafruit:
// https://github.com/adafruit/RTClib
// Which is a fork of the RTClib from JeeLab:
// https://github.com/jcw/rtclib

 #include <Wire.h>     // RTClib won't work without this
 #include <RTClib.h>
#endif // USE_RTC

#ifdef DEBUG
 // http://arduiniana.org/libraries/streaming/
 // has to be included BEFORE Flash
 #include <Streaming.h>

 // UART baud rate
 #define UART_BAUD_RATE  9600
#endif // DEBUG

#ifdef SEND_DATA
 #if (defined USE_RCSWITCH) && (defined USE_MORSE)
  #error "Cannot use both RCSwitch and Morse!"
 #endif
 #if (defined USE_RCSWITCH)
  #include <RCSwitch.h>
 
 /** Instance of tht RCSwitch class. */
 extern RCSwitch theSender;
 #endif // USE_RCSWITCH
 #if (defined USE_MORSE)
  #include "Morse.h"

 /** Instance of the Morse class. */
 extern Morse morseGen;
 #endif // USE_MORSE
#endif // SEND_DATA

// http://www.pjrc.com/teensy/td_libs_OneWire.html
#include <OneWire.h>

// https://github.com/milesburton/Arduino-Temperature-Control-Library
// http://milesburton.com/Main_Page?title=Dallas_Temperature_Control_Library
// Note:
// The library archive name is not well chosen and has to be renamed into DallasTemperature
//
// DS18B20       +-------+
//               |       |
//               | 18B20 |
//               |       |
//               ++--+--++
//                |  |  |
//              GND Dq  Vdd
//
#include <DallasTemperature.h>

//#define LED         13
#define LED          0

#ifdef USE_LDR
 // define a pin for Photo resistor (ADC input!)
 #define LDR_PIN    A0
#endif // USE_LDR

#ifdef USE_LEDS
 #define LED1       4
 #define LED2       5
 #define LED3       6
 #define LED4       7
#endif // USE_LEDS

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS            2
#define TEMPERATURE_PRECISION  10

// local prototypes
extern int availableMemory();

// Tbd: put in tsensor.cpp
extern void printAddress(DeviceAddress);
extern void printResolution(DeviceAddress);

#endif // _general_h_
