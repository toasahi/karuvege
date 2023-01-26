extern void print280Values();
extern void setup280();
extern void bme280_PROC();

#include <Wire.h>
#include <ST7032.h>
#include <EEPROM.h>
#include <Adafruit_NeoPixel.h>

#include <HTTPClient.h>
#include <Adafruit_BME280.h>
#include <SSD1306.h>

#include "Getdata.h"
#include "NumberOfDaysUsed.h"

// 定数宣言
#define SEALEVELPRESSURE_HPA (1013.25)    // bme280用定数

//Wifi
HTTPClient http;
WiFiClientSecure client;

Adafruit_BME280 bme; // I2C
SSD1306  display(0x3c, 21, 22);

const char *wd[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

//WiFi設定
const char ssid[] = "C0p2Ec2-WLAN";
const char password[] = "d6ad418b63849f1d5c2fb20b3389c60787a9504bbe8900e77f4b6871fbc9da48";

const int moterPin = 16;
const int temperatureFanPin = 17;

// EEPROMのキー
int timeKey = 0;

// 苗を植えたときの日付が保存されているか
int timeStatus = 0;

int currentTime[6];

unsigned long delayTime;
unsigned bmeStatus;
static float bmeFloatValue;

const int LED_COUNT = 25;
const int ledPin = 18;

Adafruit_NeoPixel pixels(LED_COUNT, ledPin, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  delay(3000);

  Wire.begin(21, 22); // 端子の定義Wire.begin(SDA,SCL)
  display.init();
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 0, "ESP32 and OLED");
  display.display();

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
      esp_sleep_enable_timer_wakeup(1 * 1000 * 1000);
      esp_deep_sleep_start();
    }
  }
  Serial.println("Connected");

  setup280();

  //日本時間の設定
  configTime( 3600 * 9, 0, "ntp.nict.jp", "time.google.com", "ntp.jst.mfeed.ad.jp");
  configTzTime("JST-9", "ntp.nict.jp", "time.google.com", "ntp.jst.mfeed.ad.jp");
  delay(5000);

  getCurrentTime(currentTime);

  EEPROM.begin(16);

  // 一旦WiFiの接続を止める
  WiFi.mode(WIFI_OFF);

  saveNumberOfDaysUsed(timeKey, timeStatus, currentTime[3], currentTime[4], currentTime[5]);
  delay(2000);
  int days = getNumberOfDaysUsed(timeKey, currentTime[3], currentTime[4], currentTime[5]);
}

void loop() {
  int currentHour = currentTime[0];
  int currentMin = currentTime[1];
  int currentDay = currentTime[2];

  pixels.clear();

  //全点灯する
  for (int i = 0; i < LED_COUNT; i++) {
    pixels.setPixelColor(i, pixels.Color(255, 221, 170));
  }

  pixels.show();
  bme280_PROC();  // 環境センサメイン処理部

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

// BME280の取得＆出力
void bme280_PROC() {
  display.clear();
  print280Values();
}

//◆◆ BME280の状況を出力 ◆◆◆◆◆◆◆◆◆◆
void print280Values() {
  // 温度の計測＆OLED表示
  display.drawString(0, 0, "Temp = ");
  bmeFloatValue =  bme.readTemperature() ;  // 取得
  display.drawString( 60 , 0,  String(bmeFloatValue, 8));
  // 気圧の計測＆OLED表示
  display.drawString(0, 16, "Press = ");
  bmeFloatValue =  bme.readPressure() / 100.0F ;  // 取得
  display.drawString(60, 16, String (bmeFloatValue, 8));
  // 湿度の計測＆OLED表示
  display.drawString(0, 32, "Humi = ");
  bmeFloatValue =  bme.readHumidity() ;  // 取得
  display.drawString(60, 32, String (bmeFloatValue, 8));
  // 高度の計測＆OLED表示
  display.drawString(0, 48, "Altitu = ");
  bmeFloatValue =  bme.readAltitude(SEALEVELPRESSURE_HPA) ;
  display.drawString(60, 48, String (bmeFloatValue, 8));
  display.display();
}

// BME280の接続確認
void setup280() {
  bmeStatus = bme.begin(0x76);
  if (!bmeStatus) {
    display.drawString(0, 0,  "BME280 sensor");
    display.drawString(16, 16, "Connection");
    display.drawString(32, 32, "Error....!");
    display.display();
    Serial.println("Loop");
    while (1) delay(10);
  }
  display.drawString(8, 8, "BME280 Ready!");
  delay(500);
  delayTime = 500;
}
