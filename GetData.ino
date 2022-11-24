#include "Getdata.h"

void getCurrentTime(unsigned long currentTime[3]) {
  time_t t;
  struct tm *tm;
  t = time(NULL);
  tm = localtime(&t);
  currentTime[0] = tm->tm_hour;
  currentTime[1] = tm->tm_min;
  currentTime[2] =tm->tm_wday;
}
