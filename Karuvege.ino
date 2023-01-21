extern void print280Values();
extern void setup280();
extern void bme280_PROC();

#include <Wire.h>
#include <ST7032.h>
#include <EEPROM.h>

#include <HTTPClient.h>
#include <Adafruit_BME280.h>
#include <SSD1306.h>

//#include "Getdata.h"
#include "NumberOfDaysUsed.h"

//Wifi
HTTPClient http;
WiFiClientSecure client;

const char *wd[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

//WiFi設定
//const char ssid[] = "C0p2Ec2-WLAN";
//const char password[] = "d6ad418b63849f1d5c2fb20b3389c60787a9504bbe8900e77f4b6871fbc9da48";

const char ssid[] = "HUMAX-01F2B";
const char password[] = "djFhMaN5LWFJN";

const int moterPin = 16;
const int temperatureFanPin = 17;

// EEPROMのキー
int timeKey = 0;

// 苗を植えたときの日付が保存されているか
int timeStatus = 0;

int currentTime[6];

// 定数宣言
#define SEALEVELPRESSURE_HPA (1013.25)    // bme280用定数

// class宣言
Adafruit_BME280 bme; // I2C
SSD1306  display(0x3c, 21, 22); 

// 変数宣言     "0123456789ABCDEF"
unsigned long delayTime;
unsigned status;
static int value;     //analog値を代入する変数を定義
static float Volts;
static float FloatValue;
unsigned static int Val_ad;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Wire.begin(21, 22); // 端子の定義Wire.begin(SDA,SCL)
  display.init();
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 0, "ESP32 and OLED");
  display.display();

  setup280();


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
  configTime( 3600 * 9, 0, "ntp.nict.jp", "time.google.com", "ntp.jst.mfeed.ad.jp");
  configTzTime("JST-9", "ntp.nict.jp", "time.google.com", "ntp.jst.mfeed.ad.jp");
  getCurrentTime(currentTime);

  EEPROM.begin(16);

  // 一旦WiFiの接続を止める
  WiFi.mode(WIFI_OFF);
  delay(5000);
  //  delay(5000);

  //  saveNumberOfDaysUsed(timeKey, timeStatus, currentTime[3], currentTime[4], currentTime[5]);
  //  delay(2000);
  //  int days = getNumberOfDaysUsed(timeKey, currentTime[3], currentTime[4], currentTime[5]);
}

void loop() {
  int currentHour = currentTime[0];
  int currentMin = currentTime[1];
  int currentDay = currentTime[2];


  bme280_PROC();  // 環境センサメイン処理部
  delay(500);

  if (currentMin < 9) {
    //    lcd.printf("%d:0%d", currentHour, currentMin);
  } else {
    //    lcd.printf("%d:%d", currentHour, currentMin);
  }

  delay(1000);
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
  FloatValue =  bme.readTemperature() ;  // 取得
  display.drawString( 60 , 0,  String(FloatValue, 8));
  // 気圧の計測＆OLED表示
  display.drawString(0, 16, "Press = ");
  FloatValue =  bme.readPressure() / 100.0F ;  // 取得
  display.drawString(60, 16, String (FloatValue, 8));
  // 湿度の計測＆OLED表示
  display.drawString(0, 32, "Humi = ");
  FloatValue =  bme.readHumidity() ;  // 取得
  display.drawString(60, 32, String (FloatValue, 8));
  // 高度の計測＆OLED表示
  display.drawString(0, 48, "Altitu = ");
  FloatValue =  bme.readAltitude(SEALEVELPRESSURE_HPA) ;
  display.drawString(60, 48, String (FloatValue, 8));
  display.display();
}

//-------------------------------------------------------------
// BME280の接続確認
void setup280() {
  status = bme.begin(0x76);
  if (!status) {
    display.drawString(0, 0,  "BME280 sensor");
    display.drawString(16, 16, "Connection");
    display.drawString(32, 32, "Error....!");
    display.display();
    while (1) delay(10);
  }
  display.drawString(8, 8, "BME280 Ready!");
  delay(500);
  delayTime = 500;
}

void getCurrentTime(int currentTime[6]) {
  time_t t;
  struct tm *tm;
  t = time(NULL);
  tm = localtime(&t);

  currentTime[0] = tm->tm_hour;
  currentTime[1] = tm->tm_min;
  currentTime[2] = tm->tm_wday;
  currentTime[3] = tm->tm_year + 1900;
  currentTime[4] = tm->tm_mon + 1;
  currentTime[5] = tm->tm_mday;
}
