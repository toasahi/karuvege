#include "Getdata.h"

unsigned long getTime(String label) {
  time_t t;
  struct tm *tm;
  t = time(NULL);
  tm = localtime(&t);
  if (label == "hour") {
    return tm->tm_hour;
  } else if (label == "min") {
    return tm->tm_min;
  };
  return tm->tm_wday;
}
