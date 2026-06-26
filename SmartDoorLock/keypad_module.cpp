#include "keypad_module.h"
#include "config.h"
#include <Keypad.h>

static const byte ROWS = KEYPAD_ROWS;
static const byte COLS = KEYPAD_COLS;

static const char hexaKeys[ROWS][COLS] = KEYPAD_KEYS;
static byte rowPins[ROWS] = ROW_PINS;
static byte colPins[COLS] = COL_PINS;

static Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

void initKeypad() {}

char getKeypadInput() {
  return customKeypad.getKey();
}
