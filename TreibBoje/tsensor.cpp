
//
// File   : tsensor.cpp
//
// Contains useful functions for the OneWire temperature sensors
//

#include "general.h"

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


/** Get the raw temperature (aka. sensor reading) for a given temperature value. */
int getRawTemperature(float temp,int precision) {

  switch (precision) {
    case 9:
      return ((int)(temp / 0.5)) & 0x01ff;
    case 10:
      return ((int)(temp / 0.25)) & 0x03ff;
    case 11:
      return ((int)(temp / 0.125)) & 0x07ff;
    case 12:
    default:
      return ((int)(temp / 0.0625)) & 0x0fff;
  }
  
  return 0;
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

