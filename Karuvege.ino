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

//取得した電圧の変数
int tempAnalogReading;
//電圧から算出した温度の変数
int temperature;

const int moterPin = 16;
const int temperatureFanPin = 17;

int currentHour;
int currentMin;
int currentDay;

const int timeSize = 6;

// EEPROMのキー
int timeKey = 0;

// 苗を植えたときの日付が保存されているか
int timeStatus = 0;

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
  
  //日本時間の設定
  configTime( JST, 0, "ntp.nict.jp", "time.google.com", "ntp.jst.mfeed.ad.jp");
  configTzTime("JST-9", "ntp.nict.jp", "time.google.com", "ntp.jst.mfeed.ad.jp");

  pinMode(moterPin, OUTPUT);

  getTemperature(); //温度センサ関数

  // LCD表示領域設定(8桁, 2行)
  lcd.begin(8, 2);

  // コントラスト設定(0〜63)
  lcd.setContrast(30);

  lcd.setCursor(0, 0);
  lcd.printf("Temp:%dC", temperature);

  lcd.setCursor(0, 1);
  lcd.printf("Hum:%d", 55);
  lcd.print("%");

  // 10秒間気温と湿度を表示する
  delay(10000);

  lcd.clear();

  EEPROM.begin(16);

  getCurrentTime(currentTime);
  
  // 一旦WiFiの接続を止める
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);

  saveNumberOfDaysUsed(timeKey,timeStatus,currentTime[3],currentTime[4],currentTime[5]);
  delay(2000);
  int days = getNumberOfDaysUsed(timeKey,currentTime[3],currentTime[4],currentTime[5]);
}

void loop() {
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

 getTemperature(); //温度センサ関数

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

//温度センサの値を取得する関数
void getTemperature(){
  

  tempAnalogReading = analogRead(voutPin);

  //温度を算出
  temperature = (tempAnalogReading - 424) / 6.25; //LM60BIZのデータシートにあった　VO = (+6.25 mV/°C × T °C) + 424 mV　という式を参考にしました。

//  Serial.println("電圧：" + String(tempAnalogReading) + "mV");
//  Serial.println("温度：" + String(temperature) + "℃");
  
}


/* ファンを回転させる
   boolean flag 停止：0 開始:1
*/

void temperatureFanControl(boolean flag){
  if (flag) {
    digitalWrite(temperatureFanPin, HIGH);
  } else {
    digitalWrite(temperatureFanPin, LOW);
  }
  delay(100);
}
