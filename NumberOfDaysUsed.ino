
/* 現在時間を保存する
   timeKey キーになる
*/

void saveNumberOfDaysUsed(int timeKey, int timeStatus, int oldYear, int oldMonth, int oldDay) {

  EEPROM.get(timeKey + 12, timeStatus);

  if (timeStatus != 1) {
    EEPROM.put(timeKey, oldYear);
    EEPROM.put(timeKey + 4, oldMonth);
    EEPROM.put(timeKey + 8, oldDay);
    EEPROM.put(timeKey + 12, 1);
    Serial.println("test");
    EEPROM.commit();
  }
  // 書き込み確定
}



int isLeapYear(int uruYear) {
  if ((uruYear % 4 == 0 && uruYear % 100 != 0) || uruYear % 400 == 0) {
    return 1;
  } else {
    return 0;
  }
}


/* 利用した日数を取得する
   timeKey キーになる
*/

int getNumberOfDaysUsed(int timeKey, int currentYear, int currentMonth, int currentDay) {

  //  // 終了時間と開始時刻を取得する変数
  //  unsigned long endTime, getStartTime;
  //  EEPROM.get(timeKey, getStartTime);
  //
  //  // 何ミリ秒経っているかを計算
  //  endTime = millis() - getStartTime;
  //
  //  // 何日経っているかの計算
  //  int dayCount = endTime / conversionMillsToDate;
  //
  //  return dayCount;

  int yearStart, monthStart, dayStart;
  int yearDiff;
  int uruFlag;
  int uruCount = 0;

  int tukihi[] = {0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

  EEPROM.get(timeKey, yearStart);
  EEPROM.get(timeKey + 4, monthStart);
  EEPROM.get(timeKey + 8, dayStart);

  Serial.printf("yearStart:%d,monthStart:%d,dayStart:%d", yearStart, monthStart, dayStart);
  Serial.println("");
  Serial.printf("currentYear:%d,currentMonth:%d,currentDay:%d", currentYear, currentMonth, currentDay);
  Serial.println("");

  dayStart += tukihi[monthStart];
  currentDay += tukihi[currentMonth];

  yearDiff = currentYear - yearStart;

  if (monthStart < 3) {
    uruFlag = isLeapYear(yearStart);
    if (uruFlag == 1) {
      dayStart--;
    }
  }

  if (currentMonth >= 3) {
    uruFlag = isLeapYear(currentYear);
    if (uruFlag == 1) {
      dayStart++;
    }
  }

  currentYear--;

  for (int i = currentYear; currentYear > yearStart; currentYear--) {
    uruFlag = isLeapYear(currentYear);
    if (uruFlag == 1) {
      uruCount++;
    }
  }

  currentDay += 365 * yearDiff + uruCount - dayStart;
  Serial.println("");
  Serial.printf("%d日経過しました", currentDay);

  return currentDay;
}
