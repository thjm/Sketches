//
// File:    ihex.cpp
//
// Purpose: Collection of some functions to write/read(?) Intel Hex Format
//


#include "utils.h"
#include "ihex.h"

// ---------------------------------------------------------------------------

// from https://www.pjrc.com/tech/8051/ihex.c
int parseIhexString(const char* theline,uint8_t bytes[],uint16_t& addr,size_t& num,int& code) {

  int len;
  uint8_t sum = 0, chksum;
  const char *ptr;

  num = 0;
  addr = 0;

  // check basic format and length requirements
  if ( theline[0] != ':' ) return 0;
  if ( strlen(theline) < 11 ) return 0;

  ptr = theline+1;
  if ( !sscanf(ptr, "%02x", &len) ) return 0;

  // length of data and load address
  ptr += 2;
  if ( strlen(theline) < (11 + (len * 2)) ) return 0;
  if (!sscanf(ptr, "%04x", &addr)) return 0;

  ptr += 4;
  if ( !sscanf(ptr, "%02x", &code) ) return 0;

  ptr += 2;
  sum = (len & 0xff) + ((addr >> 8) & 0xff) + (addr & 0xff) + (code & 0xff);
  while ( num != len ) {

    if (!sscanf(ptr, "%02hhx", &bytes[num])) return 0;
    ptr += 2;
    sum += bytes[num];
    num++;
    if ( num >= 256 ) return 0;
  }

  if ( !sscanf(ptr, "%02hhx", &chksum) ) return 0;
  sum += chksum;


  if ( sum & 0xff ) return 0; /* checksum error */

  return 1; // OK
}

// ---------------------------------------------------------------------------

// https://en.wikipedia.org/wiki/Intel_HEX
void writeIhexData(Stream& stream,const uint8_t *data,const size_t data_length,const uint32_t eprom_addr) {

  // I prefer the shorter record format
  static const size_t eIhexBlocksize = 16;  // 32
  uint32_t addr = eprom_addr;
  size_t cur_length = data_length;
  size_t offset = 0;

  while ( cur_length > 0 ) {

    // number of bytes in one IHEX record
    size_t len = min(cur_length, eIhexBlocksize);

    uint8_t addr_lo = (uint8_t)(addr & 0xff);
    uint8_t addr_hi = (uint8_t)((addr & 0xff00) >> 8);

    byte checksum = 0;

    checksum += len;
    checksum += addr_hi;
    checksum += addr_lo;

    stream.print(":");
    printHex8(stream, len);  // current block length
    printHex8(stream, addr_hi);
    printHex8(stream, addr_lo);
    stream.print("00");   // block type 'data'

    for ( size_t i=0; i<len; ++i ) {
      printHex8(stream, data[offset + i]);
      checksum += data[offset + i];
    }

    // create two's complement
    checksum = ~checksum + 1;

    printHex8(stream, checksum);
    stream.println("");

    cur_length -= len;
    addr += len;
    offset += len;
  }
}

// ---------------------------------------------------------------------------

void writeIhexEOF(Stream& stream) {

  stream.println(":000001FF");  // block type 'EOF'
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
