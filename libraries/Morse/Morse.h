
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
//#include <Stream.h>
#include <Print.h>

/** Class Morse, a class to output characters as Morse code on a
  * preconfigured pin of the Arduino.
  *
  * The class inherits from the Arduino base class Print, thus together with
  * Streaming.h the streaming operator<()< and many overloaded print() methods 
  * can be used.
  */
class Morse : public Print {
public:

    /**  */
    typedef struct _MorseCode_t {

      byte code;
      byte len;

    } MorseCode_t;

    Morse();
    
    /**  */
    void enableTransmit(int nTransmitterPin);
    /**  */
    void disableTransmit();
    
    /** set speed in wpm */
    virtual void setSpeed(unsigned int speed);
    
    /** to be implemented from class Print */
    size_t write(uint8_t val);

protected:
    
    /** create a 'dit' (dot - short beep) */
    virtual void dit();
    /** create a 'dah' (dash - long beep) */
    virtual void dah();

    /** Get character specific info about Morse code. */
    static MorseCode_t getMorseCode(uint8_t val);

    int nTransmitterPin;
    // length of a dot in ms
    unsigned int dotLength;
};

#endif // _Morse_h_
