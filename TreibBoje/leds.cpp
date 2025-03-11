//
// File  : leds.cpp
//
// Code for the LED flashing etc.
//


#include "general.h"

/** Initialize the uC's ports for output. */
void initLED() {

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
}

/** LED "hello" */
void helloLED(int nRounds) {

  for ( int i=0; i<nRounds; ++i ) {
    digitalWrite(LED1, HIGH);
    delay(100);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, HIGH);
    delay(100);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, HIGH);
    delay(100);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, HIGH);
    delay(100);
    digitalWrite(LED4, LOW);
  }
}

/** LED flash */
void flashLED() {

  helloLED(3);

  digitalWrite(LED1, HIGH);
  digitalWrite(LED3, HIGH);
  delay(200);
  digitalWrite(LED1, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED4, HIGH);
  delay(200);
  digitalWrite(LED2, LOW);
  digitalWrite(LED4, LOW);

  helloLED(3);
}
