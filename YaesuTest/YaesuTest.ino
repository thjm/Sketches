//
// File   : YaesuCAT.ino
//
// Purpose: Test program for Yaesu CAT
//
// $Id$
//

#define DEBUG

#include "YaesuCAT.h"

#ifdef DEBUG
 // http://arduiniana.org/libraries/streaming/
 #include <Streaming.h>
 
 // UART baud rate
 #define UART_BAUD_RATE  9600
#endif // DEBUG

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

SoftwareSerial Serial2(8, 9); // RX, TX

/** Print an integer in hex format, width can be specified, starts always with '0x'. */
extern void printHex(unsigned int,unsigned int width=2);
/** Print a complete char array in hex format, one line. */
extern void printMessage(const byte* message,int msgLength);

// local prototypes
static int availableMemory();
  
void setup() {
  
#ifdef DEBUG
  /* Initialize serial output at UART_BAUD_RATE bps */
  Serial.begin(UART_BAUD_RATE);
  Serial << F("YaesuCAT: starting ...") << endl;

  Serial << F("Free SRAM: ") << availableMemory() << endl;
#endif // DEBUG

  // set the data rate for the SoftwareSerial port(s)
  Serial2.begin(4800);  // menu item 14: 4800/9600/38400
}

void loop() {

  static unsigned long loop_init = millis();

  int rxDataLen = 0;
  byte rxData[20];

  int txDataLen = 0;
  byte txData[8];
  
  Serial2.listen();
  
  // check if 'Serial2' has data to receive
  if (Serial2.available()) {

    byte ch = Serial2.read();

    Serial << F("Rx: "); printHex(ch); Serial.println();

#if 0
    //rxData[rxDataLen++] = ch;
    
    // try to decode the reseived message
    if ( rxDataLen == 5 ) {
      switch (rxData[4]) { // last byte is command byte
        default:
      }
    }
#endif
  } // Serial1.available()

  // send message to the rig
  if ( millis() - loop_init > 2000 ) {
    
    loop_init = millis();

    txData[txDataLen++] = 0x00;
    txData[txDataLen++] = 0x00;
    txData[txDataLen++] = 0x00;
    txData[txDataLen++] = 0x00;
    txData[txDataLen++] = YaesuCAT::eREAD_FREQ_MODE;  // 5 byte reply
    //txData[txDataLen++] = YaesuCAT::eREAD_RX_STATUS;  // 1 byte reply
    //txData[txDataLen++] = YaesuCAT::eREAD_TX_STATUS;  // 1 byte reply

#ifdef DEBUG
    Serial << F("Tx(") << txDataLen << F("): ");
    printMessage(txData, txDataLen);
#endif // DEBUG

    for ( int i=0; i<txDataLen; ++i )
      Serial2.write(txData[i]);

    txDataLen = 0;
  }
}

/** Convert Yaesu rig mode into text. */
void mode2Text(byte mode) {
  
}

/** 
 *  Try to inquire max. available memory by allocating until heap is exhausted.
 */
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

/**
 * Print a hexadecimal number in hex format, with teh given length.
 */
void printHex(unsigned int hex,unsigned int width) {
  Serial.print("0x");
  if ( hex < 16 )
    Serial.print("0");
  if ( width > 2 ) {
    if ( hex < 0xfff )
      Serial.print("0");
    if ( hex < 0xff )
      Serial.print("0");
  }
  if ( width > 4 ) {
    if ( hex < 0xfffff )
      Serial.print("0");
    if ( hex < 0xffff )
      Serial.print("0");
  }
  if ( width > 6 ) {
    if ( hex < 0xfffffff )
      Serial.print("0");
    if ( hex < 0xffffff )
      Serial.print("0");
  }
  Serial.print(hex, HEX);
}

/**
 * Print a string in hexadecimal notation.
 */
void printMessage(const byte* message,int msgLength) {

  for (int i=0; i<msgLength; ++i ) {
    printHex(message[i], 2 );
    Serial.print(" ");
  }
  if ( msgLength )
    Serial.println();
}

