
//
// File   : IcomControl.cpp
//
// Purpose: Implementation of the class IcomControl
//

#include <IcomCAT.h>
#include "IcomControl.h"

IcomControl::IcomControl(Stream& serial,byte catAddress)
: mySerial(serial), icom(serial, catAddress), enable(false) {
}

bool IcomControl::isFrequencyOK() {

  uint32_t frequency = icom.getFrequency();

  //if ( frequency == IcomCAT::ILLEGAL_FREQ ) return false;
  if ( frequency < 70000000 ) return false;
  if ( frequency >= 70200000 ) return false;

  return true;
}

bool IcomControl::isModeOK() {

  byte mode = icom.getMode();
  
  //if ( mode == IcomCAT::ILLEGAL_MODE ) return false;
  if ( mode == IcomCAT::eModeUSB ) return true;
  if ( mode == IcomCAT::eModeCW ) return true;
  
  return false;
}

void IcomControl::requestFrequencyAndMode(uint32_t& frequency,byte& mode) {

  static unsigned long r_time = millis();
  
  if ( frequency == IcomCAT::ILLEGAL_FREQ 
       && (millis() - r_time) > 100 ) {
    
    icom.requestFrequency();

    r_time = millis();
  }  

  if ( mode == IcomCAT::ILLEGAL_MODE  
       && (millis() - r_time) > 100 ) {
       
    icom.requestMode();
    
    r_time = millis();
  }
}

void IcomControl::setFrequency(uint32_t& frequency,byte& mode) {

  static unsigned long s_time = millis();

  if ( !isFrequencyOK() && (millis() - s_time) > 200 ) {

    icom.writeFrequency(70175000);
    
    s_time = millis();
  }
}

void IcomControl::setMode(uint32_t& frequency,byte& mode) {
  
  static unsigned long s_time = millis();

  if ( !isModeOK() && (millis() - s_time) > 200 ) {

    icom.writeMode(IcomCAT::eModeUSB);
    
    s_time = millis();
  }
}

void IcomControl::listen() {

#if defined (__AVR__)
  mySerial.listen();
#endif // (__AVR__)

  // check if 'Serial#' has data to receive
  if (mySerial.available()) {

    icom.read();
  
    if (icom.msgIsComplete()) {
      icom.parseMessage();
    }
  } // mySerial.available()
}

bool IcomControl::run(uint32_t& frequency,byte& mode) {
  
  static unsigned long init = millis();
  static byte state = 0;

  mode = icom.getMode();
  frequency = icom.getFrequency();

  // set the operation_mode appropriately
  if ( frequency == IcomCAT::ILLEGAL_FREQ || mode == IcomCAT::ILLEGAL_MODE 
       /* || !enable */ )
    state = 0;
  else if ( !isFrequencyOK() )
    state = 1;
  else if ( !isModeOK() )
    state = 2;
  else
    state = 3;
  
  switch ( state ) {
    
    case 0: 
      requestFrequencyAndMode(frequency, mode);
      break;

    case 1: 
      setFrequency(frequency, mode);  // set right frequency
      break;

    case 2: 
      setMode(frequency, mode);  // set right mode
      break;

    case 3:
      if ( (millis() - init) > 500 ) {
        requestFrequencyAndMode(frequency, mode);
        init = millis();
      }
      break;
      
    default:
      ;
  }

  listen();

  return (state == 3);
}

