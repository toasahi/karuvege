#include <Wire.h>
#include <ST7032.h>
#include "Getdata.h"
#include <HTTPClient.h>

//Wifi
HTTPClient http;
WiFiClientSecure client;

ST7032 lcd;

//日本標準時子午線
const int JST = 3600 * 9;
const char *wd[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

//WiFi設定
const char ssid[] = "C0p2Ec2-WLAN";
const char password[] = "d6ad418b63849f1d5c2fb20b3389c60787a9504bbe8900e77f4b6871fbc9da48";

const int moterPin = 16;

unsigned long currentHour;
unsigned long currentMin;
unsigned long currentDay;

unsigned long currentTime[3];

void setup() {
  Serial.begin(115200);
  //WiFi接続
  Serial.print("Connecting to");
  Serial.print(ssid);
  WiFi.begin(ssid, password);
  int wifi_count = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    //通信状況が悪い時リスタート
    if (++wifi_count > 30) {
      Serial.print("リスタート");
    }
  }
  Serial.println("Connected");

  //日本時間の設定
  configTime( JST, 0, "ntp.nict.jp", "time.google.com", "ntp.jst.mfeed.ad.jp");
  configTzTime("JST-9", "ntp.nict.jp", "time.google.com", "ntp.jst.mfeed.ad.jp");

  pinMode(moterPin, OUTPUT);

  // LCD表示領域設定(8桁, 2行)
  lcd.begin(8, 2);

  // コントラスト設定(0〜63)
  lcd.setContrast(30);

  lcd.setCursor(0, 0);
  lcd.printf("Temp:%dC", 20);

  lcd.setCursor(0, 1);
  lcd.printf("Hum:%d", 55);
  lcd.print("%");

  delay(10000);

  lcd.clear();
}

void loop() {
  getCurrentTime(currentTime);
  currentHour = currentTime[0];
  currentMin = currentTime[1];
  currentDay = currentTime[2];

  //(1行目)
  lcd.setCursor(0, 0);
  if (currentMin < 9) {
    lcd.printf("%d:0%d", currentHour, currentMin);
  } else {
    lcd.printf("%d:%d", currentHour, currentMin);
  }
  // (2行目)
  lcd.setCursor(0, 1);
  lcd.print(wd[currentDay]);

  delay(1000);
  lcd.clear();
}

/* モーターを回転させる
   int flag 停止：0 開始:1
*/

void moterControl(int flag) {
  if (flag) {
    digitalWrite(moterPin, HIGH);
  } else {
    digitalWrite(moterPin, LOW);
  }
  delay(100);
}
