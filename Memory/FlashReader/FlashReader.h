//
// File   : FlashReader.h
//
// Purpose: Contains 'global' declarations etc. for FlashReader project
//


#ifndef _FlashReader_h_
 #define _FlashReader_h_

#include "EEprom.h"

#define DEBUG
// execute test code (if any)
#undef TEST
#define INTERACTIVE   1
#if (INTERACTIVE == 0)
 #undef DEBUG
#endif

// UART baud rate
#define UART_BAUD_RATE  9600

// the E(E)PROM object
extern EEprom eeprom;

/** Size of the local data buffer, more cannot be read/flashed in one go. */
const int kMAX_BLOCK_SIZE = 256;

// 'global' variables
extern uint8_t eepromData[kMAX_BLOCK_SIZE];
extern uint32_t eepromAddr;
extern EEprom::eEEPROMtype eepromType;

// function prototypes

/** 'loop()' function for the interactive version. */
extern void loopInteractive();
/** 'loop()' function for the non-interactive version. */
extern void loopNonInteractive();

/** Run the various tests for the hardware (tests.cpp). */
extern void runTests(void);

#endif // _FlashReader_h_
