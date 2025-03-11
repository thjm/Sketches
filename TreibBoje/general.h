//
// general.h
//
// File with general definitions etc.
//


#ifndef _general_h_
 #define _general_h_

// debug via serial interface
#undef DEBUG

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
// temperatures are sent as raw values (integer)
// otherwise as temp * 10 to cover one decimal digit
#undef SEND_RAW_TEMPERATURE
//#define MORSE_SPEED     20
#define MORSE_SPEED     25

// read photo resistor (LDR)
#undef USE_LDR
// U_{Bat} (battery voltage) reading
#define READ_UBAT

#if (defined USE_LDR) || (defined READ_UBAT)
 #define USE_ADC
#endif // USE_LDR

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

extern OneWire oneWire;
extern DallasTemperature sensors;

extern DeviceAddress gSensor1;
extern DeviceAddress gSensor2;
extern DeviceAddress gSensor3;
extern DeviceAddress gSensor4;
extern DeviceAddress gSensor5;

// generic LED, usually at pint D0
//#define LED         13
#define LED          0

#ifdef USE_LDR
 // define a pin for Photo resistor (ADC input!)
 #define LDR_PIN    A0
#endif // USE_LDR

#ifdef READ_UBAT
 // define a pin for U_{Bat} measurement
 #define UBAT_PIN   A1

 // ADC resolution [mV], see adc.cpp
 #define ADC_RES    18
#endif // READ_UBAT

#ifdef USE_LEDS
 #define LED1       4
 #define LED2       5
 #define LED3       6
 #define LED4       7
#endif // USE_LEDS

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS            2
#define TEMPERATURE_PRECISION  10

// delay between transmission loop [usec]
#define LOOP_DELAY   5*1000
//#define LOOP_DELAY  10*1000

// local prototypes
extern int availableMemory();

// to be found in adc.cpp
extern void initADC();
extern int readLDR();
extern int readUBAT();

// to be found in leds.cpp
extern void initLED();
extern void helloLED(int nRounds=8);
extern void flashLED();

// to be found in tsensor.cpp
extern int getRawTemperature(float temp,int precision=12);
extern void printAddress(DeviceAddress);
extern void printResolution(DeviceAddress);

#endif // _general_h_
