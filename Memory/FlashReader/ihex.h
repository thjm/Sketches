
//
// File   : ihex.h
//
// Purpose: Header file for ihex.cpp
//
// $Id$
//

#ifndef _ihex_h_
 #define _ihex_h_

#include <Arduino.h>

/** Write IHEX (I8HEX) data to the specified stream. */
extern void writeIhexData(Stream& stream,const uint8_t *data,
                          const size_t data_length,const uint32_t eprom_addr);

/** Write the IHEX (I8HEX) end-of-file mark to the specified stream. */
extern void writeIhexEOF(Stream& stream);

#endif // _ihex_h_
