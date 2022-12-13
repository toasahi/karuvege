//#include "Getdata.h"

/* 現在時間を取得する関数
   currentTime[0] 時
   currentTime[1] 分
   currentTime[2] 曜日の曜日
*/

void getCurrentTime(int currentTime[3]) {
  time_t t;
  struct tm *tm;
  t = time(NULL);
  tm = localtime(&t);

  //  double test = difftime(t,1669864160);
  //  Serial.println(test);
  
  currentTime[0] = tm->tm_hour;
  currentTime[1] = tm->tm_min;
  currentTime[2] = tm->tm_wday;
  
}
