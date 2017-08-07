
/*
  Morse - Arduino libary to create Morse code on any output pin.
  Copyright (c) 2015 - 2017 Hermann-Josef Mathes.  All right reserved.

  Contributors:
  - add your name and eMail address

  Project home: https://github.com/thjm/Sketches

  Download: https://github.com/thjm/Sketches/libraries/Morse

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

#include "RFM12Morse.h"

// ---------------------------------------------------------------------------

RFM12Morse::RFM12Morse(int pin) : rfmSelPin(pin) {

  pinMode(rfmSelPin, OUTPUT);
  digitalWrite(rfmSelPin,HIGH);

  // start the SPI library:
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(0);
  SPI.setClockDivider(SPI_CLOCK_DIV2);

  // initialise RFM12
  delay(200); // wait for RFM12 POR
  rfm_write(0x0000); // clear SPI
  rfm_write(0xCA83); // non-sensitive reset
  rfm_write(0x80D7); // 433MHz band
  rfm_write(0xA640); // set frequency to 434.0000MHz (adjust to suit device)
  rfm_write(0x9850); // max power
  rfm_write(0x821d); // Turn on crystal and synthesizer
}

// ---------------------------------------------------------------------------

// original code by martinR:
// void ookPulse(int on, int off) {
//   rfm_write(0x823d);
//   delayMicroseconds(on+3);
//   rfm_write(0x821d);
//   delayMicroseconds(off-43);
// }
//
// Please note that delayMicroseconds() is limited to value up to 16383, i.e.
// the behavior for argument values larger than 16383 is unpredicted, see also:
//  https://www.arduino.cc/en/Reference/DelayMicroseconds
//
// delayMicroseconds() replaced for this application by delay() function,
// the achievable accuracy seems to be sufficient.
//
void RFM12Morse::dit() {

  rfm_write(0x823d); // on
  delay(Morse::dotLength*1);
  rfm_write(0x821d); // off
  delay(Morse::dotLength*1);
}

// ---------------------------------------------------------------------------

void RFM12Morse::dah() {

  rfm_write(0x823d); // on
  delay(Morse::dotLength*3);
  rfm_write(0x821d); // off
  delay(Morse::dotLength*1);
}

// ---------------------------------------------------------------------------

word RFM12Morse::rfm_write(word cmd) {

  digitalWrite(rfmSelPin,LOW);
  word result = (SPI.transfer(cmd>>8)<<8) | SPI.transfer(cmd & 0xff);
  digitalWrite(rfmSelPin,HIGH);

  return result;
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
