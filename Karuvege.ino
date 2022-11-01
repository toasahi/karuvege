#include <Wire.h>
#include <LiquidCrystal_I2C.h>
 
const unsigned int ADDRESS = 0x27;
const int CHARS_NUM = 16;
const int LINES_NUM = 2;
 
LiquidCrystal_I2C lcd(ADDRESS, CHARS_NUM, LINES_NUM);
 
void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
}
 
void loop() {
  lcd.setCursor(0, 0);
  lcd.print("Hello World!");
 
  lcd.setCursor(0, 1);
  lcd.print("DISP TEST");
 
  delay(100);
}
