#include "set_time.h"
#include <WiFiNINA.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <RTCZero.h>

// Wi-Fi / NTP setup
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", -18000, 60000);
// RTC instance
RTCZero rtc;

void setTimeFromNTP() {
  Serial.println("Getting time from NTP...");

  timeClient.begin();

  while (!timeClient.update()) {
    timeClient.forceUpdate();
  }

  unsigned long epochTime = timeClient.getEpochTime();
  Serial.print("Epoch time from NTP: ");
  Serial.println(epochTime);

  rtc.begin();
  rtc.setEpoch(epochTime);

  Serial.println("RTC set from NTP.");
}

void printCurrentTime() {
  char buf[20];
  sprintf(buf, "%02d:%02d:%02d %02d/%02d/%04d",
          rtc.getHours(),
          rtc.getMinutes(),
          rtc.getSeconds(),
          rtc.getMonth(),
          rtc.getDay(),
          rtc.getYear() + 2000);

  Serial.println(buf);
  Serial.println();
}
