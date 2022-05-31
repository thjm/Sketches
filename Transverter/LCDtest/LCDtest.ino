
//
// Test code for LCM1602 I2C-LCD interface module used in the 4m Transverter
//
// It uses a variant for PCF8574 I2C extender chips:
// http://playground.arduino.cc/Code/LCDi2c
//
// see also: Sketchbook/Display/LCM1602test
//

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// set the LCD address to 0x27 for a 20 chars and 2 line display
LiquidCrystal_I2C lcd(0x27,20,2);

void setup() {

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("LCM1602 test ...");
  lcd.setCursor(0, 1);
  lcd.print("Voltage: ");
  lcd.setCursor(13, 1);
  lcd.print("V");
}

void loop() {

  int val;
  float temp;

  val = analogRead(0);
  temp = val/4.092;
  val = (int)temp;

  lcd.setCursor(9, 1);
  lcd.print((uint8_t)(0x30+val/100));
  lcd.print((uint8_t)(0x30+(val%100)/10));
  lcd.print('.');
  lcd.print((uint8_t)(0x30+val%10));

  delay(100);
}
