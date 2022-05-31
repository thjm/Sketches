//
// File   : TreibBoje.ino
//
// Purpose: Frame for fun project 'Treibboje' (experimental)
//
// $Id$
//

// see also: https://arduino.stackexchange.com/questions/19892/list-of-arduino-board-preprocessor-defines
#if (defined ARDUINO_AVR_UNO)
 #warning Building for Arduino Uno!
#endif
#if (defined ARDUINO_AVR_NANO)
 #warning Building for Arduino Nano!
#endif

#include "comms.h"

#ifdef USE_RTC
RTC_DS1307 rtc;
#endif // USE_RTC

#ifdef SEND_DATA
 #if (defined USE_RCSWITCH)
RCSwitch theSender = RCSwitch();
 #endif // USE_RCSWITCH

 #if (defined USE_MORSE)
Morse morseGen = Morse();
 #endif // USE_MORSE
#endif // SEND_DATA

// the global cycle counter
static uint16_t gCycleCounter = 0;

/**
 * Initialization
 */
void setup() {

  if (LED > 0) pinMode(LED, OUTPUT);

#ifdef DEBUG
  /* Initialize serial output at UART_BAUD_RATE bps */
  Serial.begin(UART_BAUD_RATE);
  Serial << F("'TreibBoje' starting ...") << endl;

  Serial << F("Free SRAM: ");
  Serial.println(availableMemory());
 #ifdef SEND_DATA
  Serial << F("Sending data:\n");
 #endif // SEND_DATA
 #ifdef USE_RCSWITCH
  Serial << F("- via RCswitch protocol\n");
 #endif // USE_RCSWITCH
 #ifdef USE_MORSE
  Serial << F("- via morse, speed="); Serial.println(MORSE_SPEED);
 #endif // USE_MORSE
 #ifdef USE_LDR
 Serial << F("+ with LDR readout\n");
 #endif // USE_LDR
 #ifdef READ_UBAT
 Serial << F("+ with U_Bat readout\n");
 #endif // READ_UBAT
 #ifdef USE_RTC
 Serial << F("+ with DS1307 I2C RTC\n");
 #endif // USE_RTC
 #ifdef USE_GPS
 Serial << F("+ with NEO 6M GPS\n");
 #endif // USE_GPS
#endif // DEBUG

  // OneWire setup etc.
  // Start up the library
  sensors.begin();

#ifdef USE_LEDS
  initLED();
  helloLED();
#endif // USE_LEDS

#ifdef SEND_DATA
 #if (defined USE_RCSWITCH)
  // Transmitter is connected to Arduino Pin #10 (TX_PIN)
  theSender.enableTransmit(TX_PIN);

  // default in RCSwitch library is 10
  //theSender.setRepeatTransmit(10);

  // Optional set pulse length.
  theSender.setPulseLength(200);

  // Optional set protocol (default is 1, will work for most outlets)
  //theSender.setProtocol(1);  // -> setPulseLength(350); '0' = 1,3
 #endif // USE_RCSWITCH

 #if (defined USE_MORSE)
  // Transmitter is connected to Arduino Pin #10 (TX_PIN)
  morseGen.enableTransmit(TX_PIN);

  // set the speed with which we output the morse chars
  morseGen.setSpeed(MORSE_SPEED);

  morseGen.print(F("vvv "));
 #endif // USE_MORSE
#endif // SEND_DATA

#ifdef USE_RTC
  Wire.begin();
  rtc.begin();

  if ( !rtc.isrunning() ) {
 #ifdef DEBUG
    Serial << F("RTC is NOT running!") << endl;
 #endif // DEBUG
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

    DateTime now = rtc.now();
  }
 #ifdef DEBUG
  else
    Serial << F("RTC is running!") << endl;
 #endif // DEBUG
#endif // USE_RTC

#if (defined SCAN_SENSORS) && (defined DEBUG)
  // locate devices on the bus
  Serial.print(F("Locating devices..."));
  Serial.print(F("Found "));
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(F(" devices."));

  // report parasite power requirements
  Serial.print(F("Parasite power is: "));
  Serial.println((sensors.isParasitePowerMode() ? "ON" : "OFF"));

  byte addr[8];
  for ( byte nSensor=0; nSensor<sensors.getDeviceCount(); ++nSensor) {
    if ( sensors.getAddress(addr, nSensor) ) {
      sensors.setResolution(addr, TEMPERATURE_PRECISION);
      Serial << "Device " << (int)nSensor << ": ";
 #if 1
      printAddress( addr );
 #else
      for ( byte i=0; i<8; ++i ) {
        Serial.print(addr[i], HEX);
        Serial.print(" ");
      }
 #endif
      Serial.println();
    }
  }
#else
  // set desired precision of the temperature sensors
  byte addr[8];
  for ( byte nSensor=0; nSensor<sensors.getDeviceCount(); ++nSensor) {
    if ( sensors.getAddress(addr, nSensor) ) {
      sensors.setResolution(addr, TEMPERATURE_PRECISION);
    }
  }
#endif // SCAN_SENSORS && DEBUG

#ifdef USE_ADC
  initADC();
#endif // USE_ADC

#ifdef DEBUG
  Serial << F("setup() done!\n");
#endif // DEBUG
}

