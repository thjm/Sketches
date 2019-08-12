
//
// File   : IcomControl.h
//
// Purpose: Declaration of the class IcomControl
//

#ifndef _IcomControl_h_
 #define _IcomControl_h_

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include <IcomCAT.h>

/** Class which does the frequency and mode control for the Icom transceiver
  * doing the RX part.
  */
class IcomControl {

public:
    /** Contructor for the class IcomControl. */
    IcomControl(Stream& stream,byte catAddress);

    /** Get frequency from ... */
    uint32_t getFrequency()
     { return icom.getFrequency(); }
    /** Get mode from ... */
    byte getMode()
     { return icom.getMode(); }

    /** Return status of 'enable' field. */
    bool isEnabled()
     { return enable; }
    
    /** Return true if the frequency is in the desired range. */
    bool isFrequencyOK();

    /** Return true if the mode is either USB or CW. */
    bool isModeOK();

    /**  */
    bool run(uint32_t& frequency,byte& mode);

    /** Enable/disable the control loop. */
    void setEnable(bool enable=true)
     { enable = enable; }

protected:
    /** This is active if either mode or frequency haven't been read from the rig. */
    void requestFrequencyAndMode(uint32_t& frequency,byte& mode);

    /** Called when the frequency is not in the right range. */
    void setFrequency(uint32_t& frequency,byte& mode);

    /** Called when the mode is not correct. */
    void setMode(uint32_t& frequency,byte& mode);

    /** Check for RX messages and process them. */
    void listen();

    /** Serial stream. */
    Stream& mySerial;
    /** CAT class for the Icom transceiver. */
    IcomCAT icom;
    /** 'enable' status for the control loop. */
    bool enable;
};

#endif // _IcomControl_h_
