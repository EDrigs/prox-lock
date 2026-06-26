#include "oled_display.h"
#include "config.h"

// Helper: draw text at coordinates
void OLEDDisplayController::drawText(int x, int y, int size, const char* text) {
    display.setTextSize(size);
    display.setCursor(x, y);
    display.print(text);
}

// Constructor
OLEDDisplayController::OLEDDisplayController() :
    display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {
}

bool OLEDDisplayController::begin() {
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        return false;
    }

    display.setTextColor(SSD1306_WHITE);
    display.clearDisplay();
    display.display();
    return true;
}

void OLEDDisplayController::displayClearStartup() {
    display.clearDisplay();
    display.display();
}

// Sliding animation helper 
void slideText(Adafruit_SSD1306 &display, const char *text, int x, int startY, int endY, int textSize, int delayMs = 25) {
    int step = (startY < endY) ? 1 : -1; // choose direction
    for (int y = startY; y != endY + step; y += step) {
        display.clearDisplay();
        display.setTextSize(textSize);
        display.setCursor(x, y);
        display.print(text);
        display.display();
        delay(delayMs);
    }
}

// Welcome Screen
void OLEDDisplayController::displayWelcomeScreen() {
    display.clearDisplay();

    // Slide "Hello!" down from above the screen to yellow zone (0 → 0)
    slideText(display, "Hello!", 30, -16, 0, 2, 20);

    // Hold it briefly before showing the rest
    delay(300);

    // Add instructions (blue zone)
    drawText(0, 28, 1, "Use keypad to unlock");
    display.display();
}

// Goodbye Screen
void OLEDDisplayController::displayGoodbyeScreen() {
    display.clearDisplay();

    // Slide "Goodbye!" upward from below the screen (y=70) to final position (y=24)
    slideText(display, "Goodbye!", 16, 70, 24, 2, 5);

    // Hold text briefly, then display footer
    delay(300);

    drawText(16, 54, 1, "Have a nice day!");
    display.display();
}

void OLEDDisplayController::displayOTPSent() {
    display.clearDisplay();
    drawText(0, 20, 1, "OTP Sent.");
    drawText(0, 36, 1, "Contact admin.");
    display.display();
}

void OLEDDisplayController::displayFingerprintMode() {
    display.clearDisplay();
    drawText(0, 20, 1, "Fingerprint Mode");
    drawText(0, 36, 1, "Place finger on Biometric Module.");
    display.display();
}


// Valid code message
void OLEDDisplayController::displayValidFingerprint() {
    display.clearDisplay();
    drawText(0, 16, 1, "Valid fingerprint!");
    drawText(0, 32, 1, "Unlocking...");
    display.display();
}

// Valid code message
void OLEDDisplayController::displayInvalidFingerprint() {
    display.clearDisplay();
    drawText(0, 16, 1, "Invalid Code");
    drawText(0, 32, 1, "Try Again");
    display.display();
}


// Valid code message
void OLEDDisplayController::displayValidCode() {
    display.clearDisplay();
    drawText(0, 16, 1, "Valid code!");
    drawText(0, 32, 1, "Unlocking...");
    display.display();
}

// Invalid code message
void OLEDDisplayController::displayInvalidCode() {
    display.clearDisplay();
    drawText(0, 16, 1, "Invalid code");
    drawText(0, 32, 1, "Try again");
    display.display();
}

void OLEDDisplayController::displayEditModeEntered() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.println("EDIT MODE");
  display.setCursor(0, 25);
  display.println("Enter new code");
  display.setCursor(0, 40);
  display.println("# = Save  * = Cancel");
  display.display();
}

void OLEDDisplayController::displayCodeSaved() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 20);
  display.println("CODE");
  display.setCursor(10, 40);
  display.println("SAVED!");
  display.display();
  delay(2000);
  display.clearDisplay();
  display.display();
}

void OLEDDisplayController::displayCodeSaveFailed() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 20);
  display.println("SAVE");
  display.setCursor(10, 40);
  display.println("FAILED");
  display.display();
  delay(2000);
  display.clearDisplay();
  display.display();
}

void OLEDDisplayController::displayCodeTooShort() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 20);
  display.println("CODE TOO SHORT");
  display.setCursor(10, 35);
  display.println("Min 4 digits");
  display.display();
  delay(2000);
  display.clearDisplay();
  display.display();
}

void OLEDDisplayController::displayEditCancelled() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(5, 25);
  display.println("CANCELLED");
  display.display();
  delay(1500);
  display.clearDisplay();
  display.display();
}

// Generic message display (reusable)
void OLEDDisplayController::displayMessage(const char *msg) {
    display.clearDisplay();
    drawText(0, 16, 1, msg);
    display.display();
}
