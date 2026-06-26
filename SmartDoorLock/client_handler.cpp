#include "client_handler.h"
#include "set_passcode.h"
#include "secrets.h"
#include "otp_manager.h"
#include <ArduinoJson.h>

static String readHTTPRequest(WiFiClient &client);
static String extractJSONBody(const String &request);
static String extractRoute(const String &request);

void handleClient(WiFiClient &client) {
    String request = readHTTPRequest(client);

    if (request.length() == 0) {
        sendResponse(client, 400, "Empty Request");
        client.stop();
        return;
    }

    String route = extractRoute(request);

    if (route == "POST /set_otp") {
        handleSetOTP(client, extractJSONBody(request));
    } else {
        sendResponse(client, 404, "Not Found");
    }

    client.stop();
}

static String readHTTPRequest(WiFiClient &client) {
    String request = "";
    const unsigned long timeout = 5000;
    unsigned long start = millis();

    // Read until end-of-headers marker
    while (client.connected() && millis() - start < timeout) {
        while (client.available()) {
            request += (char)client.read();
            start = millis();
            if (request.endsWith("\r\n\r\n")) goto headers_done;
        }
        delay(5);
    }
headers_done:

    if (request.length() == 0) return "";

    // Parse Content-Length (case-insensitive)
    int clIndex = request.indexOf("\r\nContent-Length:");
    if (clIndex == -1) clIndex = request.indexOf("\r\ncontent-length:");

    int contentLength = 0;
    if (clIndex != -1) {
        int lineEnd = request.indexOf("\r\n", clIndex + 2);
        if (lineEnd != -1) {
            String clLine = request.substring(clIndex + 2, lineEnd);
            int colon = clLine.indexOf(':');
            if (colon != -1) {
                String val = clLine.substring(colon + 1);
                val.trim();
                contentLength = val.toInt();
            }
        }
    }

    // Read body
    if (contentLength > 0) {
        unsigned long bodyStart = millis();
        int headerEnd = request.indexOf("\r\n\r\n");
        while (client.connected()
               && (int)(request.length() - headerEnd - 4) < contentLength
               && millis() - bodyStart < timeout) {
            while (client.available()) {
                request += (char)client.read();
                bodyStart = millis();
            }
            delay(5);
        }
    }

    return request;
}

static String extractRoute(const String &request) {
    int end = request.indexOf("\r\n");
    return (end == -1) ? "" : request.substring(0, end);
}

static String extractJSONBody(const String &request) {
    int idx = request.indexOf("\r\n\r\n");
    return (idx == -1) ? "" : request.substring(idx + 4);
}

void handleSetOTP(WiFiClient &client, const String &jsonBody) {
    StaticJsonDocument<256> doc;
    DeserializationError err = deserializeJson(doc, jsonBody);

    if (err) {
        sendResponse(client, 400, "Invalid JSON");
        return;
    }

    if (String(doc["secret"].as<const char*>()) != API_SECRET) {
        sendResponse(client, 403, "Unauthorized");
        return;
    }

    String otp = doc["otp"];
    sendResponse(client, storeOTP(otp) ? 200 : 400, storeOTP(otp) ? "OTP stored" : "Invalid OTP");
}

void sendResponse(WiFiClient &client, int statusCode, const char *message) {
    const char* reason;
    switch (statusCode) {
        case 200: reason = "OK";          break;
        case 400: reason = "Bad Request"; break;
        case 403: reason = "Forbidden";   break;
        case 404: reason = "Not Found";   break;
        default:  reason = "Error";       break;
    }

    client.print("HTTP/1.1 ");
    client.print(statusCode);
    client.print(" ");
    client.println(reason);
    client.println("Content-Type: text/plain");
    client.println("Connection: close");
    client.println();
    client.println(message);
}
