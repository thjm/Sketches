//
// File   : TreibBoje1.ino
//
// Purpose: Frame for experimental 'Treibbojen' project
//
// $Id$
//

// debug via serial interface
#define DEBUG
// scan sensors (to inquire addresses)
#define SCAN_SENSORS

#ifdef DEBUG
 // http://arduiniana.org/libraries/streaming/
 // has to be included BEFORE Flash
 #include <Streaming.h>
 #include <Flash.h>
#endif // DEBUG

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

// UART baud rate
#define UART_BAUD_RATE  9600

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2
#define TEMPERATURE_PRECISION 12

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// arrays to hold device addresses of known 1-wire temp. sensors
// sensor #1
//DeviceAddress gSensor1 = { };
// sensor #2
//DeviceAddress gSensor2 = {  };
// sensor #3
//DeviceAddress gSensor3 = {  };
// sensor #4
//DeviceAddress gSensor4 = {  };
// sensor #5
//DeviceAddress gSensor5 = {  };

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
      //printAddress( addr );
      for ( byte i=0; i<8; ++i ) {
        Serial.print(addr[i], HEX);
        Serial.print(" ");
      }
      Serial.println();
    }
  }
#endif // SCAN_SENSORS
}

/**
 * Main loop
 */
void loop() {
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
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

/** function to print a device's resolution */
void printResolution(DeviceAddress deviceAddress) {

  Serial.print("Resolution: ");
  Serial.print(sensors.getResolution(deviceAddress));
  Serial.println();    
}


