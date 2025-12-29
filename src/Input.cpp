#include "Input.h"

// Lookup mapped keys per user layout
KeypadKey Input::getKeyFromMatrix(int row, int col) const {
  if (row == 0 && col == 0)
    return KEY_1;
  else if (row == 1 && col == 0)
    return KEY_2;
  else if (row == 2 && col == 0)
    return KEY_3;
  else if (row == 0 && col == 2)
    return KEY_4;
  else if (row == 1 && col == 2)
    return KEY_5;
  else if (row == 2 && col == 2)
    return KEY_6;
  else if (row == 0 && col == 1)
    return KEY_7;
  else if (row == 1 && col == 1)
    return KEY_8;
  else if (row == 2 && col == 1)
    return KEY_9;
  else if (row == 0 && col == 3)
    return KEY_10;
  else if (row == 1 && col == 3)
    return KEY_11;
  else if (row == 2 && col == 3)
    return KEY_12;
  else
    return KEY_NONE;
}

// Sets up all keypad pins: columns OUTPUT/HIGH, rows INPUT_PULLUP
void Input::initializeKeypad() {
  for (int c = 0; c < COLS; c++) {
    pinMode(colPins[c], OUTPUT);
    digitalWrite(colPins[c], HIGH); // inactive
  }
  for (int r = 0; r < ROWS; r++) {
    pinMode(rowPins[r], INPUT_PULLUP);
  }
}

void Input::updateKeypad() {
  for (int c = 0; c < COLS; c++) {
    digitalWrite(colPins[c], LOW); // Activate column
    for (int r = 0; r < ROWS; r++) {
      bool pressed = (digitalRead(rowPins[r]) == LOW);
      KeypadKey mapped = getKeyFromMatrix(r, c);
      if (mapped != KEY_NONE) {
        // Detect new press only if rising edge
        if (pressed && !lastKeyStates[r][c]) {
          if (keyCallback)
            keyCallback(mapped);
        }
        lastKeyStates[r][c] = pressed;
      }
    }
    digitalWrite(colPins[c], HIGH); // Deactivate column
  }
}
