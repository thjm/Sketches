
//
// File   : IcomCAT.cpp
//
// Purpose: Implementation of the class IcomCAT
//
// $Id$
//


#include <Stream.h>
#include <Streaming.h>

#include <CATutil.h>
#include <IcomCAT.h>

IcomCAT::IcomCAT(Stream& stream,byte address)
 : myStream(stream), rigAddress(address), 
   rxMsgLength(0), rxMsgComplete(false),
   myMode(ILLEGAL_MODE), myFrequency(ILLEGAL_FREQ)
 {

}

static int getNibble(const byte* s,int i) {

  byte k = s[i/2];
  if ( i % 2 == 0 )
    k = k >> 4;
  return k & 0x0f;
}

uint32_t IcomCAT::parseFrequency(const byte* message) {

  static byte ks[] = { 10, 11, 8, 9, 6, 7, 4, 5, 2, 3 };
  uint32_t f = 0;

  for (size_t i=0; i<sizeof(ks); ++i) {
     byte k = ks[i];
     f = 10*f + getNibble(message,k);
  }

  return f;
}

bool IcomCAT::parseMessage() {

  if ( !rxMsgComplete ) return false;

#ifdef DEBUG
  Serial << F("Rx(") << rxMsgLength << F("): "); 
  CATutil::print(rxMessage,rxMsgLength);
#endif // DEBUG

  // reset 'complete' flag
  rxMsgComplete = false;
  
  bool err = false;
  
  if (rxMessage[0] != IcomCAT::INTRO) err = true;
  if (rxMessage[1] != IcomCAT::INTRO) err = true;
  if (rxMessage[rxMsgLength-1] != IcomCAT::EOM) err = true;
  // message to the rig?
  if (rxMessage[2] == rigAddress) {
    // erase it
    rxMsgLength = 0;
    Serial << F("Skipped ECHO!") << endl;
    return false;
  }
  // message from the rig?
  if (rxMessage[3] != rigAddress) err = true;

  if ( err ) {
    Serial << F("CAT message error!") << endl;
    rxMsgLength = 0;
    
    return false;
  }

  bool status = true;
  
  switch ( rxMessage[4] ) {

    case eTRANSFER_FREQ: // transfer operating frequency data
    case eGET_OPERATING_FREQ:
      myFrequency = parseFrequency(&rxMessage[4]);
      break;

    case eTRANSFER_MODE: // transfer operating mode data
    case eGET_OPERATING_MODE:
      // rxMessage[6] is the passband/filter data
      // 0x01 = wide, 0x02 = normal, 0x03 = narrow
      myMode = rxMessage[5];
      break;
    
    default:
      Serial << F("Message ");
      CATutil::print(rxMessage[4]);
      Serial << F(" ignored!") << endl;
      status = false;
      break;
  }
  
  rxMsgLength = 0;
  
  return status;
}

bool IcomCAT::read() {

  static int nIntro = 0;
  
  byte ch = myStream.read();

#ifdef DEBUG
  //Serial << F("Rx: "); CATutil::print(ch); Serial.println();
#endif // DEBUG

  if ( ch == IcomCAT::INTRO ) {
    nIntro++;
    rxMessage[rxMsgLength++] = ch;
  }
  if ( ch != IcomCAT::INTRO && nIntro == 2 ) {
    rxMessage[rxMsgLength++] = ch;
  }
  if ( ch == IcomCAT::EOM ) {
    nIntro = 0;
    rxMsgComplete = true;
  }
  if ( rxMsgLength > IcomCAT::MAXLEN ) {
    Serial << F("MSG length error!") << endl;
    rxMsgComplete = false;
    rxMsgLength = 0;  // clear message buffer
    nIntro = 0;
  }

  return rxMsgComplete;
}

/**
 * - message to ICom:
 *  +------+------+----+----+----+------+
 *  | 0xFE | 0xFE | 58 | E0 | 03 | 0xFD |
 *  +------+------+----+----+----+------+
 *  
 *  - message from ICom:
 *  +------+------+----+----+----+-------+-------+-------+-------+-------+------+
 *  | 0xFE | 0xFE | 00 | 58 | 00 | Data0 | Data1 | Data2 | Data3 | Data4 | 0xFD |
 *  +------+------+----+----+----+-------+-------+-------+-------+-------+------+
 */
bool IcomCAT::requestFrequency() {

  int msgLen = 0;
  byte message[8];

  message[msgLen++] = IcomCAT::INTRO;
  message[msgLen++] = IcomCAT::INTRO;
  message[msgLen++] = rigAddress;
  message[msgLen++] = MY_ADDRESS;
  message[msgLen++] = IcomCAT::eGET_OPERATING_FREQ;
  message[msgLen++] = IcomCAT::EOM;

  return sendMessage(message, msgLen);
}

