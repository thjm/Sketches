
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

/** Class Morse, a class to output characters as Morse code on a
  * preconfigured pin of the Arduino.
  */
class Morse {
public:

    Morse();
    
    void enableTransmit(int nTransmitterPin);
    void disableTransmit();
    
    // set speed in wpm
    void setSpeed(unsigned int speed);
    
    void write(char val);
    void write(const char str[]);

protected:
    
    void dit();
    void dah();
    
    int nTransmitterPin;
    // length of dot in ms
    unsigned int dotLength;
};

#endif // _Morse_h_
