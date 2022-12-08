#include "Getdata.h"

/* 現在時間を取得する関数
   currentTime[0] 時
   currentTime[1] 分
   currentTime[2] 曜日の曜日
*/

void getCurrentTime(int currentTime[6]) {
  time_t t;
  struct tm *tm;
  t = time(NULL);
  tm = localtime(&t);

  //  double test = difftime(t,1669864160);
  //  Serial.println(test);
  
  currentTime[0] = tm->tm_year + 1900;
  currentTime[1] = tm->tm_mon + 1;
  currentTime[2] = tm->tm_mday;
  currentTime[3] = tm->tm_hour;
  currentTime[4] = tm->tm_min;
  currentTime[5] = tm->tm_wday;
  
}
