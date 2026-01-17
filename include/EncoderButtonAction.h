#pragma once

#include "WString.h"
class EncoderButtonAction {
public:
  EncoderButtonAction() {};
  String toString() {
    return "{\"action\": \"encoder_button\", \"data\": {\"clicked\": true}}";
  };
};