/**
 * Main loop
 */
void loop() {

  if ( LED > 0 ) digitalWrite(LED, gCycleCounter & 0x01);

  SEND_SYNC(1);

#ifdef USE_MORSE
  morseGen.print(F("QAM de DC2IP "));
#endif // USE_MORSE

#ifdef DEBUG
  Serial << "# " << gCycleCounter << endl;
#endif // DEBUG
  SEND_CYCLE_COUNTER(gCycleCounter);

#ifdef USE_RTC
  DateTime now = rtc.now();

  printDateTime(now);
#endif // USE_RTC

#ifdef READ_SENSORS
  if ( sensors.getDeviceCount() > 0 ) {

    sensors.requestTemperatures(); // Send the command to get temperatures

    float temp;

    // printout of temperature sensor values in the order they are enumerated
    // when the 1-wire bus is initialized
    // reading will be prefixed via T1: .. T2: ... etc.
    // alternative printout, only known sensors correctly prefixed
    if ( sensors.isConnected( gSensor1 ) ) {
      temp = sensors.getTempC( gSensor1 );
 #ifdef DEBUG
      Serial << "T1: " << temp << " "
             << getRawTemperature(temp, TEMPERATURE_PRECISION)
             << " ";
 #endif // DEBUG
      SEND_T1(temp);
    }
    if ( sensors.isConnected( gSensor2 ) ) {
      temp = sensors.getTempC( gSensor2 );
 #ifdef DEBUG
      Serial << "T2: " << temp << " "
             << getRawTemperature(temp, TEMPERATURE_PRECISION)
             << " ";
 #endif // DEBUG
      SEND_T2(temp);
    }
    if ( sensors.isConnected( gSensor3 ) ) {
      temp = sensors.getTempC( gSensor3 );
 #ifdef DEBUG
      Serial << "T3: " << temp << " "
             << getRawTemperature(temp, TEMPERATURE_PRECISION)
             << " ";
 #endif // DEBUG
      SEND_T3(temp);
    }
    if ( sensors.isConnected( gSensor4 ) ) {
      temp = sensors.getTempC( gSensor4 );
 #ifdef DEBUG
      Serial << "T4: " << temp << " "
             << getRawTemperature(temp, TEMPERATURE_PRECISION)
             << " ";
 #endif // DEBUG
      SEND_T4(temp);
    }
    if ( sensors.isConnected( gSensor5 ) ) {
      temp = sensors.getTempC( gSensor5 );
 #ifdef DEBUG
      Serial << "T5: " << temp << " "
             << getRawTemperature(temp, TEMPERATURE_PRECISION)
             << " ";
 #endif // DEBUG
      SEND_T5(temp);
    }
 #ifdef DEBUG
    Serial.println();
 #endif // DEBUG
  } // if ( getDeviceCount() > 0 )
#endif // READ_SENSORS

#ifdef USE_LDR
  int ldr_value = readLDR();

  SEND_LDR(ldr_value);
#endif // USE_LDR

#ifdef READ_UBAT
  int ubat_value = readUBAT();

  SEND_UBAT(ubat_value);
#endif // READ_UBAT

  // some symbols lost if after flashLED() !?
  SEND_SYNC(2);

#ifdef USE_LEDS
  flashLED();
#endif // USE_LEDS

  gCycleCounter = ( gCycleCounter == 0x1ff ) ? 0 : (gCycleCounter+1);

#if 0
  // wait a bit, if too few sensors
  if ( sensors.getDeviceCount() <= 2 ) delay(2000);
#else
  //delay(LOOP_DELAY);
#endif
}

/**
 * Try to inquire max. available memory by allocating until heap is exhausted.
 */
int availableMemory() {

  // see also:
  // https://learn.adafruit.com/memories-of-an-arduino/measuring-free-memory
  extern int __heap_start, *__brkval;
  int v;

  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

#ifdef USE_RTC
/** function to print the current data and time to the Serial console. */
static void printDateTime(DateTime& dt) {

#ifdef DEBUG
  Serial << dt.year() << '/'
         << ((dt.month()<10) ? "0" : "") << dt.month() << '/'
         << ((dt.day()<10) ? "0" : "") << dt.day() << ' '
         << ((dt.hour()<10) ? "0" : "") << dt.hour() << ':'
         << ((dt.minute()<10) ? "0" : "") << dt.minute() << ':'
         << ((dt.second()<10) ? "0" : "") << dt.second()
         << endl;
#endif // DEBUG
}
#endif // USE_RTC
