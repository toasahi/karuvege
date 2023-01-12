
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

/* その年に閏年があるか
   int uruYear 年
*/

private int isLeapYear(int uruYear) {
  if ((uruYear % 4 == 0 && uruYear % 100 != 0) || uruYear % 400 == 0) {
    return 1;
  } else {
    return 0;
  }
}


/* 利用した日数を取得する
   int timeKey キーになる(0,1)
   int currentYear 現在の年代
   int currentMonth　現在の月
   int currentDay　現在の日
*/

int getNumberOfDaysUsed(int timeKey, int currentYear, int currentMonth, int currentDay) {
  
  int yearStart, monthStart, dayStart;
  int yearDiff;
  int uruFlag;
  int uruCount = 0;

  //1月〜12月までの日数
  int tukihi[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

  EEPROM.get(timeKey, yearStart);
  EEPROM.get(timeKey + 4, monthStart);
  EEPROM.get(timeKey + 8, dayStart);

  Serial.printf("yearStart:%d,monthStart:%d,dayStart:%d", yearStart, monthStart, dayStart);
  Serial.println("");
  Serial.printf("currentYear:%d,currentMonth:%d,currentDay:%d", currentYear, currentMonth, currentDay);
  Serial.println("");

  dayStart += tukihi[monthStart-1];
  currentDay += tukihi[currentMonth-1];

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
