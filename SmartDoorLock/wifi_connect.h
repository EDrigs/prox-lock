#ifndef WIFI_CONNECT_H
#define WIFI_CONNECT_H

#include <WiFiNINA.h>

// Function declarations
bool connectToWiFi();
void printWiFiStatus();

extern char ssid[];
extern char pass[];

#endif
