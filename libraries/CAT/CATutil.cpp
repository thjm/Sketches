//
// File   : CATutil.cpp
//
// Purpose: Implementation of the functions declared in CATutil.h
//


#include <Arduino.h>
#include "CATutil.h"

int CATutil::getNibble(const byte* s,int i) {

  byte k = s[i/2];
  if ( i % 2 == 0 )
    k = k >> 4;
  return k & 0x0f;
}

void CATutil::print(unsigned int hex,unsigned int width) {

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

void CATutil::print(const byte* message,size_t msgLength) {

  for (size_t i=0; i<msgLength; ++i ) {
    print(message[i], 2 );
    Serial.print(" ");
  }
  if ( msgLength )
    Serial.println();
}
