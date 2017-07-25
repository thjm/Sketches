
//
// File  : adc.cpp
//
// Code for the ADC specific parts, e.g. LDR, U_Bat, etc.
//

#include "general.h"

void initADC() {

  //           PhotoR     10K
  // +5    o---/\/\/--.--/\/\/---o GND
  //                  |
  // Pin 0 o----------+
  //
  pinMode(LDR_PIN, INPUT);
  
  // change range and resolution of analog pin, default
  analogReference(DEFAULT);
}

int readLDR() {

  // divide the resulting value:
  // - with a 10k resistor divide the value by 2
  // - for 100k resistor divide by 4.
  // - this is a 10bit (0..1023) ADC
  int ldr_value = analogRead(LDR_PIN)/4;

#ifdef DEBUG
  Serial << "LDR: " << ldr_value << endl;
#endif // DEBUG

  return ldr_value;
}
