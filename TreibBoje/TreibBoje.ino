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

#include "general.h"
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

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// arrays to hold device addresses of known 1-wire temp. sensors
// sensor #1
DeviceAddress gSensor1 = { 0x28, 0xCE, 0x15, 0x46, 0x06, 0x00, 0x00, 0x04 };
// sensor #2
DeviceAddress gSensor2 = { 0x28, 0x65, 0x5D, 0x47, 0x06, 0x00, 0x00, 0xF8 };
// sensor #3
DeviceAddress gSensor3 = { 0x28, 0x76, 0xC1, 0xE0, 0x06, 0x00, 0x00, 0x43 };
// sensor #4
DeviceAddress gSensor4 = { 0x28, 0x7A, 0x31, 0xE0, 0x06, 0x00, 0x00, 0x98 };
// sensor #5
DeviceAddress gSensor5 = { 0x28, 0x6B, 0x84, 0xDF, 0x06, 0x00, 0x00, 0xBB };

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
  Serial << F("''TreibBoje' starting ...") << endl;

  Serial << F("Free SRAM: ");
  Serial.println(availableMemory());
 #ifdef SEND_DATA
  Serial << F("Sending data...\n");
 #endif // SEND_DATA
 #ifdef USE_RCSWITCH
  Serial << F("- with RCswitch protocol\n");
 #endif // USE_RCSWITCH
 #ifdef USE_MORSE
  Serial << F("- with morse, speed="); Serial.println(MORSE_SPEED);
 #endif // USE_MORSE
 #ifdef USE_RTC
 Serial << F("- with DS1307 I2C RTC\n");
 #endif // USE_RTC
#endif // DEBUG

  // OneWire setup etc.
  // Start up the library
  sensors.begin();

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
 
  morseGen.print("vvv");
 #endif // USE_MORSE

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
#endif // SEND_DATA

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

#ifdef USE_LDR
  //           PhotoR     10K
  // +5    o---/\/\/--.--/\/\/---o GND
  //                  |
  // Pin 0 o----------+
  //
  pinMode(LDR_PIN, INPUT);
  
  // change range and resolution of analog pin, default
  analogReference(DEFAULT);
#endif // USE_LDR

#ifdef USE_LEDS
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
#endif // USE_LEDS

#ifdef DEBUG
  Serial << F("setup() done!\n");
#endif // DEBUG
}

/** Get the raw temperature (aka. sensor reading) for a given temperature value. */
int getRawTemperature(float temp,int precision=12) {

  switch (precision) {
    case 12:
      return ((int)(temp / 0.0625)) & 0x0fff;
    case 11:
      return ((int)(temp / 0.125)) & 0x07ff;
    case 10:
      return ((int)(temp / 0.25)) & 0x03ff;
    case 9:
      return ((int)(temp / 0.5)) & 0x01ff;
  }
  
  return 0;
}

/**
 * Main loop
 */
