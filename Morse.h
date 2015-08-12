
#ifndef _Morse_h_
 #define _Morse_h_

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#elif defined(ENERGIA) // LaunchPad, FraunchPad and StellarPad specific
    #include "Energia.h"	
#else
    #include "WProgram.h"
#endif

class Morse {
public:

    Morse();
    
    void enableTransmit(int nTransmitterPin);
    void disableTransmit();

protected:
    
    int nTransmitterPin;
}

#endif // _Morse_h_
