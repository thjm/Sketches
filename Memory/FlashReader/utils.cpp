//
// File:    utils.cpp
//
// Purpose: Collection of some useful functions
//


// http://arduiniana.org/libraries/streaming/
#include <Streaming.h>

#include "utils.h"

// ---------------------------------------------------------------------------

void printHex8(Stream& stream,uint8_t data) {

  if ( data < 0x10 )
    stream.print("0");
  stream.print(data, HEX);
}

// ---------------------------------------------------------------------------

void printHex16(Stream& stream,uint16_t data) {

  printHex8(stream, (uint8_t)((data & 0xff00) >> 8));
  printHex8(stream, (uint8_t)(data & 0x00ff));
}

// ---------------------------------------------------------------------------

void printHex24(Stream& stream,uint32_t data) {

  printHex8(stream, (uint8_t)((data & 0x00ff0000) >> 16));
  printHex16(stream, (uint16_t)(data & 0x0000ffff));
}

// ---------------------------------------------------------------------------

void printHex32(Stream& stream,uint32_t data) {

  printHex16(stream, (uint16_t)((data & 0xffff0000) >> 16));
  printHex16(stream, (uint16_t)(data & 0x0000ffff));
}

// ---------------------------------------------------------------------------

void dumpHex(uint8_t *data,size_t length,uint32_t addr) {

  for ( size_t j=0; j<length/16+1; ++j ) {

    if ( j*16 >= length ) break;

    Serial.print(F("0x"));
    //printHex(addr+j*16, 4);
    if ( (addr+j*16) > 0xffff )
      printHex24(Serial, addr+j*16);
    else
      printHex16(Serial, (uint16_t)(addr+j*16));
    Serial.print(":");

    for (size_t i=0; i<16; ++i ) {  // output binary data, 1-byte format
      if ( i == 8 )
        Serial.print( " -");
      if ( j*16+i >= length )
        Serial.print(F("     "));
      else {
        Serial.print(F(" 0x"));
        printHex8(Serial, data[j*16+i]);
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

uint32_t readInt32(void) {

  uint32_t num = 0;
  uint32_t c;
  byte digits[10];
  int8_t i = sizeof(digits) - 1;

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

  uint32_t factor = 1;
  for (c = i+1; c <10; c++) {
    num += digits[c] * (uint32_t)factor;  // force 32 bit operation
    factor *= 10;
  }

  delay(10); // AVR is too fast, still getting the LF of the CRLF
  while (Serial.available() > 0)
    Serial.read();

  return num;
}

// ---------------------------------------------------------------------------

uint16_t readInt(uint16_t imin,uint16_t imax) {

  uint16_t input;

  do {

    Serial << " [";
    Serial.print(imin, DEC);
    Serial << "..";
    Serial.print(imax, DEC);
    Serial << "]: ";

    input = readInt();

  } while ( input < imin && input > imax );

  return input;
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
