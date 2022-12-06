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


  //  currentTime[0] = tm->tm_hour;
  //  currentTime[1] = tm->tm_min;
  //  currentTime[2] =tm->tm_wday;

  int timeYear = tm->tm_year + 1900;
  int timeMonth = tm->tm_mon + 1;
  int timeDay = tm->tm_mday;
  int timeHour = tm->tm_hour;
  int timeMin = tm->tm_min;
  int timeWday = tm->tm_wday;

  
  currentTime[0] = timeYear;
  currentTime[1] = timeMonth;
  currentTime[2] = timeDay;
  currentTime[3] = timeHour;
  currentTime[4] = timeMin;
  currentTime[5] = timeWday;
  
}
