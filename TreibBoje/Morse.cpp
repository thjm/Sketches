
/*
  Morse - Arduino libary to create Morse code on any output pin.
  Copyright (c) 2015 Hermann-Josef Mathes.  All right reserved.
    
  Contributors:
  - add your name and eMail address
  
  Project home: https://github.com/thjm/morse

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "Morse.h"

typedef struct _MorseCode_t {

  byte code;
  byte len;

} MorseCode_t;

// Morse code table for ASCII characters 0x20 .. 0x5f
// 1st byte : code, 0=dit, 1=dah
// 2nd byte : number of dits/dahs
// For the idea of this table, see:
// http://www.avr-asm-tutorial.net/cq-dl/teil4/Cw01_200.asm
// (C) 2002 by Gerhard Schmidt, DG4FAC
static const MorseCode_t gMorseCode[] PROGMEM = {

  // 0x20 .. 0x2F
  { 0b00000000, 0 }, // Blank
  { 0b01000000, 5 }, // ! = Warten
  { 0b01001000, 6 }, // "
  { 0b11011000, 5 }, // # = ~n
  { 0b01101000, 5 }, // $ = ?, ?a
  { 0b01000000, 5 }, // % = ?
  { 0b00000000, 0 }, // & = not used
  { 0b01111000, 6 }, // '
  { 0b10110000, 5 }, // (, KN
  { 0b10110100, 6 }, // ), KK
  { 0b00000000, 0 }, // * = not used
  { 0b01010000, 5 }, // + = Spruchende, AR
  { 0b11001100, 6 }, // ,
  { 0b10000100, 6 }, // -
  { 0b01010100, 6 }, // .
  { 0b10010000, 5 }, // /
  // 0x30 .. 0x3F
  { 0b11111000, 5 }, // 0
  { 0b01111000, 5 }, // 1
  { 0b00111000, 5 }, // 2
  { 0b00011000, 5 }, // 3
  { 0b00001000, 5 }, // 4
  { 0b00000000, 5 }, // 5
  { 0b10000000, 5 }, // 6
  { 0b11000000, 5 }, // 7
  { 0b11100000, 5 }, // 8
  { 0b11110000, 5 }, // 9
  { 0b11100000, 6 }, // :
  { 0b10101000, 6 }, // ;
  { 0b10101000, 5 }, // < = Verkehrsanfang
  { 0b10001000, 5 }, // =
  { 0b01010000, 5 }, // > = Verkehrsende
  { 0b00110000, 6 }, // ?
  // 0x40 .. 0x4F
  { 0b01101000, 6 }, // @
  { 0b01000000, 2 }, // A
  { 0b10000000, 4 }, // B
  { 0b10100000, 4 }, // C
  { 0b10000000, 3 }, // D
  { 0b00000000, 1 }, // E
  { 0b00100000, 4 }, // F
  { 0b11000000, 3 }, // G
  { 0b00000000, 4 }, // H
  { 0b00000000, 2 }, // I
  { 0b01110000, 4 }, // J
  { 0b10100000, 3 }, // K
  { 0b01000000, 4 }, // L
  { 0b11000000, 2 }, // M
  { 0b10000000, 2 }, // N
  { 0b11100000, 3 }, // O
  // 0x50 .. 0x5F
  { 0b01100000, 4 }, // P
  { 0b11010000, 4 }, // Q
  { 0b01000000, 3 }, // R
  { 0b00000000, 3 }, // S
  { 0b10000000, 1 }, // T
  { 0b00100000, 3 }, // U
  { 0b00010000, 4 }, // V
  { 0b01100000, 3 }, // W
  { 0b10010000, 4 }, // X
  { 0b10110000, 4 }, // Y
  { 0b11000000, 4 }, // Z
  { 0b01010000, 4 }, // [ = AE
  { 0b11100000, 4 }, // \ = OE
  { 0b00110000, 4 }, // ] = UE
  { 0b00000000, 8 }, // ^ = Irrung
  { 0b00110100, 6 }, // _
};

// ---------------------------------------------------------------------------

Morse::Morse()
 : nTransmitterPin(0), dotLength(1200/10) { }

// ---------------------------------------------------------------------------

void Morse::enableTransmit(int pin) {
  nTransmitterPin = pin;
  pinMode(pin, OUTPUT);
}

// ---------------------------------------------------------------------------

void Morse::disableTransmit() {
  pinMode(nTransmitterPin, INPUT);
  nTransmitterPin = 0;
}

// ---------------------------------------------------------------------------

void Morse::setSpeed(unsigned int speed) {
  dotLength = 1200 / speed;
}

// ---------------------------------------------------------------------------

size_t Morse::write(uint8_t val) {

  val = toupper(val);
  if ( val < 0x20 || val > 0x5f ) return 0;
  
  MorseCode_t morse;
  byte *ptr = (byte *)&morse;
  for ( int i=0; i<sizeof(MorseCode_t); ++i )
    *ptr++ = pgm_read_byte_near((byte *)&gMorseCode[val - 0x20] + i);
  
#if 0
  // debug output (to check pgm_read_byte_near() operation...)
  Serial.print(val);
  Serial.print(" ");
  Serial.print((int)val, HEX);
  Serial.print(": ");
  Serial.print((int)morse.code, BIN);
  Serial.print(" ");
  Serial.print(morse.len);
  Serial.println();
#endif

  byte mask = 0x80;
  for ( int i=0; i<morse.len; ++i ) {
    if ( morse.code & mask )
      dah();  // 3 dots and 1 dot pause
    else
      dit();  // 1 dot and 1 dot pause
    mask >>= 1;
  }

  // gap between letters is 3 dots
  if ( morse.len ) delay(2*dotLength);

  // gap between words is 7 dots
  if ( val == 0x20 ) delay(6*dotLength);
  
  return 1;
}
    
// ---------------------------------------------------------------------------

void Morse::dit() {
  digitalWrite(nTransmitterPin, HIGH);
  delay(dotLength);
  digitalWrite(nTransmitterPin, LOW);
  delay(dotLength);
}

// ---------------------------------------------------------------------------

void Morse::dah() {
  digitalWrite(nTransmitterPin, HIGH);
  delay(3*dotLength);
  digitalWrite(nTransmitterPin, LOW);
  delay(dotLength);
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
