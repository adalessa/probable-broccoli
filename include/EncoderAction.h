#pragma once

#include "WString.h"
class EncoderAction {
private:
  int value;

public:
  EncoderAction(int value) : value(value) {};
  String toString() {
    return "{\"action\": \"encoder\", \"data\": {\"value\": " + String(value) +
           "}}";
  };
};
