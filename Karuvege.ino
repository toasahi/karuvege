#include <Wire.h>
#include <Preferences.h>
#include <ST7032.h>
#include <EEPROM.h>

#include <HTTPClient.h>

#include "Getdata.h"
#include "NumberOfDaysUsed.h"

//Wifi
HTTPClient http;
WiFiClientSecure client;

Preferences preferences;

ST7032 lcd;

//ピン
int voutPin = 35;

//日本標準時子午線
const int JST = 3600 * 9;
const char *wd[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

//WiFi設定
const char ssid[] = "C0p2Ec2-WLAN";
const char password[] = "d6ad418b63849f1d5c2fb20b3389c60787a9504bbe8900e77f4b6871fbc9da48";


//温度センサの値を取得する関数
void temp() {

  // R1の電圧を取得
  float reading = analogRead(voutPin);

  // AD値をmVに変換
  //float voltage = ((long)reading * VOLT * 1000) / ANALOG_MAX;

  //温度を算出
  //float temprature = (voltage - 424) / 6.25 //LM60BIZのデータシートにあった　VO = (+6.25 mV/°C × T °C) + 424 mV　という式を参考にしました。
  //float temprature = (voltage - 424) / 6.25//T = ( Vo-424[mV] ) / 6.25[mV]

  Serial.println(reading);

}

const int moterPin = 16;

int currentHour;
int currentMin;
int currentDay;

const int timeSize = 3;

// EEPROMのキー
int timeKey = 0;

// ミリ秒を日に変換する定数
const unsigned long conversionMillsToDate = 86400000;

int currentTime[timeSize];

void setup() {
  Serial.begin(115200);
  delay(1000);
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
  //
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

  // 10秒間気温と湿度を表示する
  delay(10000);

  lcd.clear();

  EEPROM.begin(16);
  // 一旦WiFiの接続を止める
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);

  saveNumberOfDaysUsed(timeKey);
  delay(2000);
  int days = getNumberOfDaysUsed(timeKey);
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

  //temp();
  //  Serial.println(analogRead(voutPin));

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
