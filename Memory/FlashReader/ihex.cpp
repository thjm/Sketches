
//
// File:    ihex.cpp
//
// Purpose: Collection of some functions to write/read(?) Intel Hex Format
//


#include "utils.h"
#include "ihex.h"

// ---------------------------------------------------------------------------

// https://en.wikipedia.org/wiki/Intel_HEX
void writeIhexData(Stream& stream,const uint8_t *data,const size_t data_length,const uint32_t eprom_addr) {

  static const size_t eIhexBlocksize = 16;  // 32
  uint32_t addr = eprom_addr;
  size_t cur_length = data_length;
  size_t offset = 0;

  while ( cur_length > 0 ) {

    size_t len = min(cur_length, eIhexBlocksize);

    uint8_t addr_lo = (uint8_t)(addr & 0xff);
    uint8_t addr_hi = (uint8_t)((addr & 0xff00) >> 8);

    byte checksum = 0;

    checksum += len;
    checksum += addr_hi;
    checksum += addr_lo;

    stream.print(":");
    printHex8(stream, eIhexBlocksize);
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
