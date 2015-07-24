//
// File   : TreibBoje1.ino
//
// Purpose: Frame for fun project 'Treibbojen' (experimental)
//
// $Id$
//

// debug via serial interface
#define DEBUG
// scan sensors (to inquire addresses)
#define SCAN_SENSORS
// read temperature sensors
#define READ_SENSORS
// send the data via 433MHZ Tx
#define SEND_DATA

#ifdef DEBUG
 // http://arduiniana.org/libraries/streaming/
 // has to be included BEFORE Flash
 #include <Streaming.h>
 #include <Flash.h>
#endif // DEBUG

#ifdef SEND_DATA
 #include <RCSwitch.h>
#endif // SEND_DATA

// http://www.pjrc.com/teensy/td_libs_OneWire.html
#include <OneWire.h>

// https://github.com/milesburton/Arduino-Temperature-Control-Library
// http://milesburton.com/Main_Page?title=Dallas_Temperature_Control_Library
// Note:
// The library archive is not well formated and has to be renamed into DallasTemperature
//
// DS18B20       +-------+
//               |       |
//               | 18B20 |
//               |       |
//               ++--+--++
//                |  |  |
//              GND Dq  Vdd
//
#include <DallasTemperature.h>

#ifdef SEND_DATA

#define TX_PIN     10
RCSwitch theSender = RCSwitch();

#endif // SEND_DATA

// UART baud rate
#define UART_BAUD_RATE  9600

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2
#define TEMPERATURE_PRECISION 10

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

/**
 * Initialization
 */
void setup()
{
#ifdef DEBUG
  /* Initialize serial output at UART_BAUD_RATE bps */
  Serial.begin(UART_BAUD_RATE);
  Serial << F("Starting ...");

  Serial << F("Free SRAM: ");
  Serial.println(availableMemory());
#endif // DEBUG

  // OneWire setup etc.
  // Start up the library
  sensors.begin();

#ifdef SEND_DATA
  // Transmitter is connected to Arduino Pin #10 (TX_PIN)
  theSender.enableTransmit(TX_PIN);

  // Optional set pulse length.
  theSender.setPulseLength(200);
  
  // Optional set protocol (default is 1, will work for most outlets)
  //theSender.setProtocol(1);  // -> setPulseLength(350); '0' = 1,3
#endif // SEND_DATA

#ifdef SCAN_SENSORS
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
      printAddress( addr );
//      for ( byte i=0; i<8; ++i ) {
//        Serial.print(addr[i], HEX);
//        Serial.print(" ");
//      }
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
#endif // SCAN_SENSORS
}

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

#ifdef SEND_DATA
  // send dumy telegram
  theSender.send(0, 12);
  theSender.send(4095, 12);
#endif // SEND_DATA

#ifdef READ_SENSORS
  if ( sensors.getDeviceCount() > 0 ) {
    sensors.requestTemperatures(); // Send the command to get temperatures

#ifdef DEBUG
    float temp;
    
    // printout of temperature sensor values in the order they are enumerated at the 1-wire bus
    // reading will be prefixed via T1: .. T2: ... etc.
    // alternative printout, only known sensors correctly prefixed
    if ( sensors.isConnected( gSensor1 ) ) {
      temp = sensors.getTempC( gSensor1 );
      Serial << "T1: " << temp << " " 
             << getRawTemperature(temp, TEMPERATURE_PRECISION);
    }
    if ( sensors.isConnected( gSensor2 ) ) {
      temp = sensors.getTempC( gSensor2 );
      Serial << "T2: " << temp << " " 
             << getRawTemperature(temp, TEMPERATURE_PRECISION);
    }
    if ( sensors.isConnected( gSensor3 ) ) {
      temp = sensors.getTempC( gSensor3 );
      Serial << "T3: " << temp << " " 
             << getRawTemperature(temp, TEMPERATURE_PRECISION);
    }
    if ( sensors.isConnected( gSensor4 ) ) {
      temp = sensors.getTempC( gSensor4 );
      Serial << "T4: " << temp << " " 
             << getRawTemperature(temp, TEMPERATURE_PRECISION);
    }
    if ( sensors.isConnected( gSensor5 ) ) {
      temp = sensors.getTempC( gSensor5 );
      Serial << "T5: " << temp << " " 
             << getRawTemperature(temp, TEMPERATURE_PRECISION);
    }
    Serial.println();
#endif // DEBUG
  } // if ( getDeviceCount() > 0 )
#endif // READ_SENSORS
}

/**
 * Try to inquire max. available memory by allocating until heap is exhausted.
 *
 * from: https://devel-ik.fzk.de/wsvn/fd-online/Diverse/Oktokopter/Arduino2Flasher/trunk/Arduino2Flasher.ino
 */
int availableMemory() {

  int size = 10000;
  byte *buf;
  while ((buf = (byte *) malloc(--size)) == NULL);
  free(buf);
  return size;
}

/** function to print a device address */
void printAddress(DeviceAddress deviceAddress) {

  for (uint8_t i = 0; i < 8; i++) {
    Serial.print("0x");
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
    Serial.print(" ");
  }
}

/** function to print a device's resolution */
void printResolution(DeviceAddress deviceAddress) {

  Serial.print("Resolution: ");
  Serial.print(sensors.getResolution(deviceAddress));
  Serial.println();    
}


