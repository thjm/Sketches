
//
// File   : IcomTest.ino
//
// Purpose: Test program for ICom CAT (CIV)
//
// $Id$
//

#include "IcomTest.h"

#ifdef DEBUG
 // http://arduiniana.org/libraries/streaming/
 #include <Streaming.h>
 
 // UART baud rate
 #define UART_BAUD_RATE  9600
#endif // DEBUG

#include <CATutil.h>
#include <IcomCAT.h>

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

SoftwareSerial rxSerial(10, 11); // RX, TX for the RX
#else
 // use RX2,TX2 on the Teensy 3.1/3.2 board, Serial1 is identical to Serial and wired to USB
 #define rxSerial  Serial2
#endif // (__AVR__)

IcomCAT Icom706(rxSerial,IC706MK2G_ADDR);

// local prototypes
#if defined (__AVR__)
static int availableMemory();
#endif // (__AVR__)

// code for setup and initialisation
void setup() {
  
#ifdef DEBUG
  /* Initialize serial output at UART_BAUD_RATE bps */
  Serial.begin(UART_BAUD_RATE);
  Serial << F("CATtransv: starting ...") << endl;

 #if defined (__AVR__)
  Serial << F("Free SRAM: ");
  Serial.println(availableMemory());
 #endif // (__AVR__)
#endif // DEBUG

  // set the data rate for the SoftwareSerial port(s)
  rxSerial.begin(9600);
  //txSerial.begin(9600);
}

/** Return true if the frequency is in the desired range. */
bool rxFrequencyOK(IcomCAT& icom) {

  uint32_t frequency = icom.getFrequency();

  //if ( frequency == IcomCAT::ILLEGAL_FREQ ) return false;
  if ( frequency < 70000000 ) return false;
  if ( frequency >= 70200000 ) return false;

  return true;
}

/** Return true if the mode is either USB or CW. */
bool rxModeOK(IcomCAT& icom) {

  byte mode = icom.getMode();
  
  //if ( mode == IcomCAT::ILLEGAL_MODE ) return false;
  if ( mode == IcomCAT::eModeUSB ) return true;
  if ( mode == IcomCAT::eModeCW ) return true;
  
  return false;
}

/** Mode 0 is active if either mode or frequency haven't been read from the rig. */
static void loop_mode0(IcomCAT& icom,uint32_t& frequency,byte& mode) {

  static unsigned long r_time = millis();
  
  if ( frequency == IcomCAT::ILLEGAL_FREQ 
       && (millis() - r_time) > 100 ) {
    
    icom.requestFrequency();

    r_time = millis();
  }  

  if ( mode == IcomCAT::ILLEGAL_MODE  
       && (millis() - r_time) > 100 ) {
       
    icom.requestMode();
    
    r_time = millis();
  }
}

/** Mode 1 is active if the frequency is not in the right range. */
static void loop_mode1(IcomCAT& icom,uint32_t& frequency,byte& mode) {

  static unsigned long s_time = millis();

  if ( !rxFrequencyOK(icom) && (millis() - s_time) > 200 ) {

    icom.writeFrequency(70175000);
    
    s_time = millis();
  }
}

/** Mode 2 is active if the mode is not correct. */
static void loop_mode2(IcomCAT& icom,uint32_t& frequency,byte& mode) {
  
  static unsigned long s_time = millis();

  if ( !rxModeOK(icom) && (millis() - s_time) > 200 ) {

    icom.writeMode(IcomCAT::eModeUSB);
    
    s_time = millis();
  }
}

// main loop, program logic:
// - check RX:
//   - if frequency in range 70000 .. 70200 kHz -> OK
//     else change to 70175
//   - if mode USB -> OK, else change to USB
// - set TX:
//   - mode = RX mode
//   - frequency = ...
//
void loop() {

  static unsigned long loop_init = millis();
  
  byte mode = Icom706.getMode();
  uint32_t frequency = Icom706.getFrequency();

  static byte operation_mode = 0;

  // set the operation_mode appropriately
  if ( frequency == IcomCAT::ILLEGAL_FREQ || mode == IcomCAT::ILLEGAL_MODE )
    operation_mode = 0;
  else if ( !rxFrequencyOK( Icom706 ) )
    operation_mode = 1;
  else if ( !rxModeOK( Icom706 ) )
    operation_mode = 2;
  else
    operation_mode = 3;

  switch ( operation_mode ) {
    
    case 0: loop_mode0(Icom706, frequency, mode);
            break;

    case 1: loop_mode1(Icom706, frequency, mode);  // set right frequency
            break;

    case 2: loop_mode2(Icom706, frequency, mode);  // set right mode
            break;

    default: ;
  }

#if defined (__AVR__)
  rxSerial.listen();
#endif // (__AVR__)

  // check if 'Serial1' has data to receive
  if (rxSerial.available()) {

    Icom706.read();
    
    if (Icom706.msgIsComplete()) {
      Icom706.parseMessage();
    }
    
  } // rxSerial.available()

  // check if 'Serial2' has data to receive
  // ...

  // display of frequency and mode from time to time
  if ( millis() - loop_init > 2000 ) {
    
    loop_init = millis();

    Serial << F("OP mode ") << operation_mode 
           << F(" fOK ") << rxFrequencyOK(Icom706)
           << F(" mOK ") << rxModeOK(Icom706)
           << F(": Frequency = ") << frequency 
           << F(" Mode = ") << mode << endl;
  }
}

/**
 * Try to inquire max. available memory by allocating until heap is exhausted.
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

// code snippet for XXX specific code fragment
#if defined(CORE_TEENSY) || defined(__AVR__) || defined(__ARM__)
// ...
#endif
