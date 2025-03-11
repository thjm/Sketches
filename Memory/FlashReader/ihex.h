//
// File   : ihex.h
//
// Purpose: Header file for ihex.cpp
//


#ifndef _ihex_h_
 #define _ihex_h_

#include <Arduino.h>

/** Parse a line (string) containing IHEX formatted data, i.e. address, length and data bytes and
 *  return this information to the calling function.
 */
extern int parseIhexString(const char* theline,uint8_t bytes[],uint16_t& addr,size_t& num,int& code);

/** Write IHEX (I8HEX) data to the specified stream. */
extern void writeIhexData(Stream& stream,const uint8_t *data,
                          const size_t data_length,const uint32_t eprom_addr);

/** Write the IHEX (I8HEX) end-of-file mark to the specified stream. */
extern void writeIhexEOF(Stream& stream);

#endif // _ihex_h_
