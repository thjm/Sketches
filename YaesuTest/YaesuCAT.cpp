
//
// File   : YaesuCat.cpp
//
// Purpose: Implementation of the class YaesuCat
//
// $Id$
//

#include <Stream.h>
#include <Streaming.h>

#include "CATutil.h"
#include "YaesuCat.h"

YaesuCAT::YaesuCAT(Stream& stream)
 : myStream(stream), 
   myMode(ILLEGAL_MODE), myFrequency(ILLEGAL_FREQ) {
  // ..
}

bool YaesuCAT::requestFrequencyAndMode() {

  byte txMsg[6];
  
  txMsg[0] = 0x00;
  txMsg[1] = 0x00;
  txMsg[2] = 0x00;
  txMsg[3] = 0x00;
  txMsg[4] = YaesuCAT::eREAD_FREQ_MODE;  // 5 byte reply
  //txData[txDataLen++] = YaesuCAT::eREAD_RX_STATUS;  // 1 byte reply
  //txData[txDataLen++] = YaesuCAT::eREAD_TX_STATUS;  // 1 byte reply

#ifdef DEBUG
  Serial << F("Tx(5): ");
  print(txMsg, 5);
    //CATutil::print(txData, txDataLen);
#endif // DEBUG

  return sendMessage(txMsg, 5);
}

bool YaesuCAT::sendMessage(const byte* msg,size_t msgLen) {
  
  if (!msgLen ) return false;

#ifdef DEBUG
  Serial << F("Tx(") << msgLen << F("): ");
  printMessage(msg, msgLen);
#endif // DEBUG
  
  for (int i=0; i<msgLen; ++i) {

    // SoftwareSerial will not receive during send
    //if ( myStream.available() ) read();
    
    // should return 1 on success (i.e. 1 byte written)
    if ( !myStream.write(msg[i]) ) return false;
  }
  
  return true;
}

bool YaesuCAT::writeFrequency(uint32_t frequency) {

  byte message[6];
  
    // convert the integer frequency into a string of 10 bytes length
  char freq_str[12];
  ultoa(frequency, freq_str, 10);

  // ... with leading zeros
  while ( strlen(freq_str) < 10 ) {
    freq_str[strlen(freq_str)+1] = 0;
    for ( int i=strlen(freq_str); i>0; --i)
      freq_str[i] = freq_str[i-1];
    freq_str[0] = '0';
  }

#ifdef DEBUG
  //Serial << "f= " << freq_str << endl;
  //Serial << "len(f)= " << strlen(freq_str) << endl;
#endif // DEBUG

  message[0] = bcd(freq_str[1], freq_str[2]);
  message[1] = bcd(freq_str[3], freq_str[4]);
  message[2] = bcd(freq_str[5], freq_str[6]);
  message[3] = bcd(freq_str[7], freq_str[8]);

  message[4] = YaesuCAT::eSET_FRQUENCY;

  return sendMessage(message, 5);
}

