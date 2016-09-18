
//
// File   : EEprom.h
//
// Purpose: Declaration for the class EEprom
//
// $Id$
//

#ifndef _EEPROM_h_
 #define _EEprom_h_

#include <Arduino.h>

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
  /** Constructor for the class EEprom.
   *
   *  Modes of the used pins as well as their default state are set.
   */
  EEprom();

  /** Read a byte from the specified address. */
  uint8_t read(uint32_t);

  /** Read a block of 'length' bytes from the specified address. */
  void read(uint32_t addr,uint8_t *data,uint32_t length);

  /** Write a byte to the specified address. */
  void write(uint32_t,uint8_t);

  /** Write a block of 'length' bytes to the specified addess (Tbd!). */
  //void write(uint32_t addr,uint8_t *data,uint32_t length);

protected:

  /** Read a byte. */
  uint8_t read();

  /** Write a byte. */
  void write(uint8_t);

  /** Set the address. */
  void setAddress(uint32_t);

  /** Set the LSB of the address. */
  void setAddressLSB(uint8_t);
  /** Set the MSB of the address. */
  void setAddressMSB(uint8_t);
  /** Set the HSB of the address. */
  void setAddressHSB(uint8_t);
};

#endif // _EEprom_h_
