//
// File   : YaesuCAT.ino
//
// Purpose: Test program for Yaesu CAT
//
// $Id$
//

#define DEBUG

#include "CATutil.h"
#include "YaesuCAT.h"

#ifdef DEBUG
 // http://arduiniana.org/libraries/streaming/
 #include <Streaming.h>
 
 // UART baud rate
 #define UART_BAUD_RATE  9600
#endif // DEBUG

#if defined (__AVR__)
/**
 * From SoftwareSerialExample:
 
   Note:
    Not all pins on the Mega and Mega 2560 support change interrupts,
    so only the following can be used for RX:
    10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69

    Not all pins on the Leonardo support change interrupts,
    so only the following can be used for RX:
    8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).
*
*/
#include <SoftwareSerial.h>

SoftwareSerial txSerial(8, 9); // RX, TX
#else
 // use RX3,TX3 on the Teensy 3.1/3.2 board, Serial1 is identical to Serial and wired to USB
 #define txSerial  Serial3
#endif // (__AVR__)

// local prototypes
#if defined (__AVR__)
static int availableMemory();
#endif // (__AVR__)

void setup() {
  
#ifdef DEBUG
  /* Initialize serial output at UART_BAUD_RATE bps */
  Serial.begin(UART_BAUD_RATE);
  Serial << F("YaesuTest: starting ...") << endl;
  
 #if defined (__AVR__)
  Serial << F("Free SRAM: ") << availableMemory() << endl;
 #endif // (__AVR__)
#endif // DEBUG

  // set the data rate for the SoftwareSerial port(s)
  txSerial.begin(4800);  // menu item 14: 4800/9600/38400
}

YaesuCAT FT817(txSerial);

void loop() {

  uint32_t frequency = FT817.getFrequency();
  byte mode = FT817.getMode();
  
  static unsigned long loop_init = millis();

#if 1
  // request frequency and mode settings...
  static unsigned long r_time = millis();
  
  if ( (frequency == YaesuCAT::ILLEGAL_FREQ || mode == YaesuCAT::ILLEGAL_MODE)
       && (millis() - r_time > 200 ) ) {

    FT817.requestFrequencyAndMode();
    
    r_time = millis();
  }
#endif

  uint32_t desired_frequency = 28175000; // 70.175 MHz transverted into 10m Band
  byte desired_mode = YaesuCAT::eModeUSB;

#if defined (__AVR__)
  txSerial.listen();
#endif // (__AVR__)

  // check if 'txSerial' has data to receive
  if (txSerial.available()) {

    FT817.read();

  } // txSerial.available()

#if 1
  // set desired frequency and mode (one quantity at a time)
  static unsigned long s_time = millis();
  
  // send message to the rig
  if ( millis() - s_time > 500 ) {
    
    s_time = millis();

    if ( frequency != desired_frequency )
      FT817.writeFrequency(desired_frequency);
    else if ( mode != desired_mode )
      FT817.writeMode(desired_mode);
  }
#endif

#if 1
  // when everything is settled, read frequency/mode from time to time
  static unsigned long p_time = millis();

  if ( frequency == desired_frequency && mode == desired_mode 
       && ( millis() - p_time) > 5000 ) {

    FT817.requestFrequencyAndMode();
    
    p_time = millis();
  }
#endif

  // display of frequency and mode from time to time
  if ( millis() - loop_init > 2000 ) {
    
    loop_init = millis();

    Serial //<< F("OP mode ") << operation_mode 
           //<< F(" fOK ") << rxFrequencyOK(Icom706)
           //<< F(" mOK ") << rxModeOK(Icom706)
           << F(": Frequency = ") << frequency 
           << F(" Mode = ") << mode << endl;
  }
}


/** 
 *  Try to inquire max. available memory by allocating until heap is exhausted.
 */
#if defined (__AVR__)
static int availableMemory() {

#if 0
  // from: https://devel-ik.fzk.de/wsvn/fd-online/Diverse/Oktokopter/Arduino2Flasher/trunk/Arduino2Flasher.ino
  int size = 10000;
  byte *buf;
  while ((buf = (byte *) malloc(--size)) == NULL);
  free(buf);
  return size;
#else
  // see also:
  // https://learn.adafruit.com/memories-of-an-arduino/measuring-free-memory
  extern int __heap_start, *__brkval; 
  int v; 

  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
#endif
}
#endif // (__AVR__)

