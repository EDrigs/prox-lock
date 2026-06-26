#include "wifi_connect.h"
#include "secrets.h"
#include <SPI.h>

static char ssid[] = SECRET_SSID;
static char pass[] = SECRET_PASS;

WiFiSSLClient client;

bool connectToWiFi() {
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("[WiFi] Module not found.");
    return false;
  }

  if (WiFi.firmwareVersion() < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("[WiFi] Firmware update recommended.");
  }

  const int maxAttempts = 10;
  for (int attempt = 1; attempt <= maxAttempts; attempt++) {
    Serial.print("[WiFi] Connecting, attempt ");
    Serial.println(attempt);

    WiFi.begin(ssid, pass);

    for (int i = 0; i < 10; i++) {
      if (WiFi.status() == WL_CONNECTED) break;
      delay(1000);
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("[WiFi] Connected.");
      printWiFiStatus();
      return true;
    }

    delay(2000);
  }

  Serial.println("[WiFi] Failed to connect after all attempts.");
  return false;
}

void printWiFiStatus() {
  Serial.print("[WiFi] SSID: ");
  Serial.println(WiFi.SSID());

  Serial.print("[WiFi] IP: ");
  Serial.println(WiFi.localIP());

  Serial.print("[WiFi] RSSI: ");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");
}
