//
// File   : CATutil.h
//
// Purpose: Header file for the class CATutil and other useful (CAT) stuff.
//
// $Id$
//


#ifndef __CAT_UTIL_H_
 #define __CAT_UTIL_H_

#include <stdint.h>

/** Print an integer in hex format, width can be specified, starts always with '0x'. */
extern void printHex(unsigned int,unsigned int width=2);
/** Print a complete char array in hex format, one line. */
extern void print(const byte* message,size_t msgLength);

/** Create BCD 'byte' from two characters (digits), no error checking done! */
#define bcd(_h,_l) ((int)_l - '0' + 16 * ((int)_h - '0'))

#endif // __CAT_UTIL_H_
