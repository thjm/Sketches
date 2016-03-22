
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
#include "YaesuCAT.h"

YaesuCAT::YaesuCAT(Stream& stream)
 : myStream(stream), rxMsgLength(0), rxBytesExpected(0),
   myMode(ILLEGAL_MODE), myFrequency(ILLEGAL_FREQ) {
  // ..
}

uint32_t YaesuCAT::parseFrequency(const byte* message) {

  static byte ks[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
  uint32_t f = 0;

  for ( int i=0; i<sizeof(ks); ++i ) {
     byte k = ks[i];
     f = 10*f + CATutil::getNibble(message,k);
  }

  return f*10;
}

bool YaesuCAT::read() {
  
  byte ch = myStream.read();

#ifdef DEBUG
  //Serial << F("Rx[") << rxMsgLength << ("]: "); printHex(ch); Serial.println();
#endif // DEBUG

  if ( rxBytesExpected ) {
    
    rxMessage[rxMsgLength++] = ch;

    // complete message received?
    if ( rxMsgLength == rxBytesExpected ) {

#ifdef DEBUG
      Serial << F("Rx(") << rxMsgLength << F("): "); CATutil::print(rxMessage, rxBytesExpected);
#endif // DEBUG

      switch ( lastCommand ) {
        case eREAD_FREQ_MODE:
          myMode = rxMessage[4];
          myFrequency = parseFrequency(rxMessage);
          break;

        default:
          Serial << F("Message ");
          CATutil::print(lastCommand);
          Serial << F(" ignored!") << endl;
      }
      
      rxBytesExpected = 0;
      rxMsgLength = 0;
    }
    if ( rxMsgLength == MAXLEN ) {
      Serial << F("RX message buffer overflow!") << endl;
      rxMsgLength = 0;
    }
  }

  return false;
}

/**
 * - message to Yaesu:
 *  +------+------+------+------+------+
 *  | 0x00 | 0x00 | 0x00 | 0x00 | 0x03 |
 *  +------+------+------+------+------+
 *  
 * - message from Yaesu (123.45678 MHz):
 *  +------+------+------+------+------+
 *  | 0x12 | 0x34 | 0x56 | 0x78 | mode |
 *  +------+------+------+------+------+
 */
bool YaesuCAT::requestFrequencyAndMode() {

  byte txMsg[5];
  
  rxBytesExpected = 5; // 5 byte reply expected
  lastCommand = YaesuCAT::eREAD_FREQ_MODE;
  
  txMsg[0] = 0x00;
  txMsg[1] = 0x00;
  txMsg[2] = 0x00;
  txMsg[3] = 0x00;
  txMsg[4] = lastCommand;  
  //txData[txDataLen++] = YaesuCAT::eREAD_RX_STATUS;  // 1 byte reply
  //txData[txDataLen++] = YaesuCAT::eREAD_TX_STATUS;  // 1 byte reply

#ifdef DEBUG
  Serial << F("Tx(5): ");
  CATutil::print(txMsg, 5);
#endif // DEBUG

  return sendMessage(txMsg, 5);
}

bool YaesuCAT::sendMessage(const byte* msg,size_t msgLen) {
  
  if (!msgLen ) return false;

#ifdef DEBUG
  Serial << F("Tx(") << msgLen << F("): ");
  CATutil::print(msg, msgLen);
#endif // DEBUG
  
  for (int i=0; i<msgLen; ++i) {

    // SoftwareSerial will not receive during send
    //if ( myStream.available() ) read();
    
    // should return 1 on success (i.e. 1 byte written)
    if ( !myStream.write(msg[i]) ) return false;
  }
  
  return true;
}

/**
 * - message to Yaesu (123.45678 MHz):
 *  +------+------+------+------+------+
 *  | 0x12 | 0x34 | 0x56 | 0x78 | 0x01 |
 *  +------+------+------+------+------+
 *  
 * - message from Yaesu:
 *   NONE
 */
bool YaesuCAT::writeFrequency(uint32_t frequency) {

  byte message[5];
  
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

  myFrequency = ILLEGAL_FREQ;

  return sendMessage(message, 5);
}

/**
 * - message to Yaesu (123.45678 MHz):
 *  +------+------+------+------+------+
 *  | mode | 0x00 | 0x00 | 0x00 | 0x07 |
 *  +------+------+------+------+------+
 *  
 * - message from Yaesu:
 *   NONE
 */
bool YaesuCAT::writeMode(byte mode) {

  byte message[5];

  message[0] = mode;
  message[1] = 0x00;
  message[2] = 0x00;
  message[3] = 0x00;

  message[4] = eSET_MODE;

  myMode = ILLEGAL_MODE;

  return sendMessage(message, 5);
}

