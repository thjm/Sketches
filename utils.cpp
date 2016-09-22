
//
// File:    utils.cpp
//
// Purpose: Collection of some useful functions
//
// $Id$
//


// http://arduiniana.org/libraries/streaming/
#include <Streaming.h>

#include "utils.h"

// ---------------------------------------------------------------------------

void dumpHex(uint8_t *data,size_t length,uint32_t addr) {

  for ( size_t j=0; j<length/16+1; ++j ) {

    if ( j*16 >= length ) break;

    // to be fixed: https://forum.arduino.cc/index.php?topic=38107.0
    Serial.print(F("0x"));
    if ( addr+j*16 < 0x1000 )
      Serial.print(F("0"));
    if ( addr+j*16 < 0x100 )
      Serial.print(F("0"));
    if ( addr+j*16 < 0x10 )
      Serial.print(F("0"));
    Serial.print(addr+j*16, HEX);   // output real address

    for (size_t i=0; i<16; ++i ) {  // output binary data, 1-byte format
      if ( i == 8 )
        Serial.print( " -");
      if ( j*16+i >= length )
        Serial.print(F("     "));
      else {
        Serial.print(F(" 0x"));
        if ( data[j*16+i] < 0x10 )
          Serial.print(F("0"));
        Serial.print(data[j*16+i], HEX );
      }
    }

    Serial.print(F("  "));

    for (size_t i=0; i<16; ++i ) {  // output char representation (if any)

      if ( j*16+i >= length ) break;

      if ( data[j*16+i]<0x20 || data[j*16+i]>0x7f )
        Serial.print(F("."));
      else
        Serial.write(data[j*16+i]);
    }
    Serial.println();
  }
}

// ---------------------------------------------------------------------------

static void printByteHex(Stream& stream,uint8_t data) {
  if ( data < 0x10 )
    stream.print("0");
  stream.print(data, HEX);
}

void printHex(uint32_t number,size_t precision) {

  char tmp[20];
  char format[128];

  sprintf(format, "0x%%.%dX", min(precision,16));

  sprintf(tmp, format, number);
  Serial.print(tmp);
}

// ---------------------------------------------------------------------------

uint16_t readInt(void) {

  uint32_t num = 0;
  uint16_t c;
  byte digits[READ_INT_MAX_DIGITS];
  int8_t i = READ_INT_MAX_DIGITS-1;

  while (Serial.available() > 0)       /* clear old garbage */
    Serial.read();

  for (;;) {
    if (Serial.available() <= 0)
      continue;
    c = Serial.read();
    if ((c < '0') || (c > '9'))     // non numeric
      break;
    Serial.print(c - '0', DEC);     // echo input
    digits[i--] = c - '0';
    if (i < 0)
      break;
  }

  uint16_t factor = 1;
  for (c = i+1; c < READ_INT_MAX_DIGITS; c++) {
    num += digits[c] * (uint32_t)factor;  // force 32 bit operation
    factor *= 10;
  }

  if (num > 65535) num = 65535;

  delay(10); // AVR is too fast, still getting the LF of the CRLF
  while (Serial.available() > 0)
    Serial.read();

  return (uint16_t)num;
}

// ---------------------------------------------------------------------------

uint16_t readInt(uint16_t imin,uint16_t imax) {

  uint16_t input;

  do {

    Serial << " [";
    Serial.print( imin, DEC );
    Serial << "..";
    Serial.print( imax, DEC );
    Serial << "]: ";

    input = readInt();

  } while ( input < imin && input > imax );

  return input;
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
