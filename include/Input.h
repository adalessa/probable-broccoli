#pragma once

#include <AiEsp32RotaryEncoder.h>
#include <Arduino.h>

// Key enum for named access and keypad API
enum KeypadKey {
  KEY_NONE = 0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9, KEY_10, KEY_11, KEY_12
};

typedef void (*KeyCallback)(KeypadKey key);

class Input {
public:
  void setEncoder(AiEsp32RotaryEncoder *enc) { encoder = enc; }
  AiEsp32RotaryEncoder *getEncoder() { return encoder; }

  // -- Keypad-related API --
  void initializeKeypad(); // pin setup
  void updateKeypad(); // scan for key events, fires callback

  // Listener registration (callback is called on key press)
  void setKeyCallback(KeyCallback cb) { keyCallback = cb; }

private:
  AiEsp32RotaryEncoder *encoder;
  // Keypad constants (match yours)
  static constexpr int COLS = 4;
  static constexpr int ROWS = 3;
  const int colPins[COLS] = {14, 15, 12, 13};
  const int rowPins[ROWS] = {26, 27, 25};

   // State
   bool lastKeyStates[ROWS][COLS] = {};


  // Utility
  KeypadKey getKeyFromMatrix(int row, int col) const;

  // Listener (callback)
  KeyCallback keyCallback = nullptr;
};
