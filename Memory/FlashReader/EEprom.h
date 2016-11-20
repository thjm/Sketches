
//
// File   : EEprom.h
//
// Purpose: Declaration of the class EEprom
//
// $Id$
//

#ifndef _EEPROM_h_
 #define _EEprom_h_

#include <Arduino.h>

/*

  The circuit:
  * low nibble of data lines is connected to PC0 .. PC3 (A0..A3)
  * high nibble of data lines is connected to PD4 .. PD7 (D4..D7)
  * CE is connected to PB5 (D13)
  * OE is connected to PB4 (D12)
  * WE is connected to PB3 (D11)
  * STR1 (Strobe 1) latches LSB of address bus, connected to PB2 (D10)
  * STR2 (Strobe 2) latches MSB of address bus, connected to PB1 (D9)
  * STR3 (Strobe 3) latches HSB of address bus, connected to PB0 (D8)
  * reserved lines:
    - PC4/PC5 for SDA/SCL (A4/A5)
    - PD0/PD1 for RxD/TxD (D0/D1)
    - PD2/PD3 not used

  Port manipulation, for the Arduino Nano we have:
  - PB0 .. PB5 = D8 .. D13
  - PC0 .. PC5 = A0 .. A5
  - PD0 .. PD7 = D0 .. D7
  See also:
  https://www.arduino.cc/en/Reference/PortManipulation
  
  https://www.arduino.cc/en/Hacking/Atmega168Hardware


 */
 
/** Port for low nibble of data byte. */
#define DATA_LOW_PORT    PORTC
#define DATA_LOW_DDR     DDRC
#define DATA_LOW_PIN     PINC
#define DATA_LOW_MASK    ((1<<PC0) | (1<<PC1) | (1<<PC2) | (1<<PC3))

/** Port for high nibble of data byte. */
#define DATA_HIGH_PORT   PORTD
#define DATA_HIGH_DDR    DDRD
#define DATA_HIGH_PIN    PIND
#define DATA_HIGH_MASK   ((1<<PD4) | (1<<PD5) | (1<<PD6) | (1<<PD7))

/** !CE = chip enable, low active. */
#define CE_PORT          PORTB
#define CE_DDR           DDRB
#define CE_PIN           PINB
#define CE_MASK          (1<<PB5)

/** !OE = output enable, low active. */
#define OE_PORT          PORTB
#define OE_DDR           DDRB
#define OE_PIN           PINB
#define OE_MASK          (1<<PB4)

/** !WE = write enable, low active. */
#define WE_PORT          PORTB
#define WE_DDR           DDRB
#define WE_PIN           PINB
#define WE_MASK          (1<<PB3)

/** STR1 = STROBE1, strobe signal for first 74HCT574 latch. */
#define STR1_PORT        PORTB
#define STR1_DDR         DDRB
#define STR1_PIN         PINB
#define STR1_MASK        (1<<PB2)

/** STR2 = STROBE2, strobe signal for second 74HCT574 latch. */
#define STR2_PORT        PORTB
#define STR2_DDR         DDRB
#define STR2_PIN         PINB
#define STR2_MASK        (1<<PB1)

/** STR3 = STROBE3, strobe signal for third 74HCT574 latch. */
#define STR3_PORT        PORTB
#define STR3_DDR         DDRB
#define STR3_PIN         PINB
#define STR3_MASK        (1<<PB0)

/**  */
class EEprom {
  friend void testAddressLatches();

public:

  /** EEPROM types, or EPROM types ? */
  typedef enum {

    eEEPROM_NONE = 0,
    eEEPROM_2716 = 1,
    eEEPROM_2732,
    eEEPROM_2764,
    eEEPROM_27128,
    eEEPROM_27256,
    eEEPROM_27512,
    eEEPROM_27010, // 27C1001
    eEEPROM_27020, // 27C2001
    eEEPROM_27040, // 27C4001, 27C4002

  } eEEPROMtype;

  /** Constructor for the class EEprom.
   *
   *  Modes of the used pins as well as their default state are set.
   */
  EEprom();

  /**  */
  virtual ~EEprom() { }
  
  /** Get the address mask for the E(E)PROM. */
  uint32_t getAddressMask() {
    return _addrMask;
  }
  
  /** Get the size in bytes of the E(E)PROM. */
  uint32_t getSize() {
    return _eepromSize;
  }

  /** Get the type of the E(E)PROM. */
  eEEPROMtype getType() {
    return _eepromType;
  }

  /** Get the type of the E(E)PROM as String. */
  static String getTypeString(eEEPROMtype eType);
  
  /** Set the type of E(E)PROM to be used (required). 
   *  
   *  This method wil set the size of the E(E)PROM automatically.
   */
  void setType(eEEPROMtype eType);
  
  /** Read a byte from a previously set address. */
  virtual uint8_t read(uint32_t);

  /** Read a block of 'length' bytes from the specified address. 
   *  
   *  Returns the number of bytes read, as this is limited by the maximum 
   *  possible address of the EEPROM.
   */
  virtual size_t read(uint32_t addr,uint8_t *data,uint32_t length);

  /** Write a byte to the specified address. */
  virtual void write(uint32_t,uint8_t);

  /** Write a block of 'length' bytes to the specified addess (Tbd!). */
  //virtual void write(uint32_t addr,uint8_t *data,uint32_t length);

protected:

  /** Set the maximum size of the E(E)PROM. */
  void setSize(uint32_t eSize) {
    _eepromSize = eSize;
    _addrMask = _eepromSize - 1;
  }
  
  /** Read a byte froma  previously set address. */
  virtual uint8_t read();

  /** Write a byte to a previously set address. */
  virtual void write(uint8_t);

  /** Set the address. */
  void setAddress(uint32_t);

  /** Set the LSB (least significant byte) of the address. */
  void setAddressLSB(uint8_t);
  /** Set the MSB of the address. */
  void setAddressMSB(uint8_t);
  /** Set the HSB of the address. */
  void setAddressHSB(uint8_t);

private:
  
  uint32_t     _eepromSize;   // size of the E(E)PROM in bytes
  eEEPROMtype  _eepromType;   // type 'code' for the used E(E)PROM chip
  
  uint32_t     _addrMask;     // can be used to mask out unneeded address bits
};

#endif // _EEprom_h_
