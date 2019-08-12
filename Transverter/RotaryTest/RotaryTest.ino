
//
// Test code for the rotary encoder attached to the Teensy in the 4m Transverter
//
// see also: 
// - Sketchbook/RotaryEncoder/rotary1/rotary1.ino
// - http://www.pjrc.com/teensy/td_libs_Encoder.html
//

#include <Encoder.h>

// Teensy pins to which encoder is attached to. Attach the center to ground.
uint8_t const kROTARY_PIN1 = 17; // CLK
uint8_t const kROTARY_PIN2 = 16; // DT
// Teensy pin where the push-button (axis of rotary encoder) is connected to
uint8_t const kPUSHBUTTON_PIN = 15; // SW

// number of steps for a complete round
#define NUM_STEPS          60

// define to enable weak pullups.
#define ENABLE_PULLUPS


// Change these two numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
//   avoid using pins with LEDs attached
Encoder myEnc(kROTARY_PIN2, kROTARY_PIN1);

/**  */
void setup() {

  pinMode(kPUSHBUTTON_PIN, INPUT);
#ifdef ENABLE_PULLUPS
  digitalWrite(kPUSHBUTTON_PIN, HIGH);
#endif // ENABLE_PULLUPS
  
  Serial.begin(9600);
  Serial.println("'rotary1' : basic encoder test:");
}

long oldPosition  = -1;

/**  */
void loop() {

  if ( digitalRead(kPUSHBUTTON_PIN) == 0 ) {
    oldPosition = -1;
    myEnc.write(0);
    delay(500);
    Serial.println("Counter reset");
  }

  long newPosition = myEnc.read();

  if ( newPosition >= NUM_STEPS ) {
    newPosition -= NUM_STEPS;
    myEnc.write(newPosition);
  }
  if ( newPosition < 0 ) {
    newPosition += NUM_STEPS;
    myEnc.write(newPosition);
  }
  
  if (newPosition != oldPosition) {
    oldPosition = newPosition;
    
    Serial.println(newPosition);
  }
}
