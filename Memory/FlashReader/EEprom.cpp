// File   : EEprom.cpp
//
// Purpose: Implementation of the class EEprom
//
// $Id$
//

#include <Arduino.h>

#include "EEprom.h"

// ---------------------------------------------------------------------------------

EEprom::EEprom() {

  _eepromSize = 0;
  _eepromType = eEEPROM_NONE;
  
  // !CE to high, output
  CE_PORT |= CE_MASK;
  CE_DDR |= CE_MASK;
  
  // !OE to high, output
  OE_PORT |= OE_MASK;
  OE_DDR |= OE_MASK;

  // !WE to high, output
  WE_PORT |= WE_MASK;
  WE_DDR |= WE_MASK;

  // strobe signals STR1 .. STR3 to low, output, HCT574 has positive strobe sigs
  STR1_PORT &= ~STR1_MASK;
  STR1_DDR |= STR1_MASK;
  STR2_PORT &= ~STR2_MASK;
  STR2_DDR |= STR2_MASK;
  STR3_PORT &= ~STR3_MASK;
  STR3_DDR |= STR3_MASK;
}

// ---------------------------------------------------------------------------------

String EEprom::getTypeString(eEEPROMtype eType) {
  
  switch ( eType ) {

    case eEEPROM_NONE:
        return String("None");
    
    case eEEPROM_2716:
        return String("2716");
      
    case eEEPROM_2732:
        return String("2732");

    case eEEPROM_2764:
        return String("2764");

    case eEEPROM_27128:
        return String("27128");

    case eEEPROM_27256:
        return String("27256");

    case eEEPROM_27512:
        return String("27512");

    default:
      return String("unknown");
  }
}

// ---------------------------------------------------------------------------------

uint8_t EEprom::read(uint32_t addr) {

  setAddress( addr );
  
  return read();
}

// ---------------------------------------------------------------------------------

size_t EEprom::read(uint32_t addr,uint8_t *data,uint32_t len) {

  size_t n_read = 0;
  
  memset(data, 0x00, (size_t)len);
  
  for (uint32_t i=0; i<len; ++i) {

    if ( (addr + i) < _eepromSize ) {
      setAddress( addr + i );
      data[i] = read();
      n_read++;
    }
  }

  return n_read;
}

// ---------------------------------------------------------------------------------

uint8_t EEprom::read() {
  
  // set 'data bus' to input
  DATA_LOW_DDR &= ~DATA_LOW_MASK;
  DATA_HIGH_DDR &= ~DATA_HIGH_MASK;

  // first !CE to low then read when !OE is low
  CE_PORT &= ~CE_MASK;
  // each nop is 62.5 ns, http://playground.arduino.cc/Main/AVR
  __asm__("nop\n\t""nop\n\t");  // 125 ns
  OE_PORT &= ~OE_MASK;
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");  // 500 ns

  uint8_t data = (DATA_HIGH_PIN & DATA_HIGH_MASK) | (DATA_LOW_PIN & DATA_LOW_MASK);

  // both !OE and !CE back to high
  OE_PORT |= OE_MASK;
  __asm__("nop\n\t""nop\n\t");  // 125 ns
  CE_PORT |= CE_MASK;

  return data;
}

// ---------------------------------------------------------------------------------

void EEprom::setAddress(uint32_t addr) {

  uint8_t msb_mask = 0;
  uint8_t hsb_mask = 0;

  // In my tests, keeping !P at high is crucial but Vpp=Vcc seems
  // to be not required but according to the data sheets it is
  //
  // EPROM 2764: A14 = A15 = HIGH (!P and Vpp)
  if ( _eepromSize == 0x2000 ) msb_mask = 0xC0;
  // EPROM 27128: A14 = A15 = HIGH (!P and Vpp)
  else if ( _eepromSize == 0x4000 ) msb_mask = 0xC0;
  // EPROM 27256: A15 = HIGH (Vpp)
  else if ( _eepromSize == 0x8000 ) msb_mask = 0x80;
  // EPROM 27512: Vpp is put on pin !OE (!G)
  
  setAddressLSB( (uint8_t)(addr & 0xff) );
  setAddressMSB( (uint8_t)((addr & 0xff00) >> 8) | msb_mask );
  setAddressHSB( (uint8_t)((addr & 0xff0000) >> 16) | hsb_mask );
}

// ---------------------------------------------------------------------------------

