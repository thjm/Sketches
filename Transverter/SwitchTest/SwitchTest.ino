
//
// Test code for the switches attached to the Teensy in the 4m Transverter
//


#include <Arduino.h>

/** Leftmost switch. */
uint8_t const kLEFT_SWITCH = 2;
/** Middle switch. */
uint8_t const kMIDDLE_SWITCH = 3;
/** Rightmost switch. */
uint8_t const kRIGHT_SWITCH = 4;

void setup() {

  // Open serial communications and wait for port to open:
  Serial.begin(9600);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("\nChecking for switches...");

  // initialize some digital pins as input (the default)
  pinMode(kLEFT_SWITCH, INPUT);
  pinMode(kMIDDLE_SWITCH, INPUT);
  pinMode(kRIGHT_SWITCH, INPUT);

  // and activate the pullups
  digitalWrite(kLEFT_SWITCH, HIGH);
  digitalWrite(kMIDDLE_SWITCH, HIGH);
  digitalWrite(kRIGHT_SWITCH, HIGH);
}

void loop() {

  static bool left_switch = (bool)digitalRead(kLEFT_SWITCH);
  static bool middle_switch = (bool)digitalRead(kMIDDLE_SWITCH);
  static bool right_switch = (bool)digitalRead(kRIGHT_SWITCH);

  static bool first = true;

  if ( first ) {

    Serial.print("Left switch is ");
    Serial.println((left_switch ? "ON" : "OFF"));

    Serial.print("Middle switch is ");
    Serial.println((middle_switch ? "ON" : "OFF"));

    Serial.print("Right switch is ");
    Serial.println((right_switch ? "ON" : "OFF"));

    first = false;
  }

  if ( (bool)digitalRead(kLEFT_SWITCH) != left_switch) {
    Serial.print("Left switch is ");
    left_switch = !left_switch;
    Serial.println((left_switch ? "ON" : "OFF"));
  }

  if ( (bool)digitalRead(kMIDDLE_SWITCH) != middle_switch) {
    Serial.print("Middle switch is ");
    middle_switch = !middle_switch;
    Serial.println((middle_switch ? "ON" : "OFF"));
  }

  if ( (bool)digitalRead(kRIGHT_SWITCH) != right_switch) {
    Serial.print("Right switch is ");
    right_switch = !right_switch;
    Serial.println((right_switch ? "ON" : "OFF"));
  }

  delay(100);
}
