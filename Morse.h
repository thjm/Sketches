
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

#ifndef _Morse_h_
 #define _Morse_h_

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
 // LaunchPad, FraunchPad and StellarPad specific
#elif defined(ENERGIA)
    #include "Energia.h"	
#else
    #include "WProgram.h"
#endif

#include <stdint.h>
#include <Stream.h>

/** Class Morse, a class to output characters as Morse code on a
  * preconfigured pin of the Arduino.
  */
class Morse : public Stream {
public:

    Morse();
    
    void enableTransmit(int nTransmitterPin);
    void disableTransmit();
    
    // set speed in wpm
    void setSpeed(unsigned int speed);
    
    // to be implemented from class Print
    size_t write(uint8_t val);
    //size_t write(const char str[]);

protected:
    
    void dit();
    void dah();
    
    // to be implemented from class Stream
    int available() { return 0; }
    int read() { return 0; }
    int peek() { return 0; }
    void flush() { }
    
    int nTransmitterPin;
    // length of dot in ms
    unsigned int dotLength;
};

#endif // _Morse_h_