/**
 * - message to ICom:
 *  +------+------+----+----+----+------+
 *  | 0xFE | 0xFE | 58 | E0 | 03 | 0xFD |
 *  +------+------+----+----+----+------+
 *  
 *  - message from ICom:
 *  +------+------+----+----+----+------+------+
 *  | 0xFE | 0xFE | 00 | 58 | 01 | mode | 0xFD |
 *  +------+------+----+----+----+------+------+
 */
bool IcomCAT::requestMode() {

  int msgLen = 0;
  byte message[8];

  message[msgLen++] = IcomCAT::INTRO;
  message[msgLen++] = IcomCAT::INTRO;
  message[msgLen++] = rigAddress;
  message[msgLen++] = MY_ADDRESS;
  message[msgLen++] = IcomCAT::eGET_OPERATING_MODE;
  message[msgLen++] = IcomCAT::EOM;

  return sendMessage(message, msgLen);
}

bool IcomCAT::sendMessage(const byte* msg,size_t msgLen) {
  
  if (!msgLen ) return false;

#ifdef DEBUG
  Serial << F("Tx(") << msgLen << F("): ");
  CATutil::print(msg, msgLen);
#endif // DEBUG
  
  for (size_t i=0; i<msgLen; ++i) {

    // SoftwareSerial will not receive during send
    if ( myStream.available() ) read();
    
    // should return 1 on success (i.e. 1 byte written)
    if ( !myStream.write(msg[i]) ) return false;
  }
  
  return true;
}

/** Create BCD 'byte' from two characters (digits), no error checking done! */
#define bcd(_h,_l) ((int)_l - '0' + 16 * ((int)_h - '0'))

/**
 *  - message to ICom:
 *  +------+------+----+----+----+-------+-------+-------+-------+-------+------+
 *  | 0xFE | 0xFE | 58 | E0 | 05 | Data0 | Data1 | Data2 | Data3 | Data4 | 0xFD |
 *  +------+------+----+----+----+-------+-------+-------+-------+-------+------+
 *  
 *  - message from ICom:
 *  +------+------+----+----+------+------+
 *  | 0xFE | 0xFE | E0 | 58 | 0xFB | 0xFD |
 *  +------+------+----+----+------+------+
 */
bool IcomCAT::writeFrequency(uint32_t frequency) {

  int msgLen = 0;
  byte message[12];

  message[msgLen++] = IcomCAT::INTRO;
  message[msgLen++] = IcomCAT::INTRO;
  message[msgLen++] = rigAddress;
  message[msgLen++] = MY_ADDRESS;
  message[msgLen++] = IcomCAT::eSET_OPERATING_FREQ;

  byte *ptr = &message[msgLen];
  
  for (int i=0; i<5; ++i )
    message[msgLen++] = 0;

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

  *ptr++ = bcd(freq_str[8], freq_str[9]);
  *ptr++ = bcd(freq_str[6], freq_str[7]);
  *ptr++ = bcd(freq_str[4], freq_str[5]);
  *ptr++ = bcd(freq_str[2], freq_str[3]);
  *ptr++ = bcd(freq_str[0], freq_str[1]);
  
  message[msgLen++] = IcomCAT::EOM;

#ifdef DEBUG
  //Serial << F("Tx: ");
  //CATutil::print(message, msgLen);
#endif // DEBUG

  myFrequency = IcomCAT::ILLEGAL_FREQ;
  
  return sendMessage(message, msgLen);
}

/**
 *  - message to ICom:
 *  +------+------+----+----+----+------+------+
 *  | 0xFE | 0xFE | 58 | E0 | 06 | mode | 0xFD |
 *  +------+------+----+----+----+------+------+
 *  
 *  - message from ICom:
 *  +------+------+----+----+------+------+
 *  | 0xFE | 0xFE | E0 | 58 | 0xFB | 0xFD |
 *  +------+------+----+----+------+------+
 */
bool IcomCAT::writeMode(byte mode) {

  int msgLen = 0;
  byte message[8];

  message[msgLen++] = IcomCAT::INTRO;
  message[msgLen++] = IcomCAT::INTRO;
  message[msgLen++] = rigAddress;
  message[msgLen++] = MY_ADDRESS;
  message[msgLen++] = IcomCAT::eSET_OPERATING_MODE;
  message[msgLen++] = mode;
  message[msgLen++] = IcomCAT::EOM;

  myMode = IcomCAT::ILLEGAL_MODE;
  
  return sendMessage(message, msgLen);
}