void EEprom::setAddressLSB(uint8_t addr) {

  // write low nibble
  DATA_LOW_DDR |= DATA_LOW_MASK;
  DATA_LOW_PORT = (DATA_LOW_PIN & ~DATA_LOW_MASK) | (addr & DATA_LOW_MASK);
  // write high nibble
  DATA_HIGH_DDR |= DATA_HIGH_MASK;
  DATA_HIGH_PORT = (DATA_HIGH_PORT & ~DATA_HIGH_MASK) | (addr & DATA_HIGH_MASK);

  // latch it, positive edge
  STR1_PORT |= STR1_MASK;
  // each nop is 62.5 ns, http://playground.arduino.cc/Main/AVR
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
  STR1_PORT &= ~STR1_MASK;

  // 'data bus' back to input
  DATA_LOW_DDR &= ~DATA_LOW_MASK;
  DATA_HIGH_DDR &= ~DATA_HIGH_MASK;
}

// ---------------------------------------------------------------------------------

void EEprom::setAddressMSB(uint8_t addr) {
  
  // write low nibble
  DATA_LOW_DDR |= DATA_LOW_MASK;
  DATA_LOW_PORT = (DATA_LOW_PIN & ~DATA_LOW_MASK) | (addr & DATA_LOW_MASK);
  // write high nibble
  DATA_HIGH_DDR |= DATA_HIGH_MASK;
  DATA_HIGH_PORT = (DATA_HIGH_PORT & ~DATA_HIGH_MASK) | (addr & DATA_HIGH_MASK);

  // latch it, positive edge
  STR2_PORT |= STR2_MASK;
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
  STR2_PORT &= ~STR2_MASK;

  // 'data bus' back to input
  DATA_LOW_DDR &= ~DATA_LOW_MASK;
  DATA_HIGH_DDR &= ~DATA_HIGH_MASK;
}

// ---------------------------------------------------------------------------------

void EEprom::setAddressHSB(uint8_t addr) {
  
  // write low nibble
  DATA_LOW_DDR |= DATA_LOW_MASK;
  DATA_LOW_PORT = (DATA_LOW_PIN & ~DATA_LOW_MASK) | (addr & DATA_LOW_MASK);
  // write high nibble
  DATA_HIGH_DDR |= DATA_HIGH_MASK;
  DATA_HIGH_PORT = (DATA_HIGH_PORT & ~DATA_HIGH_MASK) | (addr & DATA_HIGH_MASK);

  // latch it, positive edge
  STR3_PORT |= STR3_MASK;
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
  STR3_PORT &= ~STR3_MASK;

  // 'data bus' back to input
  DATA_LOW_DDR &= ~DATA_LOW_MASK;
  DATA_HIGH_DDR &= ~DATA_HIGH_MASK;
}

// ---------------------------------------------------------------------------------

void EEprom::setType(eEEPROMtype eType) {

  uint32_t eepromSize;

  _eepromType = eType;
  
  switch ( _eepromType ) {
    
    case eEEPROM_2716:
        eepromSize = 0x0800;
      break;
      
    case eEEPROM_2732:
        eepromSize = 0x1000;
      break;

    case eEEPROM_2764:
        eepromSize = 0x2000;
      break;

    case eEEPROM_27128:
        eepromSize = 0x4000;
      break;

    case eEEPROM_27256:
        eepromSize = 0x8000;
      break;

    case eEEPROM_27512:
        eepromSize = 0x10000L;
      break;

    default:
      eepromSize = 4096;
  }

  setSize(eepromSize);
}

// ---------------------------------------------------------------------------------

void EEprom::write(uint32_t addr,uint8_t data) {

  setAddress( addr );
  write( data );
}

// ---------------------------------------------------------------------------------

void EEprom::write(uint8_t data) {

  // select the chip
  CE_PORT &= ~CE_MASK;
  
  // write low nibble
  DATA_LOW_DDR |= DATA_LOW_MASK;
  DATA_LOW_PORT = (DATA_LOW_PIN & ~DATA_LOW_MASK) | (data & DATA_LOW_MASK);
  // write high nibble
  DATA_HIGH_DDR |= DATA_HIGH_MASK;
  DATA_HIGH_PORT = (DATA_HIGH_PORT & ~DATA_HIGH_MASK) | (data & DATA_HIGH_MASK);

  // write it into the EEPROM
  WE_PORT &= ~WE_MASK;
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
  WE_PORT |= WE_MASK;

  // deselect the chip
  CE_PORT |= CE_MASK;

  // 'data bus' back to input
  DATA_LOW_DDR &= ~DATA_LOW_MASK;
  DATA_HIGH_DDR &= ~DATA_HIGH_MASK;
}

// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------

