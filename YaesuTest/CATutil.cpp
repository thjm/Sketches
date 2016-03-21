
//
// File   : CATutil.cpp
//
// Purpose: Implementation of the functions declared in CATutil.h
//
// $Id$
//


#include <Arduino.h>
#include "CATutil.h"

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

void print(const byte* message,size_t msgLength) {

  for (size_t i=0; i<msgLength; ++i ) {
    printHex(message[i], 2 );
    Serial.print(" ");
  }
  if ( msgLength )
    Serial.println();
}
