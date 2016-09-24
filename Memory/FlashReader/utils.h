
//
// File   : utils.h
//
// Purpose: Header file for utils.cpp
//
// $Id$
//


#ifndef _utils_h_
 #define _utils_h_

#include <Arduino.h>

extern void printHex8(Stream& stream,uint8_t data);
extern void printHex16(Stream& stream,uint16_t data);
extern void printHex32(Stream& stream,uint32_t data);

/** Function to create hex dump from data, organized as 8 bit words. */
extern void dumpHex(uint8_t *data,size_t length,uint32_t addr);

/** Print a number in hexadecimal format, the width of the output with leading zeros can be specified
  * (sprintf() based implementation). 
  */
void printHex(uint32_t number,size_t len=2);

#define READ_INT_MAX_DIGITS 5

/** Read up to READ_INT_MAX_DIGITS digits until CR or non-digit is entered.
  *
  * Source: https://devel-ik.fzk.de/wsvn/fd-online/Diverse/Oktokopter/Arduino2Flasher/trunk/Arduino2Flasher.ino
  */
extern uint16_t readInt(void);

/** Read integer within specified limits. */
extern uint16_t readInt(uint16_t imin,uint16_t imax);

#endif // _utils_h_
