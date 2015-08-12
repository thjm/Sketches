
/*
  Morse - Arduino libary to create Morse code on any output pin.
  Copyright (c) 2015 Hermann-Josef Mathes.  All right reserved.
    
  Contributors:
  - add your name and eMail address
  
  Project home: https://github.com/thjm/morse

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "Morse.h"

typedef struct _MorseCode_t {

  byte character;
  byte code;

} MorseCode_t;

const MorseCode_t gMorseCodes[] PROGMEM = {

  ('a', 0b00000001),
  ('b', 0b00001000),
};
