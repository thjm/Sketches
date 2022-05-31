
//
// Test code for the LEDs attached to the Teensy in the 4m Transverter
//
// see also: examples/01.Basics/Blink/Blink.ino
//

#include <Arduino.h>

/** Green LED to indicate RX status. */
uint8_t const kRX_GREEN_LED = 20;
/** Red LED to indicate RX status.  */
uint8_t const kRX_RED_LED = 21;
/** Green LED to indicate TX status. */
uint8_t const kTX_GREEN_LED = 22;
/** Red LED to indicate TX status. */
uint8_t const kTX_RED_LED = 23;


void setup() {

  // initialize some digital pins as output.
  pinMode(kRX_GREEN_LED, OUTPUT);
  pinMode(kRX_RED_LED, OUTPUT);

  pinMode(kTX_GREEN_LED, OUTPUT);
  pinMode(kTX_RED_LED, OUTPUT);
}

void loop() {

  static uint8_t count = 1;

  if ( count & 0x01 )
    digitalWrite(kRX_GREEN_LED, HIGH);
  else
    digitalWrite(kRX_GREEN_LED, LOW);

  if ( count & 0x02 )
    digitalWrite(kRX_RED_LED, HIGH);
  else
    digitalWrite(kRX_RED_LED, LOW);

  if ( count & 0x04 )
    digitalWrite(kTX_GREEN_LED, HIGH);
  else
    digitalWrite(kTX_GREEN_LED, LOW);

  if ( count & 0x08 )
    digitalWrite(kTX_RED_LED, HIGH);
  else
    digitalWrite(kTX_RED_LED, LOW);

#if 0
  if ( ++count == 16 ) count = 0;
#else
  count <<= 1;
  if ( count == 16 ) count = 1;
#endif

  delay(1000);
}
