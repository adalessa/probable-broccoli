#pragma once

#include "Input.h"
#include "WString.h"
class KeypadAction {
private:
  KeypadKey key;

public:
  KeypadAction(KeypadKey key) : key(key) {};
  String toString() {
    return "{\"action\": \"keypad\", \"data\": {\"key\": " + String((int)key) +
           "}}";
  };
};
