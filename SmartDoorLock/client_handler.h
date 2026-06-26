#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include <WiFiNINA.h>

void handleClient(WiFiClient &client);
void sendResponse(WiFiClient &client, int statusCode, const char* message);
void handleSetOTP(WiFiClient &client, const String &jsonBody);

#endif
