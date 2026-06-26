#ifndef OLED_DISPLAY_H
#define OLED_DISPLAY_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "config.h"



class OLEDDisplayController {
public:
    // Constructor
    OLEDDisplayController();

    bool begin();
    void displayClearStartup();
    void displayWelcomeScreen();
    void displayGoodbyeScreen();
    void displayOTPSent();
    void displayValidCode();
    void displayInvalidCode();
    void displayEditModeEntered();
    void displayCodeSaved();
    void displayCodeSaveFailed();
    void displayCodeTooShort();
    void displayEditCancelled();
    void displayValidFingerprint();
    void displayInvalidFingerprint();
    void displayFingerprintMode();
    void displayMessage(const char *msg);
    
private:
    // The Adafruit SSD1306 object is a private member of the class
    Adafruit_SSD1306 display;

    // Helper function to handle text rendering (clearing, setting cursor, etc.)
    void drawText(int x, int y, int size, const char* text);
};

#endif