void loop() {

  if ( LED > 0 ) digitalWrite(LED, gCycleCounter & 0x01);


#ifdef SEND_DATA
 #ifdef USE_RCSWITCH
  // send some dummy telegrams, cannot set 0!
  //theSender.send(0x0e01, 12);
  //theSender.send(0x0e02, 12);
  //theSender.send(0x0fff, 12);
 #endif // USE_RCSWITCH
 #ifdef USE_MORSE
 #endif // USE_MORSE 
#endif // SEND_DATA

  SEND_SYNC(1);
  
#ifdef USE_MORSE
  morseGen.print("QAM de DC2IP ");
#endif // USE_MORSE

  SEND_CYCLE_COUNTER();
#ifdef DEBUG
  Serial << "# " << gCycleCounter << endl;
#endif // DEBUG

#ifdef USE_RTC
  DateTime now = rtc.now();
    
  printDateTime(now);
#endif // USE_RTC

#ifdef READ_SENSORS
  if ( sensors.getDeviceCount() > 0 ) {

    sensors.requestTemperatures(); // Send the command to get temperatures

    float temp;
    
    // printout of temperature sensor values in the order they are enumerated at the 1-wire bus
    // reading will be prefixed via T1: .. T2: ... etc.
    // alternative printout, only known sensors correctly prefixed
    if ( sensors.isConnected( gSensor1 ) ) {
      temp = sensors.getTempC( gSensor1 );
 #ifdef DEBUG
      Serial << "T1: " << temp << " " 
             << getRawTemperature(temp, TEMPERATURE_PRECISION)
             << " ";
 #endif // DEBUG
      SEND_T1(getRawTemperature(temp, TEMPERATURE_PRECISION));
    }
    if ( sensors.isConnected( gSensor2 ) ) {
      temp = sensors.getTempC( gSensor2 );
 #ifdef DEBUG
      Serial << "T2: " << temp << " " 
             << getRawTemperature(temp, TEMPERATURE_PRECISION)
             << " ";
 #endif // DEBUG
      SEND_T2(getRawTemperature(temp, TEMPERATURE_PRECISION));
    }
    if ( sensors.isConnected( gSensor3 ) ) {
      temp = sensors.getTempC( gSensor3 );
 #ifdef DEBUG
      Serial << "T3: " << temp << " " 
             << getRawTemperature(temp, TEMPERATURE_PRECISION)
             << " ";
 #endif // DEBUG
      SEND_T3(getRawTemperature(temp, TEMPERATURE_PRECISION));
    }
    if ( sensors.isConnected( gSensor4 ) ) {
      temp = sensors.getTempC( gSensor4 );
 #ifdef DEBUG
      Serial << "T4: " << temp << " " 
             << getRawTemperature(temp, TEMPERATURE_PRECISION)
             << " ";
 #endif // DEBUG
      SEND_T4(getRawTemperature(temp, TEMPERATURE_PRECISION));
    }
    if ( sensors.isConnected( gSensor5 ) ) {
      temp = sensors.getTempC( gSensor5 );
 #ifdef DEBUG
      Serial << "T5: " << temp << " " 
             << getRawTemperature(temp, TEMPERATURE_PRECISION)
             << " ";
 #endif // DEBUG
      SEND_T5(getRawTemperature(temp, TEMPERATURE_PRECISION));
    }
 #ifdef DEBUG
    Serial.println();
 #endif // DEBUG
  } // if ( getDeviceCount() > 0 )
#endif // READ_SENSORS

#ifdef USE_LDR
  // divide the resulting value:
  // - with a 10k resistor divide the value by 2
  // - for 100k resistor divide by 4.
  // - this is a 10bit (0..1023) ADC
  int ldr_value = analogRead(LDR_PIN)/4;
 #ifdef DEBUG
  Serial << "LDR: " << ldr_value << endl;
 #endif // DEBUG
  SEND_LDR(ldr_value);
#endif // USE_LDR

#ifdef USE_LEDS
  digitalWrite(LED1, ((gCycleCounter & 0x02) ? HIGH : LOW));
  digitalWrite(LED2, ((gCycleCounter & 0x02) ? LOW : HIGH));
  digitalWrite(LED3, ((gCycleCounter & 0x04) ? HIGH : LOW));
  digitalWrite(LED4, ((gCycleCounter & 0x08) ? HIGH : LOW));
#endif // USE_LEDS

  SEND_SYNC(2);
  
  gCycleCounter = ( gCycleCounter == 0x1ff ) ? 0 : (gCycleCounter+1);

#if 1
  // wait a bit, if too few sensors
  if ( sensors.getDeviceCount() <= 2 ) delay(2000);
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

/** function to print a Onewire device address */
void printAddress(DeviceAddress deviceAddress) {

  for (uint8_t i = 0; i < 8; i++) {
    Serial.print("0x");
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
    Serial.print(" ");
  }
}

/** function to print a Onewire device's resolution */
void printResolution(DeviceAddress deviceAddress) {

  Serial.print("Resolution: ");
  Serial.print(sensors.getResolution(deviceAddress));
  Serial.println();    
}

#ifdef USE_RTC
/** function to print the current data and time to the Serial console. */
static void printDateTime(DateTime& dt) {

  Serial << dt.year() << '/' 
         << ((dt.month()<10) ? "0" : "") << dt.month() << '/'
         << ((dt.day()<10) ? "0" : "") << dt.day() << ' '
         << ((dt.hour()<10) ? "0" : "") << dt.hour() << ':'
         << ((dt.minute()<10) ? "0" : "") << dt.minute() << ':'
         << ((dt.second()<10) ? "0" : "") << dt.second() 
         << endl;
}
#endif // USE_RTC

