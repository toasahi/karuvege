#include <Wire.h>
#include <ST7032.h>

ST7032 lcd;

void setup() {
  // LCD表示領域設定(8桁, 2行)
  lcd.begin(8, 2);

  // コントラスト設定(0〜63)
  lcd.setContrast(30);

  // (1行目)
  lcd.setCursor(0, 0);
  lcd.print("I Love");

  // (2行目)
  lcd.setCursor(0, 1);
  lcd.print("Arduino!");
}

void loop() {
}
