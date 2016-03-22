//
// File   : CATutil.h
//
// Purpose: Header file for the class CATutil and other useful (CAT) stuff.
//
// $Id$
//


#ifndef __CAT_UTIL_H_
 #define __CAT_UTIL_H_

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include <stdint.h>

/** Create BCD 'byte' from two characters (digits), no error checking done! */
#define bcd(_h,_l) ((int)_l - '0' + 16 * ((int)_h - '0'))

/** COntainer with useful functions/methods. */
class CATutil {
public:
  /** Get specified nibble from byte string. */
  static int getNibble(const byte* s,int i);

  /** Print an integer in hex format, width can be specified, starts always with '0x'. */
  static void print(unsigned int,unsigned int width=2);
  /** Print a complete char array in hex format, one line. */
  static void print(const byte* message,size_t msgLength);
};
#endif // __CAT_UTIL_H_
