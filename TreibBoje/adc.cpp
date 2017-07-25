
//
// File  : adc.cpp
//
// Code for the ADC specific parts, e.g. LDR, U_Bat, etc.
//

#include "general.h"

void initADC() {

#ifdef USE_LDR
  //             PhotoR     10k
  // +5      o---/\/\/--.--/\/\/---o GND
  //                    |
  // LDR_PIN o----------+
  //
  pinMode(LDR_PIN, INPUT);
#endif // USE_LDR

#ifdef READ_UBAT
  //              33k       10k
  // U_in    o---/\/\/--.--/\/\/---o GND
  //                    |
  // U_ADC_  o----------+
  //
  // U_ADC = 10k * U_in / 43k
  // ADC_counts = U_ADC / U_ref * 1024; U_ref = 5V
  //
  // => resolution: 21 mV
  //
  pinMode(UBAT_PIN, INPUT);
#endif // READ_UBAT

  // change range and resolution of analog pin, default is 5V
  analogReference(DEFAULT);
}

int readLDR() {

#ifdef USE_LDR
  // divide the resulting value:
  // - with a 10k resistor divide the value by 2
  // - for 100k resistor divide by 4.
  // - this is a 10bit (0..1023) ADC
  int ldr_value = analogRead(LDR_PIN)/4;

#ifdef DEBUG
  Serial << "LDR: " << ldr_value << endl;
#endif // DEBUG

  return ldr_value;
#else
  return 0;
#endif
}

int readUBAT() {

  int ubat_value = analogRead(UBAT_PIN);

#ifdef DEBUG
  Serial << "UBat: " << ubat_value << endl;
#endif // DEBUG

  return ubat_value;
}
