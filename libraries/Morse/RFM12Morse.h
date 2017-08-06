
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

#ifndef _RFM12Morse_h_
 #define _RFM12Morse_h_

#if defined(ARDUINO) && ARDUINO >= 100
 #include "Arduino.h"
 // LaunchPad, FraunchPad and StellarPad specific
#elif defined(ENERGIA)
 #include "Energia.h"
#else
 #include "WProgram.h"
#endif

#include <SPI.h>

#include "Morse.h"

/** Class RFM12Morse which uses the RFM12 transceiver board and puts
  * it in OOK mode to generate Morse signs.
  *
  * Idea of fast OOK transmission from:
  * https://openenergymonitor.org/forum-archive/node/3369.html
  *
  * This class uses some of the functionality of the base class Morse
  * especially the Morse code table and the speed calculation.
  */
class RFM12Morse : public Morse {
public:
    RFM12Morse(int rfmSelPin);
    ~RFM12Morse() { }

protected:

    /** Create a 'dit' (dot): */
    void dit();

    /** Create a 'dah' (dash). */
    void dah();

    /** Write a command to the RFM12 */
    word rfm_write(word cmd);

    int rfmSelPin;
};

#endif // _RFM12Morse_h_
