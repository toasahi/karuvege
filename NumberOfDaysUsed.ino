
/* 現在時間を保存する
   timeKey キーになる
*/

void saveNumberOfDaysUsed(int timeKey,int timeStatus) {

  // 開始時間
  unsigned long startTime;
  // 時間を測定する
  startTime = millis();

  EEPROM.get(timeKey+4, timeStatus);

  if(timeStatus != 1){
    EEPROM.put(timeKey, startTime);
    EEPROM.put(timeKey+4, 1);
    Serial.println("test");
  }
  Serial.println(timeStatus);
  // 書き込み確定
  EEPROM.commit();
}

/* 利用した日数を取得する
   timeKey キーになる
*/

int getNumberOfDaysUsed(int timeKey) {

  // 終了時間と開始時刻を取得する変数
  unsigned long endTime, getStartTime;
  EEPROM.get(timeKey, getStartTime);

  // 何ミリ秒経っているかを計算
  endTime = millis() - getStartTime;

  // 何日経っているかの計算
  int dayCount = endTime / conversionMillsToDate;

  return dayCount;
}
