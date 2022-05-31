
//
// File   : TransvControl.ino
//
// Purpose: Sketch to control operation of 4m Transverter by UT5JCW
//

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include <CATutil.h>
#include <IcomCAT.h>

#include "TransvControl.h"
#include "IcomControl.h"

#define DEBUG
#define DEBUG_SWITCH

#ifdef DEBUG
 // http://arduiniana.org/libraries/streaming/
 #include <Streaming.h>

 // UART baud rate
 #define UART_BAUD_RATE  9600
#endif // DEBUG

// set the LCD address to 0x27 for a 20 chars and 2 line display
LiquidCrystal_I2C lcd(0x27,20,2);

// use RX2,TX2 on the Teensy 3.1/3.2 board, Serial1 is identical to Serial and wired to USB
#define rxSerial  Serial2

IcomControl Icom706(rxSerial,kIC706MK2G_ADDR);

/**  */
void setup() {

  // setup of LEDs
  pinMode(kRX_GREEN_LED, OUTPUT);
  pinMode(kRX_RED_LED, OUTPUT);

  pinMode(kTX_GREEN_LED, OUTPUT);
  pinMode(kTX_RED_LED, OUTPUT);

  // setup of switches
  pinMode(kLEFT_SWITCH, INPUT);
  pinMode(kMIDDLE_SWITCH, INPUT);
  pinMode(kRIGHT_SWITCH, INPUT);

  // and activate their pullups
  digitalWrite(kLEFT_SWITCH, HIGH);
  digitalWrite(kMIDDLE_SWITCH, HIGH);
  digitalWrite(kRIGHT_SWITCH, HIGH);

  // setup of the rotary encoder
  // ...

  // setup serial interface for RX
  rxSerial.begin(9600);

  // setup serial interface for TX
  //txSerial.begin(4800);

  // setup of the LCD display via LCM1602
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
          // "0123456789012345"
  lcd.print("4m Transverter");
  lcd.setCursor(0, 1);
  lcd.print("by UT5JCW/DC2IP");

  delay(1000);

#ifdef DEBUG
  // serial interface for debugging etc.
  Serial.begin(9600);

#if 0
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
#endif

  Serial.println("Initialisation done!");
#endif // DEBUG

  lcd.clear();
}

/** @Return true if 'left' switch is on. */
bool isLeftSwitchOn() {
  return (bool)digitalRead(kLEFT_SWITCH);
}
#define isRxControlEnabled() isLeftSwitchOn()

/** @Return true if 'middle' switch is on. */
bool isMiddleSwitchOn() {
  return (bool)digitalRead(kMIDDLE_SWITCH);
}
/** @Return true if 'right' switch is on. */
bool isRightSwitchOn() {
  return (bool)digitalRead(kRIGHT_SWITCH);
}

TransvControlRec_t transvControl;

/**  */
void loop() {

  static bool first = true;
  static unsigned long loop_counter = millis();
  static unsigned long lcd_counter = millis();

#ifdef DEBUG_SWITCH
  static bool left_switch = (bool)digitalRead(kLEFT_SWITCH);
  static bool middle_switch = (bool)digitalRead(kMIDDLE_SWITCH);
  static bool right_switch = (bool)digitalRead(kRIGHT_SWITCH);
#endif // DEBUG_SWITCH

  transvControl.rxMode = Icom706.getMode();
  transvControl.rxFrequency = Icom706.getFrequency();

  //transvControl.txMode = FT817.getMode();
  //transvControl.txFrequency = FT817.getFrequency();

  if ( first ) {

#ifdef DEBUG_SWITCH
    Serial.print("Left switch is ");
    Serial.println((left_switch ? "ON" : "OFF"));

    Serial.print("Middle switch is ");
    Serial.println((middle_switch ? "ON" : "OFF"));

    Serial.print("Right switch is ");
    Serial.println((right_switch ? "ON" : "OFF"));
#endif // DEBUG_SWITCH

    // prepare for RX frequency display
    // 0123456789012345
    // RX_433.12345_MHz
    lcd.setCursor(0, 0);
    lcd.print("RX");
    lcd.setCursor(13, 0);
    lcd.print("MHz");

    // prepare for TX frequency display
    // 0123456789012345
    // TX_433.12345_MHz

    //transvControl.desiredMode = YaesuCAT::eModeUSB;
    transvControl.desiredFrequency = 28175000; // 70.175 MHz transverted into 10m Band

    first = false;
  }

  Icom706.setEnable( isRxControlEnabled() );

#ifdef DEBUG_SWITCH
  if ( (bool)digitalRead(kLEFT_SWITCH) != left_switch) {
    Serial.print("Left switch is ");
    left_switch = !left_switch;
    Serial.println((left_switch ? "ON" : "OFF"));

    delay(20);  // 'debouncing'
  }

  if ( (bool)digitalRead(kMIDDLE_SWITCH) != middle_switch) {
    Serial.print("Middle switch is ");
    middle_switch = !middle_switch;
    Serial.println((middle_switch ? "ON" : "OFF"));

    delay(20);  // 'debouncing'
  }

  if ( (bool)digitalRead(kRIGHT_SWITCH) != right_switch) {
    Serial.print("Right switch is ");
    right_switch = !right_switch;
    Serial.println((right_switch ? "ON" : "OFF"));

    delay(20);  // 'debouncing'
  }
#endif // DEBUG_SWITCH

  bool rxStatus = Icom706.run( transvControl.rxFrequency, transvControl.rxMode );

  bool txStatus = false;

  // write RX frequency to LCD
  if ( transvControl.rxFrequency != IcomCAT::ILLEGAL_FREQ && lcd_counter > 1000 ) {

    lcd_counter = millis();

    // convert the integer frequency into a string of 10 bytes length
    char freq_str[12];
    ultoa(transvControl.rxFrequency, freq_str, 10);

    // ... with leading blanks
    while ( strlen(freq_str) < 9 ) {
      freq_str[strlen(freq_str)+1] = 0;
      for ( int i=strlen(freq_str); i>0; --i)
        freq_str[i] = freq_str[i-1];
      freq_str[0] = ' ';
    }

    lcd.setCursor(3, 0);
    for ( int i=0; i<3; ++i )
      lcd.print(freq_str[i]);
    lcd.print('.');
    for ( int i=3; i<8; ++i )
      lcd.print(freq_str[i]);
  }

  // write rxStatus & txStatus to status LEDs
  if ( rxStatus ) {
    digitalWrite(kRX_GREEN_LED, HIGH);
    digitalWrite(kRX_RED_LED, LOW);
  }
  else {
    digitalWrite(kRX_GREEN_LED, LOW);
    digitalWrite(kRX_RED_LED, HIGH);
  }

  if ( txStatus ) {
    digitalWrite(kTX_GREEN_LED, HIGH);
    digitalWrite(kTX_RED_LED, LOW);
  }
  else {
    digitalWrite(kTX_GREEN_LED, LOW);
    digitalWrite(kTX_RED_LED, HIGH);
  }

  // display of frequency and mode from time to time
  if ( millis() - loop_counter > 2000 ) {

    loop_counter = millis();

#ifdef DEBUG
    Serial << F("RX(") << (int)rxStatus << F("): ")
           << F("Frequency = ") << transvControl.rxFrequency
           << F(" Mode = ") << transvControl.rxMode << endl;
    //Serial << F("TX(") << (int)txStatus << F("): ")
    //       << F("Frequency = ") << transvControl.txFrequency
    //       << F(" Mode = ") << transvControl.txMode << endl;
    //Serial << F("TX: Set frequency = ") << transvControl.desiredFrequency << endl;
#endif // DEBUG
  }
}
